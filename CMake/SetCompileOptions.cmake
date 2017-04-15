## SetCompileOptions.cmake: 
##  Various compile/optimisation options that we may want to enable
##  
##  Copyright 2017 Marc van der Sluys, http://han.vandersluys.nl
##  Lectorate of Sustainable Energy, HAN University of applied sciences, Arnhem, The Netherlands
##  
##  This file is part of the SolTrack package, see: http://soltrack.sourceforge.net
##  SolTrack is derived from libTheSky (http://libthesky.sourceforge.net) under the terms of the GPL v.3
##  
##  This is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published
##  by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
##  
##  This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
##  
##  You should have received a copy of the GNU Lesser General Public License along with this code.  If not, see 
##  <http://www.gnu.org/licenses/>.


option( CMAKE_VERBOSE_MAKEFILE  "Verbose makefile" off )
option( CREATE_VERSION          "Create a code-version file with git version, hash and date (for developers)" off )

option( WANT_CHECKS             "Activate runtime checks (array bounds, NaNs)" off )
option( WANT_WARNINGS           "Activate warnings" on )
option( STOP_ON_WARNING         "Stop compilation on warnings" off )

option( WANT_32BIT              "Force the generation of 32-bit code" off )
option( WANT_64BIT              "Force the generation of 64-bit code" off )
option( WANT_PACKAGE            "Create a linux-distribution binary package" off )

option( WANT_OPENMP             "Use OpenMP parallelisation (experimental)" off )
option( WANT_SSE42              "Enable generation of SSE4.2 code" off )
option( WANT_HOST_OPT           "Enable host-specific optimisation. Choose only when compiling and running on the same machine! Overrides WANT_SSE42" off )
option( WANT_IPO                "Inter-procedural optimisation" off )
option( WANT_STATIC             "Generate statically linked executable" off )

option( WANT_LIBRARY            "Use compiler options to create libraries" on )
option( CREATE_SHAREDLIB        "Create shared libraries" on )
option( CREATE_STATICLIB        "Create static libraries" on )
option( COMPILER_SPECIFIC_LIBS  "Create compiler-specific libraries" off )

