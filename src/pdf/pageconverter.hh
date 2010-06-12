// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
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
#ifndef __PAGECONVERTER_HH__
#define __PAGECONVERTER_HH__
#include "settings.hh"
#include <QObject>
namespace wkhtmltopdf {

void dumpDefaultTOCStyleSheet(QTextStream & stream, settings::TableOfContent & s);

class PageConverterPrivate;
class PageConverter: public QObject {
	Q_OBJECT
public:
	PageConverter(settings::Global & globalSettings);
	~PageConverter();
	int phaseCount();
	int currentPhase();
	QString phaseDescription(int phase=-1);
	QString progressString();
	int httpErrorCode();
	void addResource(const settings::Page & pageSettings);
	const settings::Global & globalSettings() const;
signals:
	void warning(const QString & message);
	void error(const QString & message);
	void phaseChanged();
	void progressChanged(int progress);
	void finished(bool ok);
public slots:
	void beginConvertion();
	bool convert();
	void cancel();
private:
	PageConverterPrivate * d;
	friend class PageConverterPrivate;
};

}
#endif //__PAGECONVERTER_HH__
