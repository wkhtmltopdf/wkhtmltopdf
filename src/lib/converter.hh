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

#ifndef __CONVERTER_HH__
#define __CONVERTER_HH__

#include <QObject>
#include <loadsettings.hh>
#include <dllbegin.inc>

namespace wkhtmltopdf {

class DLL_LOCAL ConverterPrivate;

class DLL_PUBLIC Converter: public QObject {
    Q_OBJECT
public:
	virtual ~Converter() {};

    int currentPhase();
    int phaseCount();
    QString phaseDescription(int phase=-1);
    QString progressString();
    int httpErrorCode();
signals:
    void warning(const QString & message);
    void error(const QString & message);
    void phaseChanged();
    void progressChanged(int progress);
    void finished(bool ok);

	void checkboxSvgChanged(const QString & path);
	void checkboxCheckedSvgChanged(const QString & path);
	void radiobuttonSvgChanged(const QString & path);
	void radiobuttonCheckedSvgChanged(const QString & path);
public slots:
    void beginConversion();
	bool convert();
	void cancel();
protected:
	void emitCheckboxSvgs(const settings::LoadPage & ls);
	virtual ConverterPrivate & priv() = 0;
	friend class ConverterPrivate;
};

}
#include <dllend.inc>
#endif //__CONVERTER_HH__
