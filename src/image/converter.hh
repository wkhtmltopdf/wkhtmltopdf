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
#include <QObject>
#include <QWebPage>
#include "settings.hh"

class Converter: public QObject {
	Q_OBJECT
public:
	Converter(wkhtmltopdf::settings::Global & s);
	~Converter();
	int httpErrorCode;
	wkhtmltopdf::settings::Global settings;
	bool convert();
public slots:
	void loaded(bool ok);
};

