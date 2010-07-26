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

#include "commandlineparserbase.hh"
#include "outputter.hh"

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

/*!
  Output the name and version of the program, and also whether we are using a patched qt
  \param o The outputter to output to
*/
void CommandLineParserBase::outputName(Outputter * o) const {
	o->beginSection("Name");
	o->paragraph(appName()+" "+QString::number(MAJOR_VERSION)+"."+QString::number(MINOR_VERSION)+"."+QString::number(PATCH_VERSION)+(QString(STRINGIZE(BUILD)).isEmpty()?"":" ")+STRINGIZE(BUILD));
	o->endSection();

}

/*!
  Output copyright stuff
  \param o The outputter to output to
*/
void CommandLineParserBase::outputLicense(Outputter * o) const {
	o->beginSection("License");
	o->paragraph("Copyright (C) 2010 wkhtmltopdf/wkhtmltoimage Authors.");
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
void CommandLineParserBase::outputAuthors(Outputter * o) const {
	o->beginSection("Authors");
	o->paragraph(
		QString::fromUtf8(
			"Written by Jan Habermann, Christian Sciberras and Jakob Truelsen. "
			"Patches by Mehdi Abbad, Lyes Amazouz, Pascal Bach, Emmanuel Bouthenot, Benoit Garret and Mário Silva."));
	o->endSection();
}

/*!
  Output information on the problems with the static version
  \param o The outputter to output to
*/
void CommandLineParserBase::outputStaticProblems(Outputter * o) const {
	o->beginSection("Static version");
	o->beginParagraph();
	o->text("On the wkhtmltopdf website you can download a static version of wkhtmltopdf ");
	o->link("http://code.google.com/p/wkhtmltopdf/downloads/list");
	o->text(". This static binary will work on most systems and comes with a build in patched QT.");
	o->endParagraph();

	o->beginParagraph();
	o->text("Unfortunately the static binary is not particularly static, on Linux it depends "
			"on both glibc and openssl, furthermore you will need to have an xserver installed "
			"but not necessary running. You will need to have different fonts install including "
			"xfonts-scalable (Type1), and msttcorefonts.  See ");
	o->link("http://code.google.com/p/wkhtmltopdf/wiki/static");
	o->text(" for trouble shouting.");
	o->endParagraph();
	o->endSection();
}

/*!
  Output documentation about the proxy settings
  \param o The outputter to output to
*/
void CommandLineParserBase::outputProxyDoc(Outputter * o) const {
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
