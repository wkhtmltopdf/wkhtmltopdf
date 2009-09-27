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
#include "commandlineparser_p.hh"

/*!
  Output the name and version of the program, and also wether we are using a patched qt
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputName(Outputter * o) const {
	o->beginSection("Name");
	o->beginParagraph();
	o->text(QString("wkhtmltopdf ")+QString::number(MAJOR_VERSION)+"."+QString::number(MINOR_VERSION)+"."+QString::number(PATCH_VERSION));;
	o->text(", ");
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	o->text("using wkhtmltopdf patched qt.");
#else
	o->bold("not");
	o->text(" using wkhtmltopdf patched qt.");
#endif
	o->endParagraph();
	o->endSection();
}

/*!
  Output copyright stuff
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputLicense(Outputter * o) const {
	o->beginSection("License");
	o->beginParagraph();
	o->text("Copyright (C) 2008,2009 WKHtmlToPdf Authors.");
	o->endParagraph();
	o->beginParagraph();
	o->text("License GPLv3+: GNU GPL version 3 or later ");
	o->link("http://gnu.org/licenses/gpl.html");
	o->text("This is free software: you are free to change and redistribute it. "
			"There is NO WARRANTY, to the extent permitted by law.");
	o->endParagraph();
}

/*!
  Output list of authors
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputAuthors(Outputter * o) const {
	o->beginSection("Authors");
	o->beginParagraph();
	o->text("Written by Jakob Truelsen. "
			"Patches by Patches by Mário Silva and Emmanuel Bouthenot.");
	o->endParagraph();
	o->endSection();
}

/*!
  Output a short synnapsis on how to call the commandline program
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputSynopsis(Outputter * o) const {
	o->beginSection("Synopsis");
	o->verbatim("wkhtmltopdf [OPTIONS]... <input file> [More inputfiles] <output file>");
	o->endSection();
}

/*!
  Explain what the program does
  \param o The outputter to output to
  \todo Do a better explanation
*/
#warning "add a better explanation"
void CommandLineParserPrivate::outputDescripton(Outputter * o) const {
	o->beginSection("Description");
	o->beginParagraph();
	o->text("Converts one or more htmlpage to a pdf document.");
	o->endParagraph();
	o->endSection();
}

/*!
  Add explanation about reduced functionality without patched qt/webkit
  \param o The outputter to output to
  \param sure Is the functionality restricted in this wkhtmltopdf
  \todo Do a better explanation
*/
#warning "explain what and why"
void CommandLineParserPrivate::outputNotPatched(Outputter * o, bool sure) const {
	o->beginSection("Reduced Functionality");
	o->endSection();
}

/*!
  Explain the page breaking is somewhat broken
  \param o The outputter to output to
  \todo Do a better explanation
*/
#warning "explain what and why"
void CommandLineParserPrivate::outputPageBreakDoc(Outputter * o) const {

}

/*!
  Output contact information
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputContact(Outputter * o) const {
	o->beginSection("Contact");
	o->beginParagraph();
	o->text("If you experience bugs or want to requent new features please visit ");
	o->link("http://code.google.com/p/wkhtmltopdf/issues/list");
	o->text(", if you have any problems or comments please feel free to contact me: see ");
	o->link("http://www.madalgo.au.dk/~jakobt/#about");
	o->endParagraph();
	o->endSection();
}
