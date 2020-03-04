
"""SolTrack: a simple, free, fast and accurate C routine to compute the position of the Sun.
  
  Copyright (c) 2014-2020  Marc van der Sluys, Paul van Kan and Jurgen Reintjes,
  Sustainable Energy research group, HAN University of applied sciences, Arnhem, The Netherlands
   
  This file is part of the SolTrack package, see: http://soltrack.sourceforge.net
  SolTrack is derived from libTheSky (http://libthesky.sourceforge.net) under the terms of the GPL v.3
  
  This is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General
  Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.
  
  This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
  License for more details.
  
  You should have received a copy of the GNU Lesser General Public License along with this code.  If not, see
  <http://www.gnu.org/licenses/>.

"""


import math as m
import numpy as np
import soltrack as st
from soltrack.data import PI,TWO_PI, R2D,R2H
from soltrack.dataclasses import Location, Time, RiseSet, copyDataclass


def computeSunRiseSet(location, time, rsAlt=0.0, useDegrees=False, useNorthEqualsZero=False):
    """Compute rise, transit and set times for the Sun, as well as their azimuths/altitude.
    
    Parameters:
      location           (Location):  Dataclass containing the geographic location to compute the Sun's rise and set times for.
      time               (Time):      Dataclass containing date and time to compute the position for, in UT.
      rsAlt              (float):     Altitude to return rise/set data for (radians; optional, default=0.0 meaning actual rise/set).  Set rsAlt>pi/2 to compute transit only.
      useDegrees         (bool):      Use degrees for input and output angular variables, rather than radians (optional, default=False).
      useNorthEqualsZero (bool):      Use the definition where azimuth=0 denotes north, rather than south (optional, default=False).
    
    Returns:
      (RiseSet):   Dataclass containing the Sun's rise, transit and set data.
    
    Note:
      - if rsAlt == 0.0, actual rise and set times are computed
      - if rsAlt != 0.0, the routine calculates when alt = rsAlt is reached
      - returns times, rise/set azimuth and transit altitude in the dataclass riseSet
     
      See:
        - subroutine riset() in riset.f90 from libTheSky (libthesky.sf.net) for more info
    
    """
    
    tmRad = np.zeros(3)
    azalt = np.zeros(3)
    alt=0.0;  ha=0.0; h0=0.0
    
    computeRefrEquatorial = True  # Compure refraction-corrected equatorial coordinates (Hour angle, declination).
    computeDistance = False       # Compute the distance to the Sun in AU.
    
    rsa = -0.8333/R2D                   # Standard altitude for the Sun in radians
    if(abs(rsAlt) > 1.e-9): rsa = rsAlt    # Use a user-specified altitude
    
    # If the used uses degrees, convert the geographic location to radians:
    # This was a local variable llocation in C
    
    loc = copyDataclass(Location, location)  # Local instance of the Location dataclass, so that it can be changed
    if(useDegrees):
        loc.longitude /= R2D
        loc.latitude  /= R2D
    
    
    # Set date and time to midnight UT for the desired day:
    rsTime        = Time()  # Local instance of the dataclass Time
    
    rsTime.year   = time.year
    rsTime.month  = time.month
    rsTime.day    = time.day
    
    rsTime.hour   = 0
    rsTime.minute = 0
    rsTime.second = 0.0
    
    # Compute the Sun's position.  Returns a Position object:
    pos = st.computeSunPosition(loc, rsTime, False, useNorthEqualsZero, computeRefrEquatorial, computeDistance)  # useDegrees = False: NEVER use degrees internally!
    
    agst0 = pos.agst      # AGST for midnight
    
    evMax = 3                  # Compute transit, rise and set times by default (1-3)
    cosH0 = (m.sin(rsa)-m.sin(loc.latitude)*m.sin(pos.declination)) / \
        (m.cos(loc.latitude)*m.cos(pos.declination))
    
    if(abs(cosH0) > 1.0):      # Body never rises/sets
        evMax = 1              # Compute transit time and altitude only
    else:
        h0 = m.acos(cosH0) % PI  # Should probably work without %

    
    tmRad[0] = (pos.rightAscension - loc.longitude - pos.agst) % TWO_PI  # Transit time in radians; lon0 > 0 for E
    if(evMax > 1):
        tmRad[1] = (tmRad[0] - h0) % TWO_PI   # Rise time in radians
        tmRad[2] = (tmRad[0] + h0) % TWO_PI   # Set time in radians
       
    
    accur = 1.0e-5        # Accuracy;  1e-5 rad ~ 0.14s. Don't make this smaller than 1e-16
    for evi in range(evMax):  # Loop over transit, rise, set
        iter = 0
        dTmRad = m.inf
        
        while(abs(dTmRad) > accur):
            th0 = agst0 + 1.002737909350795*tmRad[evi]  # Solar day in sidereal days in 2000
            
            rsTime.second = tmRad[evi]*R2H*3600.0       # Radians -> seconds - w.r.t. midnight (h=0,m=0)
            pos = st.computeSunPosition(loc, rsTime, False, useNorthEqualsZero, computeRefrEquatorial, computeDistance)  # useDegrees = False: NEVER use degrees internally!
            
            ha  = revPI(th0 + loc.longitude - pos.rightAscension)        # Hour angle: -PI - +PI
            alt = m.asin(m.sin(loc.latitude)*m.sin(pos.declination) +
                         m.cos(loc.latitude)*m.cos(pos.declination)*m.cos(ha))  # Altitude
            
            # Correction to transit/rise/set times:
            if(evi==0):           # Transit
                dTmRad = -revPI(ha)  # -PI - +PI
            else:                 # Rise/set
                dTmRad = (alt-rsa)/(m.cos(pos.declination)*m.cos(loc.latitude)*m.sin(ha))
                
            tmRad[evi] = tmRad[evi] + dTmRad
            
            # Print debug output to stdOut:
            # print(" %4i %2i %2i  %2i %2i %9.3lf    " % (rsTime.year,rsTime.month,rsTime.day, rsTime.hour,rsTime.minute,rsTime.second))
            # print(" %3i %4i   %9.3lf %9.3lf %9.3lf \n" % (evi,iter, tmRad[evi]*24,abs(dTmRad)*24,accur*24))
            
            iter += 1
            if(iter > 30): break  # while loop doesn't seem to converge
        # while(abs(dTmRad) > accur)
        
        
        if(iter > 30):  # Convergence failed
            print("\n  *** WARNING:  riset():  Riset failed to converge: %i %9.3lf  ***\n" % (evi,rsAlt))
            tmRad[evi] = -m.inf
            azalt[evi] = -m.inf
        else:               # Result converged, store it
            if(evi == 0):
                azalt[evi] = alt                                                                      # Transit altitude
            else:
                azalt[evi] = m.atan2( m.sin(ha), ( m.cos(ha) * m.sin(loc.latitude)  -
                                                   m.tan(pos.declination) * m.cos(loc.latitude) ) )   # Rise,set hour angle -> azimuth
        
        
        if(tmRad[evi] < 0.0 and abs(rsAlt) < 1.e-9):
            tmRad[evi] = -m.inf
            azalt[evi] = -m.inf
            
    # for-loop evi
    
    
    # Set north to zero radians for azimuth if desired:
    if(useNorthEqualsZero):
        azalt[1] = (azalt[1] + PI) % TWO_PI  # Add PI and fold between 0 and 2pi
        azalt[2] = (azalt[2] + PI) % TWO_PI  # Add PI and fold between 0 and 2pi
    
    
    # Convert resulting angles to degrees if desired:
    if(useDegrees):
        azalt[0] *= R2D   # Transit altitude
        azalt[1] *= R2D   # Rise azimuth
        azalt[2] *= R2D   # Set azimuth
        
        
    # Store results:
    riseSet = RiseSet()  # Instance of the dataclass RiseSet, to store and return the results
    
    riseSet.transitTime     = tmRad[0]*R2H  # Transit time - radians -> hours
    riseSet.riseTime        = tmRad[1]*R2H  # Rise time - radians -> hours
    riseSet.setTime         = tmRad[2]*R2H  # Set time - radians -> hours
    
    riseSet.transitAltitude = azalt[0]      # Transit altitude
    riseSet.riseAzimuth     = azalt[1]      # Rise azimuth
    riseSet.setAzimuth      = azalt[2]      # Set azimuth
    
    return riseSet



def revPI(angle):
    """Fold an angle in radians to take a value between -PI and +PI.
    
    Parameters:
      angle (float):  Angle to fold (rad).
    
    """
    
    return ((angle + PI) % TWO_PI) - PI


