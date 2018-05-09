// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010, 2011 wkhtmltopdf authors
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

#ifndef __PROGRESSFEEDBACK_HH__
#define __PROGRESSFEEDBACK_HH__
#include <converter.hh>
#include <logging.hh>
namespace wkhtmltopdf {

class ProgressFeedback: public QObject {
	Q_OBJECT
private:
	settings::LogLevel logLevel;
	Converter & converter;
	int lw;
public slots:
	void warning(const QString &message);
	void error(const QString &message);
	void phaseChanged();
	void progressChanged(int progress);
public:
	ProgressFeedback(settings::LogLevel logLevel, Converter & _);
};

}
#endif //__PROGRESSFEEDBACK_HH__
