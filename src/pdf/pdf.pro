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
TARGET = wkhtmltopdf
DESTDIR = ../../bin
DEPENDPATH += . ../shared
INCLUDEPATH += . ../shared

unix {
    man.path=$$INSTALLBASE/share/man/man1
    man.extra=LD_LIBRARY_PATH=../../bin/ ../../bin/wkhtmltopdf --manpage | gzip > $(INSTALL_ROOT)$$INSTALLBASE/share/man/man1/wkhtmltopdf.1.gz

    QMAKE_EXTRA_TARGETS += man
    INSTALLS += man
}

macx {
    CONFIG -= app_bundle
}

INSTALLS += target
target.path=$$INSTALLBASE/bin

include(../shared/shared.pri)

CONFIG(shared, shared|static) {
  LIBS += -L../../bin -lwkhtmltox
} else {
  include(../lib/lib.pri)
}

#Application part
SOURCES += wkhtmltopdf.cc pdfarguments.cc pdfcommandlineparser.cc \
           pdfdocparts.cc
