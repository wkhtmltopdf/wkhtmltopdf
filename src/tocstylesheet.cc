// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
// This file is part of wkhtmltopdf.
//
// wkhtmltopdf is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// wkhtmltopdf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with wkhtmltopdf.  If not, see <http://www.gnu.org/licenses/>.
#include "outline_p.hh"
#include <QTextOStream>

namespace wkhtmltopdf {

void dumbDefaultTOCStyleSheet(QTextStream & stream) {
    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl
		   << "<xsl:stylesheet version=\"1.0\"" << endl
		   << "	        xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\"" << endl
		   << "                xmlns:outline=\"http://code.google.com/p/wkhtmltopdf/outline\"" << endl
		   << "                xmlns=\"http://www.w3.org/1999/xhtml\">" << endl
		   << "  <xsl:template match=\"outline:outline\">" << endl
		   << "    <html>" << endl
		   << "      <head>" << endl
		   << "        <title><xsl:value-of select=\"@title\" /></title>" << endl
		   << "        <style>" << endl
		   << "          h1 {" << endl
		   << "            text-align: center;" << endl
		   << "            font-size: 20px;" << endl
		   << "            font-family: ariel;" << endl
		   << "          }" << endl
		   << "          div {border-bottom: 1px dashed rgb(200,200,200);}" << endl
		   << "          span {float: right;}" << endl
		   << "          li {list-style: none;}" << endl
		   << "          ul {" << endl
		   << "            font-size: 20px;" << endl
		   << "            font-family: ariel;" << endl
		   << "          }" << endl
		   << "          ul ul {font-size: 80%; }" << endl
		   << "          ul {padding-left: 0em;}" << endl
		   << "          ul ul {padding-left: 1em;}" << endl
		   << "          a {text-decoration:none; color: black;}" << endl
		   << "        </style>" << endl
		   << "      </head>" << endl
		   << "      <body>" << endl
		   << "        <h1><xsl:value-of select=\"@title\" /></h1>" << endl
		   << "        <ul><xsl:apply-templates select=\"outline:item\"/></ul>" << endl
		   << "      </body>" << endl
		   << "    </html>" << endl
		   << "  </xsl:template>" << endl
		   << "  <xsl:template match=\"outline:item\">" << endl
		   << "    <li>" << endl
		   << "      <div>" << endl
		   << "        <a>" << endl
		   << "          <xsl:if test=\"@link\">" << endl
		   << "            <xsl:attribute name=\"href\"><xsl:value-of select=\"@link\"/></xsl:attribute>" << endl
		   << "          </xsl:if>" << endl
		   << "          <xsl:if test=\"@backLink\">" << endl
		   << "            <xsl:attribute name=\"name\"><xsl:value-of select=\"@backLink\"/></xsl:attribute>" << endl
		   << "          </xsl:if>" << endl
		   << "          <xsl:value-of select=\"@title\" /> " << endl
		   << "        </a>" << endl
		   << "        <span> <xsl:value-of select=\"@page\" /> </span>" << endl
		   << "      </div>" << endl
		   << "      <ul>" << endl
		   << "        <xsl:apply-templates select=\"outline:item\"/>" << endl
		   << "      </ul>" << endl
		   << "    </li>" << endl
		   << "  </xsl:template>" << endl
		   << "</xsl:stylesheet>" << endl;
}

}

