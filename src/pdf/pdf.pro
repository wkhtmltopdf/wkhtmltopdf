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

include(../../version.pri)
include(../../common.pri)

TEMPLATE = app
DESTDIR = ../../bin
TARGET = wkhtmltopdf
DEPENDPATH += . ../shared
INCLUDEPATH += . ../shared

readme.target=README
readme.commands=./wkhtmltopdf --readme > README
readme.depends=wkhtmltopdf

QMAKE_EXTRA_TARGETS += readme

unix {
    man.target=wkhtmltopdf.1.gz
    man.commands=./wkhtmltopdf --manpage | gzip > $@
    man.depends=wkhtmltopdf

    manins.target=manins
    manins.depends=man
    manins.files=wkhtmltopdf.1.gz
    manins.path=$$INSTALLBASE/share/man/man1

    QMAKE_EXTRA_TARGETS += manins man
    INSTALLS += manins
}

INSTALLS += target
target.path=$$INSTALLBASE/bin

include(../shared/shared.pri)

contains(DEFINES, QT_SHARED) {
  LIBS += -L../../bin -lwkhtmltox
} else {
  include(../lib/lib.pri)
}

#Application part
SOURCES += wkhtmltopdf.cc pdfarguments.cc pdfcommandlineparser.cc \
           pdfdocparts.cc
