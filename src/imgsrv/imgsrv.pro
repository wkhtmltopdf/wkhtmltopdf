# Copyright 2010, 2011 wkhtmltopdf authors
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

include(../../common.pri)

TEMPLATE = app
TARGET = imgsrv
DESTDIR = ../../bin
DEPENDPATH += . ../shared
INCLUDEPATH += . ../shared ../image
CONFIG += debug

INSTALLS += target
target.path=$$INSTALLBASE/bin

include(../shared/shared.pri)

contains(DEFINES, QT_SHARED) {
  LIBS += -L../../bin -lwkhtmltox
} else {
  include(../lib/lib.pri)
}

HEADERS += srv.h
SOURCES += main.cpp srv.cpp
