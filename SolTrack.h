/*
  SolTrack: a simple, free, fast and accurate C routine to compute the position of the Sun
  
  
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


#ifndef __SOLTRACK_H
#define __SOLTRACK_H


#include "stdio.h"
#include "math.h"


// Constants:
#define PI             3.14159265358979323846    // Pi
#define TWO_PI         6.28318530717958647693    // 2 pi
#define MPI            3.14159265358979323846e6  // One Megapi...
#define R2D            57.2957795130823208768    // Radians to degrees conversion factor
#define EARTH_RADIUS   6.3781370e8               // Earth's radius (cm - GWS84)
#define AU             1.49597870700e13          // Astronomical Unit (cm - 2012 IAU value)



// Structs:

/// @brief Date and time to compute the Sun's position for, in UT
struct Time {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  double second;
};


/// @brief Location to compute the Sun's position for
struct Location {
  double longitude;
  double latitude;
};


/// @brief Position of the Sun
struct Position {
  double julianDay, dJD, tJC,tJC2,tJC3;
  
  double longitude, distance;
  double obliquity, nutationLon;
  
  double rightAscension,declination;
  
  double altitude,altitudeRefract,azimuthRefract;
  
  double hourAngleRefract, declinationRefract;
};


// Functions:
void SolTrack(struct Time time, struct Location location, struct Position *position, int computeEquatorial);
double computeJulianDay(int year, int month, int day,  int hour, int minute, double second);
void computeLongitude(struct Position *position);
void convertEclipticToEquatorial(double longitude, double obliquity,  double *rightAscension, double *declination);
void convertEquatorialToHorizontal(struct Location location, struct Position *position);
void eq2horiz(double latitude, double longitude,  double rightAscension, double declination, double agst, double *azimuth, double *altitude);
void convertHorizontalToEquatorial(double latitude, double azimuth, double altitude,  double *hourAngle, double *declination);
void useNorthEqualsZero(double *azimuth, double *hourAngle, int computeEquatorial);
void convertRadiansToDegrees(double *azimuth, double *altitude,  double *hourAngle, double *declination, int computeEquatorial);

#endif
