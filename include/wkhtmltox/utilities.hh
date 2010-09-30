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

#ifndef __UTILITIES_HH__
#define __UTILITIES_HH__
#ifdef __WKHTMLTOX_UNDEF_QT_DLL__
#ifdef QT_DLL
#undef QT_DLL
#endif
#endif

#include <QCleanlooksStyle>
#include <QCommonStyle>
#include <QFile>
#include <QFileInfo>
#include <QImageWriter>
#include <QPainter>
#include <QStyleOption>
#include <QSvgRenderer>
#include <QUrl>

#include <wkhtmltox/dllbegin.inc>
/**
 * Custom simplistic style
 */
class DLL_PUBLIC MyLooksStyle: public QCleanlooksStyle {
	Q_OBJECT
public:
	typedef QCleanlooksStyle parent_t;
	MyLooksStyle();
	void drawPrimitive( PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget = 0 ) const;
	bool weAreDrawingForms;

	static QSvgRenderer * checkbox;
	static QSvgRenderer * checkbox_checked;
	static QSvgRenderer * radiobutton;
	static QSvgRenderer * radiobutton_checked;
public slots:
	void producingForms(bool f);
	void setCheckboxSvg(const QString & path);
	void setCheckboxCheckedSvg(const QString & path);
	void setRadioButtonSvg(const QString & path);
	void setRadioButtonCheckedSvg(const QString & path);
};

DLL_PUBLIC int handleError(bool success, int errorCode);

#include <wkhtmltox/dllend.inc>
#endif //__UTILITIES_HH__
