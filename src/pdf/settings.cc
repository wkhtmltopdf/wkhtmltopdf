// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010 wkhtmltopdf authors
//
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

#include <QMap>
#include <settings.hh>
#include <stdexcept>
namespace wkhtmltopdf {
namespace settings {

/*!
  \file settings.hh
  \brief Defines the Settings class
*/
QMap<QString, QPrinter::PageSize> pageSizeMap() {
	QMap<QString, QPrinter::PageSize> res;
	res["A0"] = QPrinter::A0;
	res["A1"] = QPrinter::A1;
	res["A2"] = QPrinter::A2;
	res["A3"] = QPrinter::A3;
	res["A4"] = QPrinter::A4;
	res["A5"] = QPrinter::A5;
	res["A6"] = QPrinter::A6;
	res["A7"] = QPrinter::A7;
	res["A8"] = QPrinter::A8;
	res["A9"] = QPrinter::A9;
	res["B0"] = QPrinter::B0;
	res["B1"] = QPrinter::B1;
	res["B10"] = QPrinter::B10;
	res["B2"] = QPrinter::B2;
	res["B3"] = QPrinter::B3;
	res["B4"] = QPrinter::B4;
	res["B5"] = QPrinter::B5;
	res["B6"] = QPrinter::B6;
	res["B7"] = QPrinter::B7;
	res["B8"] = QPrinter::B8;
	res["B9"] = QPrinter::B9;
	res["C5E"] = QPrinter::C5E;
	res["Comm10E"] = QPrinter::Comm10E;
	res["DLE"] = QPrinter::DLE;
	res["Executive"] = QPrinter::Executive;
	res["Folio"] = QPrinter::Folio;
	res["Ledger"] = QPrinter::Ledger;
	res["Legal"] = QPrinter::Legal;
	res["Letter"] = QPrinter::Letter;
	res["Tabloid"] = QPrinter::Tabloid;
	return res;
}

/*!
  Convert a string to a paper size, basically all thinkable values are allowed.
  if a unknown value is given A4 is returned
  \param s The string to convert
  \param ok If supplied indicates if the conversion was successful
*/
QPrinter::PageSize strToPageSize(const char * s, bool * ok) {
	QMap<QString,QPrinter::PageSize> map = pageSizeMap();
	for (QMap<QString,QPrinter::PageSize>::const_iterator i=map.begin(); i != map.end(); ++i) {
		if (i.key().compare(s, Qt::CaseInsensitive) != 0) continue;
		if (ok) *ok=true;
		return i.value();
	}
	if (ok) *ok = false;
	return QPrinter::A4;
}

QString pageSizeToStr(QPrinter::PageSize ps) {
	QMap<QString,QPrinter::PageSize> map = pageSizeMap();
	for (QMap<QString,QPrinter::PageSize>::const_iterator i=map.begin(); i != map.end(); ++i) {
		if (i.value() == ps) return i.key();
	}
	return "";
}


/*!
  Read orientation from a string, possible values are landscape and portrait (case insensitive)
  \param s The string containing the orientation
  \param ok If supplied indicates whether the s was valid
*/
QPrinter::Orientation strToOrientation(const char * s, bool * ok) {
	if (ok) *ok = true;
 	if (!strcasecmp(s,"Landscape")) return QPrinter::Landscape;
 	if (!strcasecmp(s,"Portrait")) return QPrinter::Portrait;
	if (ok) *ok = false;
	return QPrinter::Portrait;
}

QString orientationToStr(QPrinter::Orientation o) {
	return (o == QPrinter::Landscape)?"Landscape":"Portrait";
}


/*!
  Parse a string describing a distance, into a real number and a unit.
  \param o Tho string describing the distance
  \param ok If supplied indicates whether the s was valid
*/
UnitReal strToUnitReal(const char * o, bool * ok) {
	qreal s=1.0; //Since not all units are provided by qt, we use this variable to scale
	//Them into units that are.
	QPrinter::Unit u=QPrinter::Millimeter;
	//Skip the real number part
	int i=0;
	while ('0' <= o[i]  && o[i] <= '9') ++i;
	if (o[i] == '.' || o[i] == '.') ++i;
	while ('0' <= o[i]  && o[i] <= '9') ++i;
	//Try to match the unit used
	if (!strcasecmp(o+i,"") || !strcasecmp(o+i,"mm") || !strcasecmp(o+i,"millimeter")) {
		u=QPrinter::Millimeter;
	} else if (!strcasecmp(o+i,"cm") || !strcasecmp(o+i,"centimeter")) {
		u=QPrinter::Millimeter;
		s=10.0; //1cm=10mm
	} else if (!strcasecmp(o+i,"m") || !strcasecmp(o+i,"meter")) {
		u=QPrinter::Millimeter;
		s=1000.0; //1m=1000m
	} else if (!strcasecmp(o+i,"didot"))
		u=QPrinter::Didot; //Todo is there a short for didot??
	else if (!strcasecmp(o+i,"inch") || !strcasecmp(o+i,"in"))
		u=QPrinter::Inch;
	else if (!strcasecmp(o+i,"pica") || !strcasecmp(o+i,"pc"))
		u=QPrinter::Pica;
	else if (!strcasecmp(o+i,"cicero"))
		u=QPrinter::Cicero;
	else if (!strcasecmp(o+i,"pixel") || !strcasecmp(o+i,"px"))
		u=QPrinter::DevicePixel;
	else if (!strcasecmp(o+i,"point") || !strcasecmp(o+i,"pt"))
		u=QPrinter::Point;
	else {
		if (ok) ok=false;
		return UnitReal(QString(o).left(i).toDouble()*s, u);
	}
	return UnitReal(QString(o).left(i).toDouble(ok)*s, u);
}

QString unitRealToStr(const UnitReal & ur, bool * ok) {
	QString c;
	if (ur.first == -1) {
		if (ok) *ok=false;
		return "";
	}
	if (ok) *ok=true;
	switch (ur.second) {
	case QPrinter::Didot: c = "didot"; break;
	case QPrinter::Inch: c = "in"; break;
	case QPrinter::Pica: c = "pica"; break;
	case QPrinter::DevicePixel: c = "px"; break;
	case QPrinter::Point: c = "pt"; break;
	case QPrinter::Millimeter: c = "mm"; break;
	default:
		if (ok) *ok=false; break;
	}
	return QString("%1%2").arg(ur.first).arg(c);
}


Size::Size():
	pageSize(QPrinter::A4),
	height(UnitReal(-1,QPrinter::Millimeter)),
	width(UnitReal(-1,QPrinter::Millimeter)) {}

HeaderFooter::HeaderFooter():
	fontSize(12),
	fontName("Ariel"),
	left(""),
	right(""),
	center(""),
	line(false),
	htmlUrl(""),
	spacing(0.0) {}

Margin::Margin():
	top(UnitReal(10,QPrinter::Millimeter)),
	right(UnitReal(10,QPrinter::Millimeter)),
	bottom(UnitReal(10,QPrinter::Millimeter)),
	left(UnitReal(10,QPrinter::Millimeter)) {}

Global::Global():
	quiet(false),
	useGraphics(false),
	orientation(QPrinter::Portrait),
	colorMode(QPrinter::Color),
	resolution(QPrinter::HighResolution),
	dpi(-1),
	pageOffset(0),
	copies(1),
	collate(true),
	outline(true),
	outlineDepth(4),
	dumpOutline(""),
	out("-"),
	documentTitle(""),
	useCompression(true),
	imageDPI(600),
	imageQuality(94) {};

TableOfContent::TableOfContent():
	useDottedLines(true),
	captionText("Table of Content"),
	forwardLinks(true),
	backLinks(false),
	indentation("1em"),
	fontScale(0.8) {}

Page::Page():
	useExternalLinks(true),
	useLocalLinks(true),
	produceForms(false),
	includeInOutline(true),
	pagesCount(true),
	isTableOfContent(false),
	tocXsl("") {};
}
}
