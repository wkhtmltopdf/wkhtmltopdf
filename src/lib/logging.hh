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

#ifndef __LOGGING_HH__
#define __LOGGING_HH__

#include <QMap>
#include <QString>
#include <dllbegin.inc>
namespace wkhtmltopdf {
namespace settings {

enum LogLevel {
	None,
	Error,
	Warn,
	Info
};

DLL_PUBLIC LogLevel strToLogLevel(const char * s, bool * ok=0);
DLL_PUBLIC QString logLevelToStr(const LogLevel & l, bool * ok=0);

}
}
#include <dllend.inc>
#endif //__LOGGING_HH__
