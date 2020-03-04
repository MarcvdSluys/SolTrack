#  Copyright (c) 2019-2020  Marc van der Sluys - marc.vandersluys.nl
#   
#  This file is part of the SolTrack Python package,
#  see: http://soltrack.sf.net
#   
#  This is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#  
#  This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License along with this code.  If not, see
#  <http://www.gnu.org/licenses/>.


"""SolTrack module

SolTrack is a simple, free, fast and accurate Python package to compute the position of the Sun, as well as
its rise and set times.  SolTrack can be used under the conditions of the GPLv3 licence.  These pages contain
the API documentation.  For more information on the Python package, licence, source code and data files, see
the [SolTrack homepage](http://soltrack.sf.net).

"""


name = "soltrack"

from .data import *
from .dataclasses import *
from .soltrack import *
from .riseset import *


