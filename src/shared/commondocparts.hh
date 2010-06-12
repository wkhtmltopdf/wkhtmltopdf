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
#ifndef __COMMONDOCPARTS_HH__
#define __COMMONDOCPARTS_HH__
#include "outputter.hh"

namespace commonDocParts {
void outputName(Outputter * o, QString app);
void outputLicense(Outputter * o);
void outputAuthors(Outputter * o);
void outputStaticProblems(Outputter * o);
void outputProxyDoc(Outputter * o);
};

#endif //__COMMONDOCPARTS_HH__
