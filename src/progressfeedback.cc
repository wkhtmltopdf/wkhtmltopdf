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
#include "progressfeedback.hh"

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
	if (pageConverter.settings().quiet) return;
	fprintf(stderr, "Warning: %s",S(message));
	for(int l = 9 + message.size(); l < lw; ++l) 
		fprintf(stderr, " ");
	fprintf(stderr, "\n");
	lw = 0;
}

/*!
  \brief Write out an error message
  \param message The error message
*/
void ProgressFeedback::error(const QString &message) {
	fprintf(stderr, "Error: %s",S(message));
	for(int l = 7 + message.size(); l < lw; ++l) 
		fprintf(stderr, " ");
	fprintf(stderr, "\n");
	lw = 0;
}

/*!
  \brief Write out the name of the next phase
*/
void ProgressFeedback::phaseChanged() {
	if (pageConverter.settings().quiet) return;
	QString desc=pageConverter.phaseDescription();
	fprintf(stderr, "%s", S(desc));
	
	int l = desc.size();
	if(pageConverter.currentPhase() < pageConverter.phaseCount() -1)
		l += fprintf(stderr," (%d/%d)",pageConverter.currentPhase()+1,pageConverter.phaseCount()-1);
	for(; l < lw; ++l) 
		fprintf(stderr, " ");
	fprintf(stderr, "\n");
	lw = 0;
}

/*!
  \brief Update progress bar
*/
void ProgressFeedback::progressChanged(int progress) {
	if (pageConverter.settings().quiet) return;
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
	fprintf(stderr, " %s", S(pageConverter.progressString()));
	int l=1+w+2+pageConverter.progressString().size();
	for(int i=l; i < lw; ++i) fprintf(stderr, " ");
	lw = l;
	fprintf(stderr, "\r");
}

ProgressFeedback::ProgressFeedback(PageConverter & _):
    pageConverter(_), lw(0) {
    connect(&pageConverter, SIGNAL(warning(const QString &)), this, SLOT(warning(const QString &)));
	connect(&pageConverter, SIGNAL(error(const QString &)), this, SLOT(error(const QString &)));
	connect(&pageConverter, SIGNAL(phaseChanged()), this, SLOT(phaseChanged()));
	connect(&pageConverter, SIGNAL(progressChanged(int)), this, SLOT(progressChanged(int)));
}

