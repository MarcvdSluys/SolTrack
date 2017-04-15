##  CompilerFlags_C.cmake
##  Compiler flags for C compilers
##  Currently, specific flags for gcc and icc are provided
##  Make sure to choose the correct ~last line (printing of the compiler options)
##  
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


# Get compiler name:
get_filename_component( C_COMPILER_NAME ${CMAKE_C_COMPILER} NAME )

# Are we on Linux?
if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
  set( LINUX TRUE )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )


######################################################################################################################################################
#  Specific options per compiler:
######################################################################################################################################################
if( C_COMPILER_NAME MATCHES "gcc" )
  
  # Get compiler version:
  exec_program(
    ${CMAKE_C_COMPILER}
    ARGS --version
    OUTPUT_VARIABLE _compiler_output)
  string(REGEX REPLACE ".* ([0-9]\\.[0-9]\\.[0-9]).*" "\\1"
    COMPILER_VERSION ${_compiler_output})
  
  
  set( CMAKE_C_FLAGS_ALL "-std=c99 -pedantic" )
  if( COMPILER_VERSION VERSION_GREATER "4.4.99" )
    set( CMAKE_C_FLAGS_ALL "${CMAKE_C_FLAGS_ALL} -fwhole-file" )  # >= v.4.5
  endif( COMPILER_VERSION VERSION_GREATER "4.4.99" )
  
  set( CMAKE_C_FLAGS "-pipe -funroll-all-loops" )
  set( CMAKE_C_FLAGS_RELEASE "-pipe -funroll-all-loops" )
  set( CMAKE_C_FLAGS_DEBUG "-g -fsignaling-nans" )
  set( CMAKE_C_FLAGS_PROFILE "-g -gp" )
  
  
  if(WANT_32BIT )
    set( BIT_FLAGS "-m32" )
  endif(WANT_32BIT )
  if(WANT_64BIT )
    set( BIT_FLAGS "-m64" )
  endif(WANT_64BIT )
  
  if(WANT_SSE42 )
    set( SSE_FLAGS "-msse4.2" )
  endif(WANT_SSE42 )
  
  if( WANT_OPENMP )
    set( OPENMP_FLAGS "-fopenmp" )
  endif( WANT_OPENMP )
  
  if( WANT_STATIC )
    set( STATIC_FLAGS "-static" )
  endif( WANT_STATIC )
  
  if( WANT_CHECKS )
    set( CHECK_FLAGS "-fsignaling-nans" )
    if( COMPILER_VERSION VERSION_GREATER "4.4.99" )
      set( CHECK_FLAGS "-fcheck=all ${CHECK_FLAGS}" )    # >= v.4.5
    else( COMPILER_VERSION VERSION_GREATER "4.4.99" )
      set( CHECK_FLAGS "-fbounds-check ${CHECK_FLAGS}" ) # <= v.4.4
    endif( COMPILER_VERSION VERSION_GREATER "4.4.99" )

    set( OPT_FLAGS "-O0" )
  else( WANT_CHECKS )
    set( OPT_FLAGS "-O2" )
  endif( WANT_CHECKS )
  
  if( WANT_WARNINGS )
    set( WARN_FLAGS "-Wall -Wextra -Wno-conversion" )
  endif( WANT_WARNINGS )
  if( STOP_ON_WARNING )
    set( WARN_FLAGS "${WARN_FLAGS} -Werror" )
  endif( STOP_ON_WARNING )
  
  if( WANT_LIBRARY )
    set( LIB_FLAGS "-fPIC -g" )
  endif( WANT_LIBRARY )
  
  
  # Package-specific flags:
  set( PACKAGE_FLAGS "" )
  
  
  ####################################################################################################################################################
elseif( C_COMPILER_NAME MATCHES "icc" )
  
  # Get compiler version:
  exec_program(
    ${CMAKE_C_COMPILER}
    ARGS --version
    OUTPUT_VARIABLE _compiler_output)
  string(REGEX REPLACE ".* ([0-9]*\\.[0-9]*\\.[0-9]*) .*" "\\1"
    COMPILER_VERSION ${_compiler_output})
  
  
  set( CMAKE_C_FLAGS_ALL "-heap-arrays" )
  set( CMAKE_C_FLAGS "-vec-guard-write -funroll-loops -align -ip" )
  if( LINUX )
    set( CMAKE_C_FLAGS_ALL "${CMAKE_C_FLAGS_ALL} -mcmodel=large" )  # -mcmodel exists for Linux only...
  endif( LINUX )
  set( CMAKE_C_FLAGS_RELEASE "-vec-guard-write -funroll-loops -align -ip" )
  set( CMAKE_C_FLAGS_DEBUG "-g -traceback" )
  set( CMAKE_C_FLAGS_PROFILE "-g -gp" )
  
  # !!! HOST_OPT overrules SSE42, as icc would !!!
  if(WANT_SSE42 )
    set( SSE_FLAGS "-axSSE4.2,SSSE3" )
  endif(WANT_SSE42 )
  if(WANT_HOST_OPT)
    set (SSE_FLAGS "-xHost")
  endif(WANT_HOST_OPT)
  
  if(WANT_IPO )
    set( IPO_FLAGS "-ipo" )
  endif(WANT_IPO )
  
  if( WANT_OPENMP )
    set( OPENMP_FLAGS "-openmp -openmp-report0" )
    message( STATUS "Linking with OpenMP support" )
  endif( WANT_OPENMP )
  
  if( WANT_STATIC )
    set( STATIC_FLAGS "-static" )
  endif( WANT_STATIC )
  
  if( WANT_CHECKS )
    set( CHECK_FLAGS "-ftrapuv -check all -check noarg_temp_created -check nostack -traceback" )  #  -check stack doesn't link in icc 2013.3.174 14.0.3
    set( OPT_FLAGS "-O0" )
  else( WANT_CHECKS )
    set( OPT_FLAGS "-O2" )
  endif( WANT_CHECKS )
  
  if( WANT_WARNINGS )
    set( WARN_FLAGS "-Wall -std=c99" )
  endif( WANT_WARNINGS )
  
  if( STOP_ON_WARNING )
    # set( WARN_FLAGS "${WARN_FLAGS}" )  # No option found yet - remove 'unused' warning from CMake this way
  endif( STOP_ON_WARNING )
  
  if( WANT_LIBRARY )
    set( LIB_FLAGS "-fPIC -g" )
  endif( WANT_LIBRARY )
  
  
  # Package-specific flags:
  set( PACKAGE_FLAGS "" )
  
  
  ####################################################################################################################################################
else( C_COMPILER_NAME MATCHES "gcc" )
  
  
  message( "CMAKE_C_COMPILER full path: " ${CMAKE_C_COMPILER} )
  message( "C compiler: " ${C_COMPILER_NAME} )
  message( "No optimized C compiler flags are known, we just try -O2..." )
  set( CMAKE_C_FLAGS "" )
  set( CMAKE_C_FLAGS_RELEASE "" )
  set( CMAKE_C_FLAGS_DEBUG "-g" )
  set( OPT_FLAGS "-O2" )
  
  
  # Package-specific flags:
  set( PACKAGE_FLAGS "" )
  
  
endif( C_COMPILER_NAME MATCHES "gcc" )
######################################################################################################################################################





######################################################################################################################################################
#  Put everything together:
######################################################################################################################################################

set( USER_FLAGS "${OPT_FLAGS} ${LIB_FLAGS} ${CHECK_FLAGS} ${WARN_FLAGS} ${BIT_FLAGS} ${SSE_FLAGS} ${IPO_FLAGS} ${OPENMP_FLAGS} ${STATIC_FLAGS} ${INCLUDE_FLAGS} ${PACKAGE_FLAGS}" )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS_ALL} ${CMAKE_C_FLAGS} ${USER_FLAGS}" )
set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_ALL} ${CMAKE_C_FLAGS_RELEASE} ${USER_FLAGS}" )
set( CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_DEBUG}" )  # These are added to the RELEASE flags when releasing with debug info




######################################################################################################################################################
#  Report what's going on:
######################################################################################################################################################

message( STATUS "" )
message( STATUS "Using C compiler:  ${C_COMPILER_NAME} ${COMPILER_VERSION}  (${CMAKE_C_COMPILER})" )

if( WANT_CHECKS )
  message( STATUS "Compiling with run-time checks:  ${CHECK_FLAGS}" )
endif( WANT_CHECKS )
if( WANT_WARNINGS )
  message( STATUS "Compiling with warnings:  ${WARN_FLAGS}" )
endif( WANT_WARNINGS )
if( WANT_LIBRARY )
  message( STATUS "Compiling with library options:  ${LIB_FLAGS}" )
endif( WANT_LIBRARY )
if( WANT_STATIC )
  message( STATUS "Linking statically:  ${STATIC_FLAGS}" )
endif( WANT_STATIC )


## Choose the one which is actually used: see CMAKE_BUILD_TYPE in CMakeLists.txt:
##   (Typically, RELEASE for executable code and RELEASE + RELWITHDEBINFO for libraries)
message( STATUS "Compiler flags used:  ${CMAKE_C_FLAGS_RELEASE} ${CMAKE_C_FLAGS_RELWITHDEBINFO}" )
message( STATUS "" )



