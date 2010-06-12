/*
 * File:   docparts.cc
 * Author: Christian Sciberras
 * Created: 20 May 2010
 *   This file is part of wkhtmltoimage.
 *   wkhtmltoimage is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *   wkhtmltoimage is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   You should have received a copy of the GNU General Public License
 *   along with wkhtmltoimage.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "commandlineparser_p.hh"
#include <QWebFrame>

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

/*!
  Output the name and version of the program, and also whether we are using a patched qt
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputName(Outputter * o) const {
	o->beginSection("Name");
	o->paragraph(QString("wkhtmltoimage ")+QString::number(MAJOR_VERSION)+"."+QString::number(MINOR_VERSION)+"."+QString::number(PATCH_VERSION)+(QString(STRINGIZE(BUILD)).isEmpty()?"":" ")+STRINGIZE(BUILD));
	o->endSection();
}


/*!
  Output name and a short description
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputManName(Outputter * o) const {
	o->beginSection("Name");
	o->paragraph("wkhtmltoimage - html to image converter");
	o->endSection();
}

/*!
  Output copyright stuff
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputLicense(Outputter * o) const {
	o->beginSection("License");
	o->paragraph("Copyright (C) 2010 wkhtmltoimage Authors.");
	o->paragraph("Copyright (C) 1989, 1991 by Jef Poskanzer.");
	o->endParagraph();
	o->beginParagraph();
	o->text("License GPLv3+: GNU GPL version 3 or later ");
	o->link("http://gnu.org/licenses/gpl.html");
	o->text(". This is free software: you are free to change and redistribute it. "
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
			"Written by Christian Sciberras. "
			"Adopted code from wkhtmltopdf by Jakob Truelsen."));
	o->endSection();
}

/*!
  Output a short synopsis on how to call the command line program
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputSynopsis(Outputter * o) const {
	o->beginSection("Synopsis");
	o->verbatim("wkhtmltoimage [OPTIONS]... <input file> <output file>\n");
	o->endSection();
}

/*!
  Explain what the program does
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputDescripton(Outputter * o) const {
	o->beginSection("Description");
	o->beginParagraph();
	o->text("Converts an HTML page into an image, ");
	o->endParagraph();
	o->endSection();
}

/*!
  Output documentation about the proxy settings
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputProxyDoc(Outputter * o) const {
	o->beginSection("Specifying A Proxy");
	o->paragraph(
		"By default proxy information will be read from the environment"
		" variables: proxy, all_proxy and http_proxy, proxy options can"
		" also by specified with the -p switch");
	o->verbatim(
		"<type> := \"http://\" | \"socks5://\"\n"
		"<serif> := <username> (\":\" <password>)? \"@\"\n"
		"<proxy> := \"None\" | <type>? <sering>? <host> (\":\" <port>)?\n");
	o->paragraph("Here are some examples (In case you are unfamiliar with the BNF):");
	o->verbatim("http://user:password@myproxyserver:8080\n"
				"socks5://myproxyserver\n"
				"None\n");
	o->endSection();
}

/*!
  Output contact information
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputContact(Outputter * o) const {
	o->beginSection("Contact");
	o->beginParagraph();
	o->text("If you experience bugs or want to request new features please visit ");
	o->link("http://code.google.com/p/wkhtmltoimage/issues/list");
	o->text(", if you have any problems or comments please feel free to contact me: ");
	o->link("uuf6429@gmail.com");
	o->endParagraph();
	o->endSection();
}

/*!
  Output beginning of the readme
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputDocStart(Outputter * o) const {
	o->beginSection(QString("wkhtmltoimage ")+QString::number(MAJOR_VERSION)+"."+QString::number(MINOR_VERSION)+"."+QString::number(PATCH_VERSION)+(QString(STRINGIZE(BUILD)).isEmpty()?"":" ")+STRINGIZE(BUILD) + " Manual");
	o->paragraph("This file documents wkhtmltoimage, a program capable of converting HTML "
				 "documents into images.");
	o->endSection();
}

/*!
  Output information on how to compile
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputCompilation(Outputter * o) const {
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
void CommandLineParserPrivate::outputInstallation(Outputter * o) const {
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
void CommandLineParserPrivate::outputExamples(Outputter * o) const {
	o->beginSection("Examples");
	o->paragraph("This section presents a number of examples of how to invoke wkhtmltoimage.");
	o->paragraph("To convert a remote HTML file to PNG:");
	o->verbatim("wkhtmltoimage http://www.google.com google.png\n");
	o->paragraph("To convert a local HTML file to PNG:");
	o->verbatim("wkhtmltoimage my.html my.png\n");
	o->endSection();
}

/*!
  Output information on the problems with the static version
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputStaticProblems(Outputter * o) const {
	o->beginSection("Static version");
	o->beginParagraph();
	o->text("On the wkhtmltoimage website you can download a static version of wkhtmltoimage ");
	o->link("http://code.google.com/p/wkhtmltoimage/downloads/list");
	o->text(". This static binary will work on most systems and comes with a build in patched QT.");
	o->endParagraph();

	o->beginParagraph();
	o->text("Unfortunately the static binary is not particularly static, on Linux it depends "
			"on both glibc and openssl, furthermore you will need to have an xserver installed "
			"but not necessary running. You will need to have different fonts install including "
			"xfonts-scalable (Type1), and msttcorefonts.  See ");
	o->link("http://code.google.com/p/wkhtmltopdf/wiki/static");
	o->text(" for trouble shooting.");
	o->endParagraph();
	o->endSection();
}
