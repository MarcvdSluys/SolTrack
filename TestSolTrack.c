/*
  SolTrack: a simple, free, fast and accurate C routine to compute the position of the Sun
  
  Test program for SolTrack:
  1) rename random_dates_100.dat to random_dates.dat
  2) run TestSolTrack on the 100 random dates between 2014 and 2113 in random_dates.dat (in UT!)
  3) the output in SolTrack_positions.dat should be identical to SolTrack_positions_100.dat, barring round-off errors
  
  
  Copyright (c) 2014  Marc van der Sluys, Paul van Kan and Jurgen Reintjes,
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
  
  // Set date and time manually (not used when reading from random_dates.dat)
  time.year = 2014;
  time.month = 6;
  time.day = 21;
  time.hour = 13 - 2;
  time.minute = 41;
  time.second = 46;
  
  
  struct Location loc;
  loc.longitude =  5.950270/R2D;  // HAN University of applied sciences, Arnhem, The Netherlands
  loc.latitude  = 51.987380/R2D;
  
  
  FILE *inFile;
  if ( (inFile=fopen("random_dates.dat","r")) == NULL ) {
    printf("Error opening file random_dates.dat\n");
    exit(1);
  }
  
  FILE *outFile;
  if ( (outFile=fopen("SolTrack_positions.dat","w")) == NULL ) {
    printf("Error opening file SolTrack_positions.dat\n");
    exit(1);
  }
  
  double jd=0.;
  struct Position pos;
  
  int status = 1;
  
  // Read the input file:
  while( fscanf(inFile, "%d %d %d %d %d %lf  %lf\n", 
		&time.year, &time.month, &time.day, 
		&time.hour, &time.minute, &time.second, &jd) != EOF) {
    
    // Compute positions:
    SolTrack(time, loc, &pos, computeEquatorial);
    
    // Write the positions to file:
    fprintf(outFile, "%4d %2d %2d   %2d %2d %9.6lf  %20.11lf   %10.6lf %10.6lf   %10.6lf %10.6lf   %10.6lf %10.6lf %10.6lf   %10.6lf %10.6lf\n", 
	   time.year, time.month, time.day, 
	    (int)time.hour, (int)time.minute, time.second, pos.julianDay, 
	    pos.longitude*R2D, 0.0,  
	    fmod(pos.rightAscension + MPI, TWO_PI)*R2D/15.0, pos.declination*R2D,  
	    pos.azimuthRefract,pos.altitude*R2D,pos.altitudeRefract,
	    pos.hourAngleRefract, pos.declinationRefract
	    );
    
  }
  
  status = status;
  return 0;
}

