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

#ifndef __OUTLINE_HH__
#define __OUTLINE_HH__

#include "pdfsettings.hh"
#include <QWebElement>
#include <QWebFrame>

#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
#include "dllbegin.inc"
namespace wkhtmltopdf {

class DLL_LOCAL OutlinePrivate;

class DLL_LOCAL Outline {
public:
	Outline(const settings::PdfGlobal & settings);
	~Outline();
	void addEmptyWebPage();
	bool replaceWebPage(int d, const QString & name, QWebPrinter & wp, QWebFrame * f, const settings::PdfObject & ps, QVector<QPair<QWebElement, QString> > & local, QHash<QString, QWebElement> & anchors);
	void addWebPage(const QString & name, QWebPrinter & wp, QWebFrame * frame, const settings::PdfObject & ps, QVector<QPair<QWebElement, QString> > & local, QHash<QString, QWebElement> & external);

	void fillHeaderFooterParms(int page, QHash<QString, QString> & parms, const settings::PdfObject & ps);
	void fillAnchors(int d, QHash<QString, QWebElement> & anchors);
	int pageCount();
	void printOutline(QPrinter * printer);

	void dump(QTextStream & stream) const;
private:
	OutlinePrivate * d;
	friend class TocPrinter;
	friend class TocPrinterPrivate;
};

#include "dllend.inc"
}
#endif //__EXTENSIVE_WKHTMLTOPDF_QT_HACK__
#endif //__OUTLINE_HH__
