/*
  SolTrack: a simple, free, fast and accurate C routine to compute the position of the Sun
  
  Test program for SolTrack:
  1) compile the code, e.g.:  gcc -O2 -lm -o TestSolTrack TestSolTrackFile.c SolTrack.c
  2) run TestSolTrack on the 100 random dates between 2014 and 2113 in random_dates.dat (in UT!)
  3) the output in SolTrack_positions.dat should be identical to SolTrack_positions_compare.dat, barring round-off errors
  
  
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
  int useDegrees = 0;             // Input (geographic position) and output are in degrees
  int useNorthEqualsZero = 1;     // Azimuth: 0 = South, pi/2 (90deg) = West  ->  0 = North, pi/2 (90deg) = East
  int computeRefrEquatorial = 1;  // Compure refraction-corrected equatorial coordinates (Hour angle, declination): 0-no, 1-yes
  int computeDistance = 1;        // Compute the distance to the Sun in AU: 0-no, 1-yes
  
  
  
  struct Time time;
  struct Location loc;
  loc.longitude =  5.950270/R2D;  // HAN University of applied sciences, Arnhem, The Netherlands
  loc.latitude  = 51.987380/R2D;
  loc.pressure    = 101.0;        // Atmospheric pressure in kPa
  loc.temperature = 283.0;        // Atmospheric temperature in K
  
  
  FILE *inFile;
  if ( (inFile=fopen("SolTrack_randomDates.dat","r")) == NULL ) {
    perror("SolTrack_randomDates.dat");
    exit(1);
  }
  
  FILE *outFile;
  if ( (outFile=fopen("SolTrack_positions.dat","w")) == NULL ) {
    perror("SolTrack_positions.dat");
    exit(1);
  }
  
  double jd=0.;
  struct Position pos;
  
  int status = 1;
  
  // Read the input file header (3 lines):
  char tmpLine[1024], *fstat;
  for(int i=0; i<3; i++) {
    fstat = fgets(tmpLine, 1024, inFile);
  }
  fstat = fstat;
  
  // Read the input file:
  while( fscanf(inFile, "%d %d %d %d %d %lf  %lf\n", 
		&time.year, &time.month, &time.day, 
		&time.hour, &time.minute, &time.second, &jd) != EOF) {
    
    // Compute positions:
    SolTrack(time, loc, &pos,  useDegrees, useNorthEqualsZero, computeRefrEquatorial, computeDistance);
    
    // Write the positions to file:
    fprintf(outFile, "%4d %2d %2d   %2d %2d %9.6lf  %20.11lf   %10.6lf %10.6lf   %10.6lf %10.6lf   %10.6lf %10.6lf %10.6lf   %10.6lf %10.6lf\n", 
	   time.year, time.month, time.day, 
	    (int)time.hour, (int)time.minute, time.second, pos.julianDay, 
	    pos.longitude*R2D, 0.0,  
	    fmod(pos.rightAscension + MPI, TWO_PI)*R2D/15.0, pos.declination*R2D,  
	    pos.azimuthRefract*R2D,pos.altitude*R2D,pos.altitudeRefract*R2D,
	    pos.hourAngleRefract*R2D, pos.declinationRefract*R2D
	    );
    
  }
  
  status = status;
  return 0;
}

