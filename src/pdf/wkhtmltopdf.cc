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

#include "pdfcommandlineparser.hh"
#include "progressfeedback.hh"
#include <QCommonStyle>
#include <QPainter>
#include <QStyleOption>
#include <QWebFrame>
#include <QtPlugin>
#include <cstdlib>
#include <iostream>
#include <qapplication.h>
#include <qglobal.h>
#include <string.h>
#include <wkhtmltox/pdfconverter.hh>
#include <wkhtmltox/pdfsettings.hh>
#include <wkhtmltox/utilities.hh>
#include "declarations.hh"

#if defined(Q_OS_UNIX)
#include <locale.h>
#endif

using namespace wkhtmltopdf::settings;
using namespace wkhtmltopdf;

/*!
 * State mashine driven, shell like parser. This is used for
 * reading commandline options from stdin
 * \param buff the line to parse
 * \param nargc on return will hold the number of arguments read
 * \param nargv on return will hold the arguments read and be NULL terminated
 */
enum State {skip, tok, q1, q2, q1_esc, q2_esc, tok_esc};
void parseString(char * buff, int &nargc, QStringList& nargv) {
	State state = skip;
	int write_start=0;
	int write=0;
	for (int read=0; buff[read]!='\0'; ++read) {
		State next_state=state;
		switch (state) {
		case skip:
			//Whitespace skipping state
			if (buff[read]!=' ' && buff[read]!='\t' && buff[read]!='\r' && buff[read]!='\n') {
				--read;
				next_state=tok;
			}
			break;
		case tok:
			//Normal toking reading state
			if (buff[read]=='\'') next_state=q1;
			else if (buff[read]=='"') next_state=q2;
			else if (buff[read]=='\\') next_state=tok_esc;
			else if (buff[read]==' ' || buff[read]=='\t' || buff[read]=='\n' || buff[read]=='\r') {
				next_state=skip;
				if (write_start != write) {
					buff[write++]='\0';
                    nargc++;
                    nargv.append(QString::fromUtf8(buff+write_start));
					if (nargc > 998) exit(1);
				}
				write_start = write;
			} else buff[write++] = buff[read];
			break;
		case q1:
			//State parsing a single qoute argument
			if (buff[read]=='\'') next_state=tok;
			else if (buff[read]=='\\') next_state=q1_esc;
			else buff[write++] = buff[read];
			break;
		case q2:
			//State parsing a double qoute argument
			if (buff[read]=='"') next_state=tok;
			else if (buff[read]=='\\') next_state=q2_esc;
			else buff[write++] = buff[read];
			break;
		case tok_esc:
			//Escape one char and return to the token parsing state
			next_state=tok;
			buff[write++] = buff[read];
			break;
		case q1_esc:
			//Espace one char and return to the single quote parsing state
			next_state=q1;
			buff[write++] = buff[read];
			break;
		case q2_esc:
			//Escape one char and return to the double qoute parsing state
			next_state=q2;
			buff[write++] = buff[read];
			break;
		}
		state=next_state;
	}
	//Remember the last parameter
	if (write_start != write) {
		buff[write++]='\0';
        nargc++;
        nargv.append(QString::fromUtf8(buff+write_start));
    }
    nargc = nargv.size();
}

int topdf(int argc, char * argv[]) {
#if defined(Q_OS_UNIX)
	setlocale(LC_ALL, "");
#endif
	//This will store all our settings
	PdfGlobal globalSettings;
	QList<PdfObject> objectSettings;
	//Create a command line parser to parse commandline arguments
	PdfCommandLineParser parser(globalSettings, objectSettings);

	//Setup default values in settings
	//parser.loadDefaults();

    //Construct QApplication required for printing
    bool use_graphics=true;
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
    use_graphics=globalSettings.useGraphics;
    if (!use_graphics) QApplication::setGraphicsSystem("raster");
#endif
#endif
    QApplication a(argc, argv, use_graphics);
    QStringList args = a.arguments();
    if (argc > 1 && QRegExp("image|pdf").exactMatch(args.at(1)))
        args.removeAt(1);

	//Parse the arguments
    parser.parseArguments(args);
	MyLooksStyle * style = new MyLooksStyle();
	a.setStyle(style);

	if (parser.readArgsFromStdin) {
		char buff[20400];
        QStringList nargv = args;
		while (fgets(buff,20398,stdin)) {
            int nargc=args.size();
			parseString(buff,nargc,nargv);

			PdfGlobal globalSettings;
			QList<PdfObject> objectSettings;
			//Create a command line parser to parse commandline arguments
			PdfCommandLineParser parser(globalSettings, objectSettings);
			//Setup default values in settings
			//parser.loadDefaults();
			//Parse the arguments
            parser.parseArguments(nargv, true);

			PdfConverter converter(globalSettings);
			ProgressFeedback feedback(globalSettings.quiet, converter);
			foreach (const PdfObject & object, objectSettings)
				converter.addResource(object);

			if (!converter.convert())
				exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	//Create the actual page converter to convert the pages
	PdfConverter converter(globalSettings);
	QObject::connect(&converter, SIGNAL(producingForms(bool)), style, SLOT(producingForms(bool)));
	QObject::connect(&converter, SIGNAL(checkboxSvgChanged(const QString &)), style, SLOT(setCheckboxSvg(const QString &)));
	QObject::connect(&converter, SIGNAL(checkboxCheckedSvgChanged(const QString &)), style, SLOT(setCheckboxCheckedSvg(const QString &)));
	QObject::connect(&converter, SIGNAL(radiobuttonSvgChanged(const QString &)), style, SLOT(setRadioButtonSvg(const QString &)));
	QObject::connect(&converter, SIGNAL(radiobuttonCheckedSvgChanged(const QString &)), style, SLOT(setRadioButtonCheckedSvg(const QString &)));

	ProgressFeedback feedback(globalSettings.quiet, converter);
	foreach (const PdfObject & object, objectSettings)
		converter.addResource(object);

	bool success = converter.convert();
	return handleError(success, converter.httpErrorCode());
}
