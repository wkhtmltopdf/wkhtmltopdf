//-*- mode: c++; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup"; -*-
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
#ifndef __OUTLINE_HH__
#define __OUTLINE_HH__
#include <QWebFrame>
#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
#include <QWebElement>
#include "settings.hh"

class OutlinePrivate;

class Outline {
public:
	Outline(const Settings & settings);
	~Outline();
	void addWebPage(const QString & name, QWebPrinter & wp, QWebFrame * frame);
	void fillHeaderFooterParms(int page, QHash<QString, QString> & parms);
	void fillAnchors(int d, QHash<QString, QWebElement> & anchors);
	int pageCount();
	void printOutline(QPrinter * printer);
	void dumpOutline();
private:
	OutlinePrivate * d;
	friend class TocPrinter;
	friend class TocPrinterPrivate;
};

#endif //__EXTENSIVE_WKHTMLTOPDF_QT_HACK__
#endif //__OUTLINE_HH__
