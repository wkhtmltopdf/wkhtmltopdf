// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010-2020 wkhtmltopdf authors
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
    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << Qt::endl
           << "<xsl:stylesheet version=\"2.0\"" << Qt::endl
           << "                xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\"" << Qt::endl
           << "                xmlns:outline=\"http://wkhtmltopdf.org/outline\"" << Qt::endl
           << "                xmlns=\"http://www.w3.org/1999/xhtml\">" << Qt::endl
           << "  <xsl:output doctype-public=\"-//W3C//DTD XHTML 1.0 Strict//EN\"" << Qt::endl
           << "              doctype-system=\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"" << Qt::endl
           << "              indent=\"yes\" />" << Qt::endl
           << "  <xsl:template match=\"outline:outline\">" << Qt::endl
           << "    <html>" << Qt::endl
           << "      <head>" << Qt::endl
           << "        <title>" << s.captionText << "</title>" << Qt::endl
           << "        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />" << Qt::endl
           << "        <style>" << Qt::endl
           << "          h1 {" << Qt::endl
           << "            text-align: center;" << Qt::endl
           << "            font-size: 20px;" << Qt::endl
           << "            font-family: arial;" << Qt::endl
           << "          }" << Qt::endl;
	if (s.useDottedLines)
        stream << "          div {border-bottom: 1px dashed rgb(200,200,200);}" << Qt::endl;
    stream << "          span {float: right;}" << Qt::endl
           << "          li {list-style: none;}" << Qt::endl
           << "          ul {" << Qt::endl
           << "            font-size: 20px;" << Qt::endl
           << "            font-family: arial;" << Qt::endl
           << "          }" << Qt::endl
           << "          ul ul {font-size: " << (s.fontScale*100.0) << "%; }" << Qt::endl
           << "          ul {padding-left: 0em;}" << Qt::endl
           << "          ul ul {padding-left: " << s.indentation << ";}" << Qt::endl
           << "          a {text-decoration:none; color: black;}" << Qt::endl
           << "        </style>" << Qt::endl
           << "      </head>" << Qt::endl
           << "      <body>" << Qt::endl
           << "        <h1>" << s.captionText << "</h1>" << Qt::endl
           << "        <ul><xsl:apply-templates select=\"outline:item/outline:item\"/></ul>" << Qt::endl
           << "      </body>" << Qt::endl
           << "    </html>" << Qt::endl
           << "  </xsl:template>" << Qt::endl
           << "  <xsl:template match=\"outline:item\">" << Qt::endl
           << "    <li>" << Qt::endl
           << "      <xsl:if test=\"@title!=''\">" << Qt::endl
           << "        <div>" << Qt::endl
           << "          <a>" << Qt::endl;
	if (s.forwardLinks)
        stream << "            <xsl:if test=\"@link\">" << Qt::endl
               << "              <xsl:attribute name=\"href\"><xsl:value-of select=\"@link\"/></xsl:attribute>" << Qt::endl
               << "            </xsl:if>" << Qt::endl;
    stream << "            <xsl:if test=\"@backLink\">" << Qt::endl
           << "              <xsl:attribute name=\"name\"><xsl:value-of select=\"@backLink\"/></xsl:attribute>" << Qt::endl
           << "            </xsl:if>" << Qt::endl
           << "            <xsl:value-of select=\"@title\" /> " << Qt::endl
           << "          </a>" << Qt::endl
           << "          <span> <xsl:value-of select=\"@page\" /> </span>" << Qt::endl
           << "        </div>" << Qt::endl
           << "      </xsl:if>" << Qt::endl
           << "      <ul>" << Qt::endl
           << "        <xsl:comment>added to prevent self-closing tags in QtXmlPatterns</xsl:comment>" << Qt::endl
           << "        <xsl:apply-templates select=\"outline:item\"/>" << Qt::endl
           << "      </ul>" << Qt::endl
           << "    </li>" << Qt::endl
           << "  </xsl:template>" << Qt::endl
           << "</xsl:stylesheet>" << Qt::endl;
}

}
