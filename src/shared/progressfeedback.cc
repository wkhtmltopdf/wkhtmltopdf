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

#include "progressfeedback.hh"
#include <cstdio>
namespace wkhtmltopdf {
/*!
  \file progressfeedback.hh
  \brief Define the ProgressFeedback class
*/

/*!
  \brief Write out a warning message
  \param message The warning message
*/
void ProgressFeedback::warning(const QString &message) {
	if (quiet) return;
    qDebug() << "Warning: " << message;
}

/*!
  \brief Write out an error message
  \param message The error message
*/
void ProgressFeedback::error(const QString &message) {
    qDebug() << "Error: " << message;
}

/*!
  \brief Write out the name of the next phase
*/
void ProgressFeedback::phaseChanged() {
	if (quiet) return;
    QString desc=converter.phaseDescription();
    qDebug() << desc;
	if (converter.currentPhase() < converter.phaseCount() -1)
        qDebug() << QString("(%1/%2)").arg(converter.currentPhase()+1).arg(converter.phaseCount()-1);
}

/*!
  \brief Update progress bar
*/
void ProgressFeedback::progressChanged(int progress) {
	if (quiet) return;
    QString s = "[";
	int w=60;
	progress *= w;
	progress /= 100;
	for (int i=0; i < w; ++i) {
        if (i < progress) s.append("=");
        else if (i == progress) s.append(">");
        else s.append(" ");
	}
    s.append("]");
    s.append(converter.progressString());
    qDebug() << s;
}

ProgressFeedback::ProgressFeedback(bool q, Converter & _):
    quiet(q), converter(_) {
    connect(&converter, SIGNAL(warning(const QString &)), this, SLOT(warning(const QString &)));
	connect(&converter, SIGNAL(error(const QString &)), this, SLOT(error(const QString &)));
	connect(&converter, SIGNAL(phaseChanged()), this, SLOT(phaseChanged()));
	connect(&converter, SIGNAL(progressChanged(int)), this, SLOT(progressChanged(int)));
}

}
