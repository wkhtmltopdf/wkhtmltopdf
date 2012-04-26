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

#ifndef __PDFCONVERTER_HH__
#define __PDFCONVERTER_HH__
#ifdef __WKHTMLTOX_UNDEF_QT_DLL__
#ifdef QT_DLL
#undef QT_DLL
#endif
#endif

#include <wkhtmltox/converter.hh>
#include <wkhtmltox/pdfsettings.hh>

#include <wkhtmltox/dllbegin.inc>
namespace wkhtmltopdf {

DLL_PUBLIC void dumpDefaultTOCStyleSheet(QTextStream & stream, settings::TableOfContent & s);

class DLL_LOCAL PdfConverterPrivate;

class DLL_PUBLIC PdfConverter: public Converter {
	Q_OBJECT
public:
	PdfConverter(settings::PdfGlobal & globalSettings);
	~PdfConverter();
	int pageCount();
	void addResource(const settings::PdfObject & pageSettings, const QString * data=0);
	const settings::PdfGlobal & globalSettings() const;
	const QByteArray & output();
    static const qreal millimeterToPointMultiplier = 2.83464567;
private:
	PdfConverterPrivate * d;
	virtual ConverterPrivate & priv();
	friend class PdfConverterPrivate;
signals:
	void producingForms(bool);
};

}
#include <wkhtmltox/dllend.inc>
#endif //__PDFCONVERTER_HH__
