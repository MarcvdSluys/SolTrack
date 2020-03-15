
"""SolTrack: a simple, free, fast and accurate C routine to compute the position of the Sun.
    
  Copyright (c) 2014-2020 Marc van der Sluys, Paul van Kan and Jurgen Reintjes, 
  Sustainable Energy research group, HAN University of applied sciences, Arnhem, The Netherlands
   
  This file is part of the SolTrack package, see: http://soltrack.sourceforge.net SolTrack is derived from
  libTheSky (http://libthesky.sourceforge.net) under the terms of the GPL v.3
  
  This is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General
  Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.
  
  This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
  License for more details.
  
  You should have received a copy of the GNU Lesser General Public License along with this code.  If not, see
  <http://www.gnu.org/licenses/>.

"""

# Create dataclasses, used like C structs.
# 
# Note, dataclasses new in Python 3.7.  For Python 3.6, (pip) install dataclasses.
#   - https://stackoverflow.com/a/45426493/1386750

from dataclasses import dataclass
import pytz as tz


@dataclass
class Location:
    """Dataclass containing the geographic location to compute the Sun position for."""
    
    longitude:   float = 0.0
    latitude:    float = 0.0
    
    sinLat:      float = 0.0
    cosLat:      float = 0.0
    
    pressure:    float = 101.0  # kPa
    temperature: float = 283.0  # Kelvin
    

@dataclass
class Time:
    """Dataclass containing the date and time (in UT) to compute the Sun position for."""
    
    year:   int   = 2000
    month:  int   = 1
    day:    int   = 1
    
    hour:   int   = 12
    minute: int   = 0
    second: float = 0.0
    
    
    def datetime2st(dtObj):
        """Convert a datetime object to a SolTrack time object.
        
           Args:
               dtObj (datetime):  Date and time in a Python datetime object.
               
           Returns:
               Time:  Date and time in a SolTrack time object.
        
        """
        
        time = Time()  # Create a SolTrack Time object
        
        utc         = dtObj.astimezone(tz.utc)  # Convert from local time to UTC
        
        time.year   = utc.year
        time.month  = utc.month
        time.day    = utc.day
        
        time.hour   = utc.hour
        time.minute = utc.minute
        time.second = utc.second
        
        return time


@dataclass
class Position:
    """Dataclass containing the position of the Sun and related variables."""
    
    julianDay:           float = 0.0
    tJD:                 float = 0.0
    tJC:                 float = 0.0
    tJC2:                float = 0.0
    
    longitude:           float = 0.0
    distance:            float = 0.0
    
    obliquity:           float = 0.0
    cosObliquity:        float = 0.0
    nutationLon:         float = 0.0
    
    rightAscension:      float = 0.0
    declination:         float = 0.0
    agst:                float = 0.0
    
    altitude:            float = 0.0
    altitudeRefract:     float = 0.0
    azimuthRefract:      float = 0.0
    
    hourAngleRefract:    float = 0.0
    declinationRefract:  float = 0.0


@dataclass
class RiseSet:
    """Dataclass containing rise,transit and set times of the Sun and their azimuths/altitudes."""
    
    riseTime:         float = 0.0
    transitTime:      float = 0.0
    setTime:          float = 0.0
    
    riseAzimuth:      float = 0.0
    transitAltitude:  float = 0.0
    setAzimuth:       float = 0.0
    
    
    
    


def copyDataclass(Class, oldInst):
    """Copy an existing dataclass object by creating a new instance and copying its members.
    
    Note:
      - Simply copying a data-class object copies it's *address*, and doesn't make a *data copy*.
    
    Parameters:
      Class (class):    the (name of the) class.
      oldInst (Class):  the existing object.
    
    Returns:
      (Class):  the new object.
    
    See:
      - https://stackoverflow.com/a/57962958/1386750
    
    """
    
    from dataclasses import fields
    
    # Create a new instance:
    newInst = Class
    
    # Copy all data fields:
    for field in fields(Class):
        setattr(newInst, field.name, getattr(oldInst, field.name))
        
    return newInst
    
