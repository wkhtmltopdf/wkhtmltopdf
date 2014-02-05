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

unix {
    TEMP = $$[QT_INSTALL_LIBS] libQtGui.prl
    PRL  = $$[QT_INSTALL_LIBS] QtGui.framework/QtGui.prl
    include($$join(TEMP, "/"))
    include($$join(PRL, "/"))
}

exists($$QMAKE_LIBDIR_QT/libQtGui.so) {
    DEFINES += QT_SHARED
} else:exists($$QMAKE_LIBDIR_QT/../bin//QtGui4.dll) {
    DEFINES += QT_SHARED
} else:contains(QMAKE_PRL_CONFIG, shared) {
    DEFINES += QT_SHARED
} else {
    DEFINES += QT_STATIC
}

win32-msvc2010 {
} else {
	MOC_DIR = ../../build
	OBJECTS_DIR = ../../build
	UI_DIR = ../../build
}

INCLUDEPATH += ../../include

win32 {
    CONFIG += console
}

QT += webkit network svg

# version related information
VERSION_TEXT=$$cat($$PWD/VERSION)
FULL_VERSION=$$split(VERSION_TEXT, ".")
MAJOR_VERSION=$$member(FULL_VERSION, 0)
MINOR_VERSION=$$member(FULL_VERSION, 1)
PATCH_VERSION=$$member(FULL_VERSION, 2)
BUILD=$$system(git rev-parse HEAD)

DEFINES += MAJOR_VERSION=$$MAJOR_VERSION MINOR_VERSION=$$MINOR_VERSION PATCH_VERSION=$$PATCH_VERSION BUILD=$$BUILD