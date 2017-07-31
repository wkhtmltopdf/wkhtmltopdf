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
  \class ProgressFeedback
  \brief Produce progress feedback on the terminal
*/

#define S(t) ((t).toLocal8Bit().constData())

/*!
  \brief Write out a warning message
  \param message The warning message
*/
void ProgressFeedback::warning(const QString &message) {
	if (logLevel < settings::Warn) return;
	fprintf(stderr, "Warning: %s",S(message));
	for (int l = 9 + message.size(); l < lw; ++l)
		fprintf(stderr, " ");
	fprintf(stderr, "\n");
	lw = 0;
	fflush(stderr);
}

/*!
  \brief Write out an error message
  \param message The error message
*/
void ProgressFeedback::error(const QString &message) {
	if (logLevel < settings::Error) return;
	fprintf(stderr, "Error: %s",S(message));
	for (int l = 7 + message.size(); l < lw; ++l)
		fprintf(stderr, " ");
	fprintf(stderr, "\n");
	lw = 0;
	fflush(stderr);
}

/*!
  \brief Write out the name of the next phase
*/
void ProgressFeedback::phaseChanged() {
	if (logLevel < settings::Info) return;
	QString desc=converter.phaseDescription();
	fprintf(stderr, "%s", S(desc));

	int l = desc.size();
	if (converter.currentPhase() < converter.phaseCount() -1)
		l += fprintf(stderr," (%d/%d)",converter.currentPhase()+1,converter.phaseCount()-1);
	for (; l < lw; ++l)
		fprintf(stderr, " ");
	fprintf(stderr, "\n");
	lw = 0;
	fflush(stderr);
}

/*!
  \brief Update progress bar
*/
void ProgressFeedback::progressChanged(int progress) {
	if (logLevel < settings::Info) return;
	fprintf(stderr, "[");
	int w=60;
	progress *= w;
	progress /= 100;
	for (int i=0; i < w; ++i) {
		if (i < progress) fprintf(stderr, "=");
		else if (i == progress) fprintf(stderr, ">");
		else fprintf(stderr, " ");
	}
	fprintf(stderr, "]");
	fprintf(stderr, " %s", S(converter.progressString()));
	int l=1+w+2+converter.progressString().size();
	for (int i=l; i < lw; ++i) fprintf(stderr, " ");
	lw = l;
	fprintf(stderr, "\r");
	fflush(stderr);
}

ProgressFeedback::ProgressFeedback(settings::LogLevel l, Converter & _):
    logLevel(l), converter(_), lw(0) {
    connect(&converter, SIGNAL(warning(const QString &)), this, SLOT(warning(const QString &)));
	connect(&converter, SIGNAL(error(const QString &)), this, SLOT(error(const QString &)));
	connect(&converter, SIGNAL(phaseChanged()), this, SLOT(phaseChanged()));
	connect(&converter, SIGNAL(progressChanged(int)), this, SLOT(progressChanged(int)));
}

}
