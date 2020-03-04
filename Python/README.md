# SolTrack #

A free, fast and simple Python package to compute the position of the Sun, as well as its rise and set times.
SolTrack was originally written in C/C++ by Marc van der Sluys of the Sustainable energy research group of the
HAN University of Applied Sciences in Arnhem, the Netherlands, and has now been translated to pure Python.

SolTrack performs about 12340 position calculations per second on a single 3.6 GHz core of my 2013 laptop,
with an accuracy of 0.0030 ± 0.0016°.  This makes it about 500x times faster than astropy, but around 50x
slower than pyEphem, which is written in C.  SolTrack has been used to control solar trackers, as well as
modelling in solar energy.


## Installation ##

This package can be installed using `pip install soltrack`.  This should automatically install the dependency
packages `numpy` and `dataclasses` (if you're not using Python 3.7+) if they haven't been installed already.
If you are installing by hand, ensure that these packages are installed as well.


## Example use ##

```python
"""Example Python script to compute the position of the Sun and its rise and set times for a single instant."""

import soltrack as st

# Set preferences (all are False by default):
useDegrees             = True   # Input (geographic position) and output are in degrees
useNorthEqualsZero     = True   # Azimuth: 0 = South, pi/2 (90deg) = West  ->  0 = North, pi/2 (90deg) = East
computeRefrEquatorial  = True   # Compure refraction-corrected equatorial coordinates (Hour angle, declination)
computeDistance        = True   # Compute the distance to the Sun

# Set up geographical location (in degrees, since useDegrees=True) in a SolTrack Location dataclass object:
loc = st.Location(5.950270, 51.987380)  # longitude (>0: east of Greenwich),  latitude (>0: northern hemisphere)

# Set (UT!) date and time in a SolTrack Time dataclass object:
time = st.Time(2045, 7, 16,  6, 2, 49.217348)  # Date: 2045-07-16, time: 06:02:49.217348


# Compute positions - returns a st.Position object:
pos = st.computeSunPosition(loc, time, useDegrees, useNorthEqualsZero, computeRefrEquatorial, computeDistance)

# Compute rise and set times - returns a st.RiseSet object:
riseSet = st.computeSunRiseSet(loc, time, 0.0, useDegrees, useNorthEqualsZero)


# Write results to standard output:
print("Location:  %0.3lf E, %0.3lf N" % (loc.longitude, loc.latitude))
print("Date:      %4d %2d %2d" % (time.year, time.month, time.day))
print("Time:      %2d %2d %9.6lf" % (time.hour, time.minute, time.second))
print("JD:        %0.11lf" % (pos.julianDay))
print()

print("Ecliptic longitude, latitude:        %10.6lf° %10.6lf°" % (pos.longitude, 0.0))
print("Right ascension, declination:        %10.6lf° %10.6lf°" % (pos.rightAscension, pos.declination))
print("Uncorrected altitude:                            %10.6lf°" % (pos.altitude))
print("Corrected azimuth, altitude:         %10.6lf° %10.6lf°" % (pos.azimuthRefract, pos.altitudeRefract))
print("Corrected hour angle, declination:   %10.6lf° %10.6lf°" % (pos.hourAngleRefract, pos.declinationRefract))
print()

print("Rise time:      %11.5lf,    azimuth:   %11.5lf" % (riseSet.riseTime, riseSet.riseAzimuth))
print("Transit time:   %11.5lf,    altitude:  %11.5lf" % (riseSet.transitTime, riseSet.transitAltitude))
print("Set time:       %11.5lf,    azimuth:   %11.5lf" % (riseSet.setTime, riseSet.setAzimuth))
print()
```


## SolTrack pages ##

* [SourceForge](http://soltrack.sf.net): SolTrack homepage, data files
* [Pypi](https://pypi.org/project/soltrack/): SolTrack Python package
* [GitHub](https://github.com/MarcvdSluys/SolTrack): SolTrack source code


## Author and licence ##

* Author: Marc van der Sluys
* Contact: http://han.vandersluys.nl/en/
* Licence: [GPLv3+](https://www.gnu.org/licenses/gpl.html)


## References ##

* Meeus, [Astronomical algorithms](https://www.willbell.com/math/MC1.HTM), 2nd Ed.
* This C and Python codes are adapted from the Fortran implementation in
  [libTheSky](http://libthesky.sourceforge.net/), which contains many references.
* [Celestial mechanics in a nutshell (CMiaNS)](https://cmians.sourceforge.io/)
