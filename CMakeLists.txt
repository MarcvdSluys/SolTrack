##  CMakeFile for SolTrack
##  MvdS, 2017-03-10
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
##
##
##  To compile, from the directory that contains this file, do:
##    $ mkdir build; cd build
##    $ cmake ..
##    $ make install


cmake_minimum_required( VERSION 2.8 )

# Set build type. Do this *before* we set the project name:
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo Profile."
    FORCE )
endif( NOT CMAKE_BUILD_TYPE )
set( CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}" CACHE INTERNAL "internal" )


# Project name and language:
project( SolTrack C )


# Increase verbosity for debugging:
option( CMAKE_VERBOSE_MAKEFILE "Verbose makefile" on )

# Search in the CMake/ directory for CMake modules:
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake )

# Various compile/optimisation options that we may want to enable:
include( SetCompileOptions )


# Place the products in their directories:
get_filename_component( C_COMPILER_NAME ${CMAKE_C_COMPILER} NAME )
set( INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/usr/include" )
set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/usr/lib${LIB_SUFFIX}" )


# Set source files:
include( FileList )

# Set C-compiler flags:
include( CompilerFlags_C )


# Create libraries:
if( CREATE_SHAREDLIB )
  add_library( "SolTrack_shared"  SHARED  ${SRC_FILES} )
  set_target_properties( SolTrack_shared PROPERTIES OUTPUT_NAME "SolTrack" )  # Sets the file name and soname to SolTrack.so
  set_target_properties( SolTrack_shared PROPERTIES C_INCLUDE_DIRECTORY ${INCLUDE_DIRECTORY} )
endif( CREATE_SHAREDLIB )

# Use add_dependencies below in order to pretend the static lib depends on the shared.  This
#   seems to be necessary to ensure that it is built *after* shared.  If this doesn't
#   happen, multiple threads will be pouring data into the SolTrack_version file simultaneously.

if( CREATE_STATICLIB )
  add_library( "SolTrack_static"  STATIC  ${SRC_FILES} )
  set_target_properties( SolTrack_static PROPERTIES OUTPUT_NAME "SolTrack" )
  set_target_properties( SolTrack_static PROPERTIES C_INCLUDE_DIRECTORY ${INCLUDE_DIRECTORY} )
  add_dependencies( SolTrack_static SolTrack_shared )  # Ensure it is built after the shared lib
endif( CREATE_STATICLIB )


# Find the package version from the VERSION file and stick it into PKG_VERSION:
# The file VERSION is either in the root directory of the package, or in doc/:
set( VERSION_FILE ${CMAKE_SOURCE_DIR}/VERSION )
if( NOT EXISTS ${VERSION_FILE} )
  set( VERSION_FILE ${CMAKE_SOURCE_DIR}/doc/VERSION )
endif( NOT EXISTS ${VERSION_FILE} )

set( PKG_VERSION "Unknown" )   # Indicates that the version number was not found
file(STRINGS ${VERSION_FILE} Lines )
foreach( Line ${Lines} )
  string(REGEX MATCH "Release version:.+" Line ${Line} )  # Returns the matching string
  string(COMPARE NOTEQUAL "${Line}" "" Matches )          # If the string is not empty, we have a match
  if( Matches )
    string(REPLACE "Release version:" "" Line ${Line})    # Remove text
    string(STRIP ${Line} Line)                            # Strip leading and trailing spaces
    set( PKG_VERSION ${Line} )
  endif( Matches )
endforeach()
message( STATUS "SolTrack version reported by the package: ${PKG_VERSION}" )
message( STATUS "Compiling for an ${CMAKE_SYSTEM_PROCESSOR} architecture" )
message( STATUS "" )

# Install to destination:
#   use DIRECTORY rather than TARGETS to allow include/<ccompiler>/
install( DIRECTORY usr/ DESTINATION ${CMAKE_INSTALL_PREFIX} )

# Install header file:
install( FILES SolTrack.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include )

# Install man pages:
install( DIRECTORY man/ DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man )

# Install documents:
if( EXISTS ${CMAKE_SOURCE_DIR}/VERSION )
  install( FILES CHANGELOG LICENCE README VERSION DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/soltrack-${PKG_VERSION} )
else( EXISTS  ${CMAKE_SOURCE_DIR}/VERSION )
  install( FILES doc/CHANGELOG doc/LICENCE doc/README doc/VERSION DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/soltrack-${PKG_VERSION} )
endif( EXISTS  ${CMAKE_SOURCE_DIR}/VERSION )


# Create Linux-distribution binary package:
if( WANT_PACKAGE )
  include( CPack_package )
endif( WANT_PACKAGE )

