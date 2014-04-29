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

#ifndef __PDF_C_BINDINGS_P_HH__
#define __PDF_C_BINDINGS_P_HH__

#include "pdf.h"
#include "pdfconverter.hh"
#include <QObject>
#include <QHash>
#include <vector>

#include "dllbegin.inc"

class DLL_LOCAL MyPdfConverter: public QObject {
    Q_OBJECT
public:
	wkhtmltopdf_str_callback warning_cb;
	wkhtmltopdf_str_callback error_cb;
	wkhtmltopdf_void_callback phase_changed;
	wkhtmltopdf_int_callback progress_changed;
	wkhtmltopdf_int_callback finished_cb;

	wkhtmltopdf::PdfConverter converter;

	wkhtmltopdf::settings::PdfGlobal * globalSettings;
	std::vector<wkhtmltopdf::settings::PdfObject *> objectSettings;
  QHash<QString, QByteArray> utf8StringCache;

	MyPdfConverter(wkhtmltopdf::settings::PdfGlobal * gs);
	~MyPdfConverter();
public slots:
    void warning(const QString & message);
    void error(const QString & message);
    void phaseChanged();
    void progressChanged(int progress);
    void finished(bool ok);
private:
    MyPdfConverter(const MyPdfConverter&);
};

#include "dllend.inc"
#endif //__PDF_C_BINDINGS_P_HH__
