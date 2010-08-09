# Copyright 2010 wkhtmltopdf authors
#
# This file is part of wkhtmltopdf.
#
# wkhtmltopdf is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# wkhtmltopdf is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with wkhtmltopdf.  If not, see <http:#www.gnu.org/licenses/>.

TEMPLATE = lib
CONFIG += dll

include(../../version.pri)
include(../../common.pri)
include(lib.pri)

DEFINES += __WKHTMLTOX_UNDEF_QT_DLL__

unix {
   headers.targit=headers
   headers.files=../../include/wkhtmltox/*
   headers.path=$$INSTALLBASE/include/wkhtmltox

   QMAKE_EXTRA_TARGETS += headers
   INSTALLS += headers
}

VERSION=$${MAJOR_VERSION}.$${MINOR_VERSION}.$${PATCH_VERSION}

TARGET=wkhtmltox
INSTALLS += target
wkhtmltox.path=$$INSTALLBASE/lib

DESTDIR = ../../bin
target.path=$$INSTALLBASE/lib
