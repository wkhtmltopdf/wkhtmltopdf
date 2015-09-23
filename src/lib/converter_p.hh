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

#ifndef __CONVERTER_P_HH__
#define __CONVERTER_P_HH__

#include "converter.hh"
#include "websettings.hh"
#include <QFile>
#include <QWebSettings>

#include "dllbegin.inc"

namespace wkhtmltopdf {

class DLL_LOCAL ConverterPrivate: public QObject {
	Q_OBJECT
public:
	void copyFile(QFile & src, QFile & dst);

	QList<QString> phaseDescriptions;
	int currentPhase;

	QString progressString;
protected:
	bool error;
	virtual void clearResources() = 0;
	virtual Converter & outer() = 0;
	int errorCode;

	bool conversionDone;

	void updateWebSettings(QWebSettings * ws, const settings::Web & s) const;
public slots:
	void fail();
	void loadProgress(int progress);

	virtual void beginConvert() = 0;
	void cancel();
	bool convert();
	void forwardError(QString error);
	void forwardWarning(QString warning);
private:
  friend class Converter;
};

}
#include "dllend.inc"
#endif //__CONVERTER_P_HH__
