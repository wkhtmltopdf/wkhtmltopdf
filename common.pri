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

QT4_PRL = libQtGui.prl  QtGui.prl  QtGui.framework/QtGui.prl
QT5_PRL = libQt5Gui.prl Qt5Gui.prl

for(prl, QT4_PRL):exists($$[QT_INSTALL_LIBS]/$${prl}) {
    include($$[QT_INSTALL_LIBS]/$${prl})
    CONFIG += qt_gui_prl_found
}
for(prl, QT5_PRL):exists($$[QT_INSTALL_LIBS]/$${prl}) {
    include($$[QT_INSTALL_LIBS]/$${prl})
    CONFIG += qt_gui_prl_found
}

qt_gui_prl_found {
    contains(QMAKE_PRL_CONFIG, static): DEFINES += QT_STATIC
    else:                               DEFINES += QT_SHARED
}
contains(QMAKE_PRL_CONFIG, static): QTPLUGIN += qcncodecs qjpcodecs qkrcodecs qtwcodecs

# if we can't determine, assume that it is shared
!qt_gui_prl_found: DEFINES += QT_SHARED

MOC_DIR      = ../../build
OBJECTS_DIR  = ../../build
UI_DIR       = ../../build
INCLUDEPATH += ../../include
RESOURCES    = $$PWD/wkhtmltopdf.qrc

win32:      CONFIG += console
win32-g++*: QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++

QT += webkit network xmlpatterns svg
greaterThan(QT_MAJOR_VERSION, 4): QT += webkitwidgets

# version related information
VERSION_TEXT=$$(WKHTMLTOX_VERSION)
isEmpty(VERSION_TEXT): VERSION_TEXT=$$cat($$PWD/VERSION)
VERSION_LIST=$$split(VERSION_TEXT, "-")
count(VERSION_LIST, 1) {
    VERSION=$$VERSION_TEXT
    FULL_VERSION=$$VERSION
} else {
    VERSION=$$member(VERSION_LIST, 0)
    BUILD=$$member(VERSION_LIST, 1)
    FULL_VERSION=$$VERSION-$$BUILD
}

DEFINES += VERSION=$$VERSION FULL_VERSION=$$FULL_VERSION
