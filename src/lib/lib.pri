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

DEFINES += BUILDING_DLL

#Shared
PUBLIC_HEADERS += ../lib/converter.h ../lib/multipageloader.h ../lib/dllbegin.inc
PUBLIC_HEADERS += ../lib/dllend.inc ../lib/loadsettings.h ../lib/websettings.h
PUBLIC_HEADERS += ../lib/utilities.h
HEADERS += ../lib/multipageloader_p.h  ../lib/converter_p.h
SOURCES += ../lib/loadsettings.cpp ../lib/multipageloader.cpp ../lib/tempfile.cpp \
           ../lib/converter.cpp ../lib/websettings.cpp  \
           ../lib/reflect.cpp ../lib/utilities.cpp

#Pdf
PUBLIC_HEADERS += ../lib/pdfconverter.h ../lib/pdfsettings.h
HEADERS += ../lib/pdfconverter_p.h
SOURCES += ../lib/pdfsettings.cpp ../lib/pdfconverter.cpp \
           ../lib/outline.cpp ../lib/tocstylesheet.cpp

PUBLIC_HEADERS += ../lib/imageconverter.h ../lib/imagesettings.h
HEADERS += ../lib/imageconverter_p.h
SOURCES += ../lib/imagesettings.cpp ../lib/imageconverter.cpp

#C-Bindings
PUBLIC_HEADERS += ../lib/pdf.h ../lib/image.h
HEADERS += ../lib/pdf_c_bindings_p.h ../lib/image_c_bindings_p.h
SOURCES += ../lib/pdf_c_bindings.cpp ../lib/image_c_bindings.cpp


HEADERS += $$PUBLIC_HEADERS
