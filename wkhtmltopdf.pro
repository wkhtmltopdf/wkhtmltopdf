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

DEFINES += MAJOR_VERSION=0 MINOR_VERSION=7 PATCH_VERSION=2

TEMP = $$[QT_INSTALL_LIBS] libQtGui.prl
include($$join(TEMP, "/"))

contains(QMAKE_PRL_CONFIG, shared) {
    DEFINES += QT_SHARED
} else {
    DEFINES += QT_STATIC
    QTPLUGIN += qjpeg qgif qtiff qmng
}

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

unix {
    man.target=wkhtmltopdf.1.gz
    man.commands=m4 wkhtmltopdf.man.m4 | gzip > $@
    man.depends=wkhtmltopdf wkhtmltopdf.man.m4 
    
    manins.target=manins
    manins.depends=man
    manins.files=wkhtmltopdf.1.gz
    manins.path=$$INSTALLBASE/share/man/man1

    QMAKE_EXTRA_UNIX_TARGETS += manins man
    INSTALLS += manins
}

win32 {
    CONFIG += console
}

INSTALLS += target
target.path=$$INSTALLBASE/bin

QT += webkit network

# Input
HEADERS += wkhtmltopdf.hh toc.hh
SOURCES += wkhtmltopdf.cc toc.cc arguments.cc
