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
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "4")

SET( CMAKE_COLOR_MAKEFILE ON )
SET( CMAKE_VERBOSE_MAKEFILE ON )
SET( CMAKE_INCLUDE_CURRENT_DIR TRUE )
ADD_DEFINITIONS( -Wall -ansi -DMAJOR_VERSION=${CPACK_PACKAGE_VERSION_MAJOR} -DMINOR_VERSION=${CPACK_PACKAGE_VERSION_MINOR})
SET( QT_USE_QTWEBKIT TRUE )
SET( QT_USE_QTNETWORK TRUE )
FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR} )
QT4_WRAP_CPP(HAT wkhtmltopdf.hh)
ADD_EXECUTABLE(wkhtmltopdf wkhtmltopdf.cc ${HAT})
TARGET_LINK_LIBRARIES( wkhtmltopdf ${QT_LIBRARIES} )
INSTALL(TARGETS wkhtmltopdf DESTINATION bin)

SET(CPACK_PACKAGE_NAME wkhtmltopdf)
SET(CPACK_SYSTEM_NAME linux-amd64)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simple shell utility to convert html to pdf using the webkit rendering engine, and qt.")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jakob Truelsen (http://www.daimi.au.dk/~jakobt/")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
#objdump -p wkhtmltopdf | grep NEEDED | sed -re 's/NEEDED//' | xargs dpkg-query -S | sed -re 's/:.*//' | sort -u
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-webkit, libqt4-network, libqt4-gui")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

SET(CPACK_GENERATOR "DEB")
execute_process(COMMAND dpkg --print-architecture OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

SET(CPACK_STRIP_FILES "bin/wkhtmltopdf")
SET(CPACK_SOURCE_STRIP_FILES "")

SET(CPACK_PACKAGE_EXECUTABLES "wkhtmltopdf" "wkhtmltopdf")
INCLUDE(CPack)