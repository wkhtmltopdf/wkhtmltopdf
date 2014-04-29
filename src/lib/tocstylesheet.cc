// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010 wkhtmltopdf authors
//
// This file is part of wkhtmltopdf.
//
// wkhtmltopdf is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// wkhtmltopdf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with wkhtmltopdf.  If not, see <http://www.gnu.org/licenses/>.


#include "outline_p.hh"
#include "pdfsettings.hh"
#include <QTextStream>

namespace wkhtmltopdf {

void dumpDefaultTOCStyleSheet(QTextStream & stream, settings::TableOfContent & s) {
    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl
		   << "<xsl:stylesheet version=\"2.0\"" << endl
		   << "                xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\"" << endl
		   << "                xmlns:outline=\"http://wkhtmltopdf.org/outline\"" << endl
		   << "                xmlns=\"http://www.w3.org/1999/xhtml\">" << endl
		   << "  <xsl:output doctype-public=\"-//W3C//DTD XHTML 1.0 Strict//EN\"" << endl
	       << "              doctype-system=\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"" << endl
		   << "              indent=\"yes\" />" << endl
		   << "  <xsl:template match=\"outline:outline\">" << endl
		   << "    <html>" << endl
		   << "      <head>" << endl
		   << "        <title>" << s.captionText << "</title>" << endl
		   << "        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />" << endl
		   << "        <style>" << endl
		   << "          h1 {" << endl
		   << "            text-align: center;" << endl
		   << "            font-size: 20px;" << endl
		   << "            font-family: arial;" << endl
		   << "          }" << endl;
	if (s.useDottedLines)
		stream << "          div {border-bottom: 1px dashed rgb(200,200,200);}" << endl;
	stream << "          span {float: right;}" << endl
		   << "          li {list-style: none;}" << endl
		   << "          ul {" << endl
		   << "            font-size: 20px;" << endl
		   << "            font-family: arial;" << endl
		   << "          }" << endl
		   << "          ul ul {font-size: " << (s.fontScale*100.0) << "%; }" << endl
		   << "          ul {padding-left: 0em;}" << endl
		   << "          ul ul {padding-left: " << s.indentation << ";}" << endl
		   << "          a {text-decoration:none; color: black;}" << endl
		   << "        </style>" << endl
		   << "      </head>" << endl
		   << "      <body>" << endl
		   << "        <h1>" << s.captionText << "</h1>" << endl
		   << "        <ul><xsl:apply-templates select=\"outline:item/outline:item\"/></ul>" << endl
		   << "      </body>" << endl
		   << "    </html>" << endl
		   << "  </xsl:template>" << endl
		   << "  <xsl:template match=\"outline:item\">" << endl
		   << "    <li>" << endl
		   << "      <xsl:if test=\"@title!=''\">" << endl
		   << "        <div>" << endl
		   << "          <a>" << endl;
	if (s.forwardLinks)
		stream << "            <xsl:if test=\"@link\">" << endl
			   << "              <xsl:attribute name=\"href\"><xsl:value-of select=\"@link\"/></xsl:attribute>" << endl
			   << "            </xsl:if>" << endl;
	stream << "            <xsl:if test=\"@backLink\">" << endl
		   << "              <xsl:attribute name=\"name\"><xsl:value-of select=\"@backLink\"/></xsl:attribute>" << endl
		   << "            </xsl:if>" << endl
		   << "            <xsl:value-of select=\"@title\" /> " << endl
		   << "          </a>" << endl
		   << "          <span> <xsl:value-of select=\"@page\" /> </span>" << endl
		   << "        </div>" << endl
		   << "      </xsl:if>" << endl
		   << "      <ul>" << endl
		   << "        <xsl:comment>added to prevent self-closing tags in QtXmlPatterns</xsl:comment>" << endl
		   << "        <xsl:apply-templates select=\"outline:item\"/>" << endl
		   << "      </ul>" << endl
		   << "    </li>" << endl
		   << "  </xsl:template>" << endl
		   << "</xsl:stylesheet>" << endl;
}

}
