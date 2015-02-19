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

#include "commandlineparserbase.hh"
#include "outputter.hh"
#include <QFile>

/*!
  Output the name and version of the program, and also whether we are using a patched qt
  \param o The outputter to output to
*/
void CommandLineParserBase::outputName(Outputter * o) const {
	o->beginSection("Name");
	o->paragraph(appName()+" "+appVersion());
	o->endSection();
}

/*!
  Output copyright stuff
  \param o The outputter to output to
*/
void CommandLineParserBase::outputLicense(Outputter * o) const {
	o->beginSection("License");
	o->paragraph("Copyright (c) 2010-2014 wkhtmltopdf authors");
	QFile file(":/LICENSE");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream stream(&file);
	o->verbatim(stream.readAll());
	o->endSection();
}

/*!
  Output list of authors
  \param o The outputter to output to
*/
void CommandLineParserBase::outputAuthors(Outputter * o) const {
	o->beginSection("Authors");
	QFile file(":/AUTHORS");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream stream(&file);
	o->verbatim(stream.readAll());
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
	o->link("http://wkhtmltopdf.org/downloads.html");
	o->text(". This static binary will work on most systems and comes with a build in patched QT.");
	o->endParagraph();

	o->beginParagraph();
	o->text("Unfortunately the static binary is not particularly static, on Linux it depends "
			"on both glibc and openssl, furthermore you will need to have an xserver installed "
			"but not necessary running. You will need to have different fonts install including "
			"xfonts-scalable (Type1), and msttcorefonts.");
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
		"<proxy> := \"None\" | <type>? <string>? <host> (\":\" <port>)?\n");
	o->paragraph("Here are some examples (In case you are unfamiliar with the BNF):");
	o->verbatim("http://user:password@myproxyserver:8080\n"
				"socks5://myproxyserver\n"
				"None\n");
	o->endSection();
}
