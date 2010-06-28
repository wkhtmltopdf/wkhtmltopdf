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

#ifndef __TOCPRINTER_HH__
#define __TOCPRINTER_HH__
#include <QWebFrame>
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
#include "outline.hh"
#include <QPainter>
#include <QPrinter>

namespace wkhtmltopdf {

class TocPrinterPrivate;

class TocPrinter {
public:
	TocPrinter(Outline * outline, QPrinter * printer, QPainter & painter);
	~TocPrinter();
	int pageCount();
	void spoolPage(int page);
	void fillLinks(int doc, QVector<QPair<QWebElement, QString> > & links);
private:
	TocPrinterPrivate * d;
};

}
#endif //__EXTENSIVE_WKHTMLTOPDF_QT_HACK__
#endif //__TOCPRINTER_HH__
