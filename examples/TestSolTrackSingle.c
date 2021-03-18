/*
  SolTrack: a simple, free, fast and accurate C routine to compute the position of the Sun
  
  Test program for SolTrack, using a hard-coded date, time and location.  Output to screen.
  
  
  Copyright (c) 2014-2017  Marc van der Sluys, Paul van Kan and Jurgen Reintjes,
  Lectorate of Sustainable Energy, HAN University of applied sciences, Arnhem, The Netherlands
   
  This file is part of the SolTrack package, see: http://soltrack.sourceforge.net
  SolTrack is derived from libTheSky (http://libthesky.sourceforge.net) under the terms of the GPL v.3
  
  This is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
  
  This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public License along with this code.  If not, see 
  <http://www.gnu.org/licenses/>.
*/


#include "SolTrack.h"
#include "stdlib.h"

int main() {
  int useDegrees = 1;             // Input (geographic position) and output are in degrees
  int useNorthEqualsZero = 1;     // Azimuth: 0 = South, pi/2 (90deg) = West  ->  0 = North, pi/2 (90deg) = East
  int computeRefrEquatorial = 1;  // Compure refraction-corrected equatorial coordinates (Hour angle, declination): 0-no, 1-yes
  int computeDistance = 1;        // Compute the distance to the Sun in AU: 0-no, 1-yes
  
  
  struct Time time;
  
  // Set (UT!) date and time manually - use the first date from SolTrack_positions.dat:
  time.year = 2045;
  time.month = 7;
  time.day = 16;
  time.hour = 8 - 2;  // 8h CEST = 6h UT
  time.minute = 2;
  time.second = 49.217348;
  
  struct Location loc;
  loc.longitude =  5.950270;  // HAN University of applied sciences, Arnhem, The Netherlands
  loc.latitude  = 51.987380;
  loc.pressure = 101.0;     // Atmospheric pressure in kPa
  loc.temperature = 283.0;  // Atmospheric temperature in K
  
  
  // Compute rise and set times:
  struct Position pos;
  struct RiseSet riseSet;
  SolTrack_RiseSet(time, loc, &pos, &riseSet, 0.0, useDegrees, useNorthEqualsZero);
  
  
  // Compute positions:
  SolTrack(time, loc, &pos, useDegrees, useNorthEqualsZero, computeRefrEquatorial, computeDistance);
  
  // Write data to screen:
  printf("Date:   %4d %2d %2d\n", time.year, time.month, time.day);
  printf("Time:   %2d %2d %9.6lf\n", (int)time.hour, (int)time.minute, time.second);
  printf("JD:     %20.11lf\n\n", pos.julianDay);
  printf("tJD:    %20.11lf\n", pos.tJD);
  printf("GMST:   %20.11lf\n", fmod(pos.agst - pos.nutationLon * pos.cosObliquity, 6.28318530717958647693)/3.14159265358979323846*12.0);
  printf("AGST:   %20.11lf\n", fmod(pos.agst, 6.28318530717958647693)/3.14159265358979323846*12.0);
  
  printf("Rise time:      %11.5lf,    azimuth:   %11.5lf\n", riseSet.riseTime, riseSet.riseAzimuth);
  printf("Transit time:   %11.5lf,    altitude:  %11.5lf\n", riseSet.transitTime, riseSet.transitAltitude);
  printf("Set time:       %11.5lf,    azimuth:   %11.5lf\n\n", riseSet.setTime, riseSet.setAzimuth);
  
  printf("Ecliptic longitude, latitude:        %10.6lf° %10.6lf°\n", pos.longitude, 0.0);
  printf("Right ascension, declination:        %10.6lf° %10.6lf°\n", pos.rightAscension, pos.declination);
  printf("Uncorrected altitude:                            %10.6lf°\n\n", pos.altitude);
  printf("Corrected azimuth, altitude:         %10.6lf° %10.6lf°\n", pos.azimuthRefract, pos.altitudeRefract);
  printf("Corected hour angle, declination:    %10.6lf° %10.6lf°\n\n", pos.hourAngleRefract, pos.declinationRefract);
  
  
  
  return 0;
}

