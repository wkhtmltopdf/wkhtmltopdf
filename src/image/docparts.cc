// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010 wkhtmltopdf authors
//
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

#include "commandlineparser.hh"
#include "outputter.hh"
#include <QWebFrame>

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

/*!
  Output name and a short description
  \param o The outputter to output to
*/
void CommandLineParser::outputManName(Outputter * o) const {
	o->beginSection("Name");
	o->paragraph("wkhtmltoimage - html to image converter");
	o->endSection();
}

/*!
  Output a short synopsis on how to call the command line program
  \param o The outputter to output to
*/
void CommandLineParser::outputSynopsis(Outputter * o) const {
	o->beginSection("Synopsis");
	o->verbatim("wkhtmltoimage [OPTIONS]... <input file> <output file>\n");
	o->endSection();
}

/*!
  Explain what the program does
  \param o The outputter to output to
*/
void CommandLineParser::outputDescripton(Outputter * o) const {
	o->beginSection("Description");
	o->beginParagraph();
	o->text("Converts an HTML page into an image, ");
	o->endParagraph();
	o->endSection();
}


/*!
  Output contact information
  \param o The outputter to output to
*/
void CommandLineParser::outputContact(Outputter * o) const {
	o->beginSection("Contact");
	o->beginParagraph();
	o->text("If you experience bugs or want to request new features please visit ");
	o->link("http://code.google.com/p/wkhtmltopdf/issues/list");
	o->text(", if you have any problems or comments please feel free to contact me: ");
	o->link("uuf6429@gmail.com");
	o->endParagraph();
	o->endSection();
}

/*!
  Output beginning of the readme
  \param o The outputter to output to
*/
void CommandLineParser::outputDocStart(Outputter * o) const {
	o->beginSection(QString("wkhtmltoimage ")+QString::number(MAJOR_VERSION)+"."+QString::number(MINOR_VERSION)+"."+QString::number(PATCH_VERSION)+(QString(STRINGIZE(BUILD)).isEmpty()?"":" ")+STRINGIZE(BUILD) + " Manual");
	o->paragraph("This file documents wkhtmltoimage, a program capable of converting HTML "
				 "documents into images.");
	o->endSection();
}

/*!
  Output information on how to compile
  \param o The outputter to output to
*/
void CommandLineParser::outputCompilation(Outputter * o) const {
	o->beginSection("Compilation");
	o->paragraph("It can happen that the static binary does not work for your system "
		     "for one reason or the other, in that case you might need to compile "
		     "wkhtmltoimage yourself.");
	o->endParagraph();
	o->endSection();
}

/*!
  Output information on how to install
  \param o The outputter to output to
*/
void CommandLineParser::outputInstallation(Outputter * o) const {
	o->beginSection("Installation");
	o->paragraph(
		"There are several ways to install wkhtmltoimage.  You can download a "
		"already compiled binary, or you can compile wkhtmltoimage yourself. ");
	o->endSection();
}

/*!
  Output examples on how to use wkhtmltoimage
  \param o The outputter to output to
*/
void CommandLineParser::outputExamples(Outputter * o) const {
	o->beginSection("Examples");
	o->paragraph("This section presents a number of examples of how to invoke wkhtmltoimage.");
	o->paragraph("To convert a remote HTML file to PNG:");
	o->verbatim("wkhtmltoimage http://www.google.com google.png\n");
	o->paragraph("To convert a local HTML file to PNG:");
	o->verbatim("wkhtmltoimage my.html my.png\n");
	o->endSection();
}
