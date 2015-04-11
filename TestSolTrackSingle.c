/*
  SolTrack: a simple, free, fast and accurate C routine to compute the position of the Sun
  
  Test program for SolTrack, using a hard-coded date, time and location.  Output to screen.
  
  
  Copyright (c) 2014-2015  Marc van der Sluys, Paul van Kan and Jurgen Reintjes,
  Lectorate of Sustainable Energy, HAN University of applied sciences, Arnhem, The Netherlands
   
  This file is part of the SolTrack package, see: http://soltrack.sourceforge.net
  SolTrack is derived from libTheSky (http://libthesky.sourceforge.net) under the terms of the GPL v.3
  
  This is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published
  by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
  
  This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License along with this code.  If not, see 
  <http://www.gnu.org/licenses/>.
*/


#include "SolTrack.h"
#include "stdlib.h"

int main() {
  int computeEquatorial = 1;  // Compure refraction-corrected equatorial coordinates (Hour angle, declination): 0-no, 1-yes
  
  
  struct Time time;
  
  // Set (UT!) date and time manually:
  time.year = 2014;
  time.month = 5;
  time.day = 12;
  time.hour = 10 - 2;  // 10h CEST = 8h UT
  time.minute = 41;
  time.second = 46.123456;
  
  
  struct Location loc;
  loc.longitude =  5.950270/R2D;  // HAN University of applied sciences, Arnhem, The Netherlands
  loc.latitude  = 51.987380/R2D;
  
  
  struct Position pos;
  
  // Compute positions:
  SolTrack(time, loc, &pos, computeEquatorial);
    
  // Write data to screen:
  printf("Date:   %4d %2d %2d\n", time.year, time.month, time.day);
  printf("Time:   %2d %2d %9.6lf\n", (int)time.hour, (int)time.minute, time.second);
  printf("JD:     %20.11lf\n\n", pos.julianDay);
  
  printf("Ecliptic longitude, latitude:        %10.6lf° %10.6lf°\n", pos.longitude*R2D, 0.0);
  printf("Right ascension, declination:        %10.6lf° %10.6lf°\n", fmod(pos.rightAscension + MPI, TWO_PI)*R2D, pos.declination*R2D);
  printf("Uncorrected altitude:                            %10.6lf°\n\n", pos.altitude*R2D);
  printf("Corrected azimuth, altitude:         %10.6lf° %10.6lf°\n", pos.azimuthRefract,pos.altitudeRefract);
  printf("Corected hour angle, declination:    %10.6lf° %10.6lf°\n\n", pos.hourAngleRefract, pos.declinationRefract);
  
  
  
  return 0;
}

