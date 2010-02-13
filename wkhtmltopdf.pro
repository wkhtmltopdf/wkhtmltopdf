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

DEFINES += MAJOR_VERSION=0 MINOR_VERSION=9 PATCH_VERSION=2 BUILD=""

TEMP = $$[QT_INSTALL_LIBS] libQtGui.prl
PRL  = $$[QT_INSTALL_LIBS] QtGui.framework/QtGui.prl
include($$join(TEMP, "/"))
include($$join(PRL, "/"))

contains(QMAKE_PRL_CONFIG, shared) {
    DEFINES += QT_SHARED
} else {
    DEFINES += QT_STATIC
    QTPLUGIN += qjpeg qgif qtiff qmng
}

TEMPLATE = app
TARGET = 
DEPENDPATH += . src
INCLUDEPATH += . src

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

readme.target=README
readme.commands=./wkhtmltopdf --readme > README
readme.depends=wkhtmltopdf

QMAKE_EXTRA_UNIX_TARGETS += readme

unix {
    man.target=wkhtmltopdf.1.gz
    man.commands=./wkhtmltopdf --manpage | gzip > $@
    man.depends=wkhtmltopdf

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
HEADERS += src/pageconverter_p.hh src/pageconverter.hh \
           src/multipageloader_p.hh src/multipageloader.hh src/progressfeedback.hh

SOURCES += src/wkhtmltopdf.cc src/arguments.cc src/commandlineparser.cc \
           src/docparts.cc src/outputter.cc src/manoutputter.cc src/settings.cc \
           src/htmloutputter.cc src/textoutputter.cc src/tempfile.cc \
           src/multipageloader.cc src/pageconverter.cc src/progressfeedback.cc \
           src/outline.cc src/tocprinter.cc
