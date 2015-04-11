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


#include "SolTrack.h"


/**
 * @brief   Main function to compute the position of the Sun
 *
 * @param [in] time                Struct containing date and time to compute the position for, in UT
 * @param [in] location            Struct containing the geographic location to compute the position for
 * @param [out] position           Struct containing the position of the Sun in horizontal (and equatorial if desired) coordinates
 * @param [in]  computeEquatorial  Compute equatorial coordinates yes (1) or no (0)
 *
 * Example Usage:
 * @code SolTrack(time, location, &position, 1); @endcode
 */

void SolTrack(struct Time time, struct Location location, struct Position *position, int computeEquatorial) {
  
  // Compute the Julian Day from the date and time:
  position->julianDay = computeJulianDay(time.year, time.month, time.day,  time.hour, time.minute, time.second);
  
  
  // Derived expressions of time:
  position->dJD  = position->julianDay - 2451545.0;                 // Time in days since 2000.0
  position->tJC  = position->dJD/36525.0;                           // Time in Julian centuries since 2000.0
  position->tJC2 = position->tJC*position->tJC;                     // T^2
  position->tJC3 = position->tJC2*position->tJC;                    // T^3
  
  
  // Compute the ecliptic longitude of the Sun and the obliquity of the ecliptic:
  computeLongitude(position);
  
  // Convert ecliptic coordinates to geocentric equatorial coordinates:
  convertEclipticToEquatorial(position->longitude, position->obliquity,  &position->rightAscension, &position->declination);
  
  // Convert equatorial coordinates to horizontal coordinates, correcting for parallax and refraction:
  convertEquatorialToHorizontal(location, position);
  
  // Convert the corrected horizontal coordinates back to equatorial coordinates:
  if(computeEquatorial) convertHorizontalToEquatorial(location.latitude, position->azimuthRefract, position->altitudeRefract,  &position->hourAngleRefract, &position->declinationRefract);
  
  // Use the North=0 convention for azimuth and hour angle (default: South = 0):
  useNorthEqualsZero(&position->azimuthRefract, &position->hourAngleRefract, computeEquatorial);
  
  // Convert final results from radians to degrees:
  convertRadiansToDegrees(&position->azimuthRefract, &position->altitudeRefract,  &position->hourAngleRefract, &position->declinationRefract, computeEquatorial);
}





/**
 * @brief  Compute the Julian Day from the date and time
 * 
 * Gregorian calendar only (>~1582).
 *
 * @param [in]  year    Year of date
 * @param [in]  month   Month of date
 * @param [in]  day     Day of date
 * @param [in]  hour    Hour of time
 * @param [in]  minute  Minute of time
 * @param [in]  second  Second of time
 *
 * @retval JulianDay    Julian day for the given date and time
 */

double computeJulianDay(int year, int month, int day,  int hour, int minute, double second) {
  if(month <= 2) {
    year -= 1;
    month += 12;
  }
  
  int tmp1 = (int)floor(year/100.0);
  int tmp2 = 2 - tmp1 + (int)floor(tmp1/4.0);
  
  double dDay = day + hour/24.0 + minute/1440.0 + second/86400.0;
  double JD = floor(365.250*(year+4716)) + floor(30.60010*(month+1)) + dDay + tmp2 - 1524.5;
  
  return JD;
}



/**
 * @brief  Compute the ecliptic longitude and distance of the Sun for a given Julian Day
 * 
 * Also computes the obliquity of the ecliptic and nutation.
 *
 * @param [inout]  position  Position of the Sun
 */

void computeLongitude(struct Position *position) {
  
  double l0 = 4.895063168 + 628.331966786 * position->tJC  +  5.291838e-6  * position->tJC2;  // Mean longitude
  double m  = 6.240060141 + 628.301955152 * position->tJC  -  2.682571e-6  * position->tJC2;  // Mean anomaly
  double e  = 0.016708634 - 0.000042037   * position->tJC  -  0.0000001267 * position->tJC2;  // Eccentricity of the Earth's orbit
    
  
  double c = (3.34161088e-2 - 8.40725e-5* position->tJC - 2.443e-7*position->tJC2)*sin(m)  +  (3.489437e-4 - 1.76278e-6*position->tJC)*sin(2*m) + 5.044e-6*sin(3*m);  // Sun's equation of the centre
  double odot = l0 + c;                                   // True longitude - rev
  double nu = m + c;                                      // True anomaly - rev
  double dist = 1.000001018*(1.0-e*e)/(1.0 + e*cos(nu));  // Geocentric distance of the Sun in AU
  
  
  // Nutation, aberration:
  double omg  = 2.1824390725 - 33.7570464271 * position->tJC  + 3.622256e-5 * position->tJC2 + 3.7337958e-8 * position->tJC3 - 2.879321e-10 * position->tJC2*position->tJC2;  // Lon. of Moon's mean ascending node
  double lm   = 3.8103417 + 8399.709113*position->tJC;                                                                                // Mean longitude of the Moon
  double dpsi = -8.338795e-5*sin(omg) - 6.39954e-6*sin(2*l0) - 1.115e-6*sin(2*lm) + 1.018e-6*sin(2*omg);                    // Nutation in longitude
  double aber = -9.93087e-5/dist;                                                                                           // Aberration
  position->longitude  = fmod(odot + aber + dpsi + MPI, TWO_PI);                                                            // Apparent geocentric longitude, referred to the true equinox of date
  position->distance   = dist;                                                                                              // Geocentric distance to the Sun, in AU
  
  
  // Obliquity of the ecliptic and nutation - do this here, since we've already computed many of the ingredients:
  double eps0 = 0.409092804222 - 2.26965525e-4*position->tJC - 2.86e-9*position->tJC2 + 8.78967e-9*position->tJC3;                      // Mean obliquity of the ecliptic
  double deps = 4.46e-5*cos(omg) + 2.76e-6*cos(2*l0) + 4.848e-7*cos(2*lm) - 4.36e-7*cos(2*omg);           // Nutation in obliquity
  position->obliquity   = eps0 + deps;                                                                    // True obliquity of the ecliptic
  position->nutationLon = dpsi;                                                                           // Nutation in longitude
}


/**
 * @brief  Convert ecliptic coordinates to equatorial coordinates
 * 
 * This function assumes that the ecliptic latitude = 0.
 *
 * @param [in] longitude  Ecliptic longitude of the Sun (rad)
 * @param [in] obliquity  Obliquity of the ecliptic (rad)
 * 
 * @param [out] rightAscension  Right ascension of the Sun (rad)
 * @param [out] declination     Declination of the Sun (rad)
 */

void convertEclipticToEquatorial(double longitude, double obliquity,  double *rightAscension, double *declination) {
  *rightAscension   = atan2(cos(obliquity)*sin(longitude),cos(longitude));
  *declination      = asin(sin(obliquity)*sin(longitude));
}



/**
 * @brief  Convert equatorial to horizontal coordinates
 * 
 * Also corrects for parallax and atmospheric refraction.
 *
 * @param [in]    location  Geographic location of the observer (rad)
 * @param [inout] position  Position of the Sun (rad)
 */

void convertEquatorialToHorizontal(struct Location location, struct Position *position) {
  double gmst   = 4.89496121273579229 + 6.3003880989849575*position->dJD + 6.77070812713916e-6*position->tJC2 - 4.5087296615715e-10*position->tJC2*position->tJC;  // Greenwich mean siderial time
  double agst   = gmst + position->nutationLon * cos(position->obliquity);                                         // Correction for equation of the equinoxes -> apparent Greenwich siderial time
  
  double alt = 0.0;
  // Azimuth does not need to be corrected for parallax or refraction, hence store the result in the 'azimuthRefract' variable directly:
  eq2horiz(location.latitude, location.longitude,  position->rightAscension, position->declination, agst,  &position->azimuthRefract, &alt);
  
  // Correct for parallax:
  alt -= asin(EARTH_RADIUS/(position->distance*AU)) * cos(alt);  // Subtract the parallax from the altitude
  position->altitude = alt;
  
  // Correct for atmospheric refraction:
  alt += 2.9670597e-4/tan(alt + 3.137559e-3/(alt + 8.91863e-2));  // Add the refraction correction to the altitude
  
  position->altitudeRefract = alt;
}



/**
 * @brief  Convert equatorial coordinates to horizontal coordinates
 * 
 * @param [in]  latitude        Geographic latitude of the observer (rad)
 * @param [in]  longitude       Geographic longitude of the observer (rad)
 * @param [in]  rightAscension  Right ascension of the Sun (rad)
 * @param [in]  declination     Declination of the Sun (rad)
 * @param [in]  agst            Apparent Greenwich siderial time (Greenwich mean siderial time, corrected for the equation of the equinoxes)
 * @param [out] azimuth         Azimuth ("wind direction") of the Sun (rad; 0=South)
 * @param [out] altitude        Altitude of the Sun above the horizon (rad)
 */

void eq2horiz(double latitude, double longitude,  double rightAscension, double declination, double agst, double *azimuth, double *altitude) {
  
  double hh  = agst + longitude - rightAscension;   // Local Hour Angle
  
  // Some preparation, saves ~29%:
  double sinhh  = sin(hh);
  double coshh  = cos(hh);
  
  double sindec = sin(declination);
  double cosdec = sqrt(1.0-sindec*sindec);    // Cosine of a declination is always positive
  double tandec = sindec/cosdec;
  
  double sinlat = sin(latitude);
  double coslat = sqrt(1.0-sinlat*sinlat);    // Cosine of a latitude is always positive
  
  *azimuth  = fmod( atan2( sinhh,    coshh  * sinlat - tandec * coslat ) + MPI, TWO_PI);  // 0 < azimuth < 2pi
  *altitude = asin( sinlat * sindec + coslat * cosdec * coshh );
}



/**
 * @brief  Convert horizontal coordinates to equatorial coordinates
 * 
 * @param [in]  latitude     Geographic latitude of the observer (rad)
 * @param [in]  azimuth      Azimuth ("wind direction") of the Sun (rad; 0=South)
 * @param [in]  altitude     Altitude of the Sun above the horizon (rad)
 * @param [out] hourAngle    Hour angle of the Sun (rad; 0=South)
 * @param [out] declination  Declination of the Sun (rad)
 */

void convertHorizontalToEquatorial(double latitude, double azimuth, double altitude,  double *hourAngle, double *declination) {
  
  // Multiply used variables:
  double cosaz  = cos(azimuth);  // Add pi to set South = 0;
  double sinaz  = sin(azimuth);  // Add pi to set South = 0;
  
  double sinalt = sin(altitude);
  double cosalt = sqrt(1.0-sinalt*sinalt);    // Cosine of an altitude is always positive
  double tanalt = sinalt/cosalt;
  
  double sinlat = sin(latitude);
  double coslat = sqrt(1.0-sinlat*sinlat);    // Cosine of a latitude is always positive
  
  *hourAngle   = atan2( sinaz ,  cosaz  * sinlat + tanalt * coslat );      // Local Hour Angle
  *hourAngle   = fmod(*hourAngle + MPI, TWO_PI);                           // 0 < Hour Angle < 2-pi
  *declination = asin(  sinlat * sinalt  -  coslat * cosalt * cosaz  );    // Declination
}



/**
 * @brief  Convert the South=0 convention to North=0 convention for azimuth and hour angle
 * 
 * South=0 is the default in celestial astronomy.  This makes the angles compatible with the compass/wind directions.
 * 
 * @param [inout] azimuth            Azimuth ("wind direction") of the Sun (rad)
 * @param [inout] hourAngle          Hour angle of the Sun (rad)
 * @param [in]    computeEquatorial  Compute equatorial coordinates
 */

void useNorthEqualsZero(double *azimuth, double *hourAngle, int computeEquatorial) {
  *azimuth   = fmod(*azimuth   + PI  + MPI, TWO_PI);                        // Add PI to set North=0 and ensure 0 < azimuth < 2pi
  if(computeEquatorial) *hourAngle = fmod(*hourAngle + PI  + MPI, TWO_PI);  // Add PI to set North=0 and ensure 0 < hour angle < 2pi
}



/**
 * @brief  Convert final results from radians to degrees
 * 
 * Not touching intermediate results.
 * 
 * @param [inout] azimuth            Azimuth ("wind direction") of the Sun (rad->deg)
 * @param [inout] altitude           Altitude of the Sun above the horizon (rad->deg)
 * @param [inout] hourAngle          Hour angle of the Sun (rad->deg)
 * @param [inout] declination        Declination of the Sun (rad->deg)
 * @param [in]    computeEquatorial  Compute equatorial coordinates
 */

void convertRadiansToDegrees(double *azimuth, double *altitude,  double *hourAngle, double *declination, int computeEquatorial) {
  *azimuth *= R2D;
  *altitude *= R2D;
  if(computeEquatorial) {
    *hourAngle *= R2D;
    *declination *= R2D;
  }
}


