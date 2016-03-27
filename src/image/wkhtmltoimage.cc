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

#include "imagecommandlineparser.hh"
#include "progressfeedback.hh"
#include <QApplication>
#include <QWebFrame>
#include <wkhtmltox/imageconverter.hh>
#include <wkhtmltox/imagesettings.hh>
#include <wkhtmltox/utilities.hh>
#include "declarations.hh"

#if defined(Q_OS_UNIX)
#include <locale.h>
#endif

int toimage(int argc, char** argv) {
#if defined(Q_OS_UNIX)
	setlocale(LC_ALL, "");
#endif
	//This will store all our settings
	wkhtmltopdf::settings::ImageGlobal settings;
	//Create a command line parser to parse commandline arguments
	ImageCommandLineParser parser(settings);

    bool use_graphics=true;
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
    use_graphics=settings.useGraphics;
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

	//Create the actual page converter to convert the pages
	wkhtmltopdf::ImageConverter converter(settings);
	QObject::connect(&converter, SIGNAL(checkboxSvgChanged(const QString &)), style, SLOT(setCheckboxSvg(const QString &)));
	QObject::connect(&converter, SIGNAL(checkboxCheckedSvgChanged(const QString &)), style, SLOT(setCheckboxCheckedSvg(const QString &)));
	QObject::connect(&converter, SIGNAL(radiobuttonSvgChanged(const QString &)), style, SLOT(setRadioButtonSvg(const QString &)));
	QObject::connect(&converter, SIGNAL(radiobuttonCheckedSvgChanged(const QString &)), style, SLOT(setRadioButtonCheckedSvg(const QString &)));

	wkhtmltopdf::ProgressFeedback feedback(settings.quiet, converter);
	bool success = converter.convert();
	return handleError(success, converter.httpErrorCode());
}
