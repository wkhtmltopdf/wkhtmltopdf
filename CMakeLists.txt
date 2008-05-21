# This file is part of wkhtmltopdf.
#
# wkhtmltopdf is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# wkhtmltopdf is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with wkhtmltopdf.  If not, see <http://www.gnu.org/licenses/>.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(wkhtmltopdf)
SET( CMAKE_COLOR_MAKEFILE ON )
SET( CMAKE_VERBOSE_MAKEFILE ON )
SET( CMAKE_INCLUDE_CURRENT_DIR TRUE )
ADD_DEFINITIONS( -Wall -ansi)
SET( QT_USE_QTWEBKIT TRUE )
SET( QT_USE_QTNETWORK TRUE )
FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR} )
QT4_WRAP_CPP(HAT wkhtmltopdf.hh)
ADD_EXECUTABLE(wkhtmltopdf wkhtmltopdf.cc ${HAT})
TARGET_LINK_LIBRARIES( wkhtmltopdf ${QT_LIBRARIES} )
INSTALL(TARGETS wkhtmltopdf DESTINATION bin)
