# Copyright 2010 wkhtmltopdf authors
#
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
    QTPLUGIN += qjpeg qgif qtiff qmng
}

MOC_DIR = ../../build
OBJECTS_DIR = ../../build
UI_DIR = ../../build

win32 {
    CONFIG += console
}

QT += webkit network xmlpatterns
