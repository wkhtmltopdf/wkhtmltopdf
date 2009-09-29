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
	o->paragraph(
		QString::fromUtf8(
			"Written by Jakob Truelsen. "
			"Patches by Patches by Mário Silva and Emmanuel Bouthenot."));
	o->endSection();
}

/*!
  Output a short synnapsis on how to call the commandline program
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputSynopsis(Outputter * o) const {
	o->beginSection("Synopsis");
	o->verbatim("wkhtmltopdf [OPTIONS]... <input file> [More inputfiles] <output file>\n");
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
	o->paragraph(
		"Converts one or more htmlpage to a pdf document.");
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
	o->beginParagraph();
	o->text(
 	        "This version of wkhtmltopdf has been compiled against a version of qt"
 	        "without the wkhtmltopdf patches, so some features are missign, if you need"
 	        "these features please use the staic edition");
	o->endSection();
}

/*!
  Explain the page breaking is somewhat broken
  \param o The outputter to output to
  \todo Do a better explanation
*/
#warning "explain what and why"
void CommandLineParserPrivate::outputPageBreakDoc(Outputter * o) const {
	o->beginSection("Page Breaking");
	o->endSection();
}

/*!
  Output documentation about the proxy settings
  \param o The outputter to output to
  \todo Do a better explanation
*/
#warning "explain what and why"
void CommandLineParserPrivate::outputProxyDoc(Outputter * o) const {
	o->beginSection("Specifying A Proxy");
	o->paragraph(
		"By default proxy information will be read from the environment"
		" variables: proxy, all_proxy and http_proxy, proxy options can"
		" also by specified with the -p switch");
	o->endParagraph();
	o->verbatim(
		"<type> := \"http://\" | \"socks5://\"\n"
		"<userinfo> := <username> (\":\" <password>)? \"@\"\n"
		"<proxy> := \"None\" | <type>? <userinfo>? <host> (\":\" <port>)?\n");
	o->endSection();
}

/*!
  Output documentation about headers and footers
  \param o The outputter to output to
  \todo Do a better explanation
*/
#warning "explain what and why"
void CommandLineParserPrivate::outputHeaderFooterDoc(Outputter * o) const {
	o->beginSection("Footers And Headers");
	o->paragraph("In a header or footer text the following variables can be used.");
	o->verbatim(
 	        " * [page]       Replaced by the number of the pages currently beeing printed\n"
 	        " * [fromPage]   Replaced by the number of the first page to be printed\n"
 	        " * [toPage]     Replaced by the number of the last page to be printed\n"
 	        " * [webpage]    Replaced by the url of the page beeing printed\n"
 	        " * [section]    Replaced by the name of the current section\n"
 	        " * [subsection] Replaced by the name of the current subsection\n"
 	        "\n");
	o->endSection();
}

/*!
  Output documentation about outlines
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputOutlineDoc(Outputter * o) const {
	o->beginSection("Outlines");
	o->paragraph(
		"Wkhtmltodpf with patched qt has support for pdf outlines also known as "
		"book marks, this can be enabeled by specifying the --outline switch. "
		"The outlines are generated based on the <h?> tags, for a indepth "
		"description of how this is done see the \"Table Of Contest\" section. ");
	o->paragraph(
		"The outline tree can sometimes be very deep, if the <h?> tags where "
		"spred to generous in the HTML document.  The --outline-depth switch can "
		"be used to bound this.");
	o->endSection();
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

/*!
  Output beginning of the readmee
  \param o The outputter to output to
  \todo Do a better explanation
*/
#warning "explain what and why"
void CommandLineParserPrivate::outputDocStart(Outputter * o) const {
	o->beginSection("WKHtmlToPdf Manual");
	o->endSection();
}

/*!
  Output information on how to compile
  \param o The outputter to output to
  \todo Do a better explanation
*/
#warning "explain what and why"
void CommandLineParserPrivate::outputCompilation(Outputter * o) const {
	o->beginSection("Compilation");
	o->endSection();
}

/*!
  Output information on how to install
  \param o The outputter to output to
  \todo Do a better explanation
*/
#warning "explain what and why"
void CommandLineParserPrivate::outputInstallation(Outputter * o) const {
	o->beginSection("Installation");
	o->endSection();
}

/*!
  Output exampels on how to use wkhtmltopdf
  \param o The outputter to output to
  \todo Do a better explanation
*/
#warning "explain what and why"
void CommandLineParserPrivate::outputExampels(Outputter * o) const {
	o->beginSection("Exampels");
	o->endSection();
}



