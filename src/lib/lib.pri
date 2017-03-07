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
PUBLIC_HEADERS += ../lib/converter.hh ../lib/multipageloader.hh ../lib/dllbegin.inc
PUBLIC_HEADERS += ../lib/dllend.inc ../lib/loadsettings.hh ../lib/websettings.hh
PUBLIC_HEADERS += ../lib/utilities.hh
HEADERS += ../lib/multipageloader_p.hh  ../lib/converter_p.hh
SOURCES += ../lib/loadsettings.cc ../lib/logging.cc ../lib/multipageloader.cc \
	   ../lib/tempfile.cc ../lib/converter.cc ../lib/websettings.cc  \
  	   ../lib/reflect.cc ../lib/utilities.cc

#Pdf
PUBLIC_HEADERS += ../lib/pdfconverter.hh ../lib/pdfsettings.hh
HEADERS += ../lib/pdfconverter_p.hh
SOURCES += ../lib/pdfsettings.cc ../lib/pdfconverter.cc \
           ../lib/outline.cc ../lib/tocstylesheet.cc

PUBLIC_HEADERS += ../lib/imageconverter.hh ../lib/imagesettings.hh
HEADERS += ../lib/imageconverter_p.hh
SOURCES += ../lib/imagesettings.cc ../lib/imageconverter.cc

#C-Bindings
PUBLIC_HEADERS += ../lib/pdf.h ../lib/image.h
HEADERS += ../lib/pdf_c_bindings_p.hh ../lib/image_c_bindings_p.hh
SOURCES += ../lib/pdf_c_bindings.cc ../lib/image_c_bindings.cc


HEADERS += $$PUBLIC_HEADERS
