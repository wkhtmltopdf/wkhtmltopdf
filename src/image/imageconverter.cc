// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
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
#include "imageconverter_p.hh"
#include "settings.hh"
#include <QObject>
#include <QWebPage>
#include <QWebFrame>
#include <QUrl>
#include <QImage>
#include <QPainter>
#include <QObject>
#include <QEventLoop>
#include <QFileInfo>
#include <QDebug>
#include <qapplication.h>

namespace wkhtmltopdf {

ImageConverterPrivate::ImageConverterPrivate(ImageConverter & o, wkhtmltopdf::settings::Global & s):
	settings(s),
	loader(s.loadGlobal),
	out(o) {
	
	phaseDescriptions.push_back("Loading page");
	phaseDescriptions.push_back("Rendering");
	phaseDescriptions.push_back("Done");

	connect(&loader, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
	connect(&loader, SIGNAL(loadFinished(bool)), this, SLOT(pagesLoaded(bool)));
	connect(&loader, SIGNAL(error(QString)), this, SLOT(forwardError(QString)));
	connect(&loader, SIGNAL(warning(QString)), this, SLOT(forwardWarning(QString)));
};

void ImageConverterPrivate::beginConvert() {
	error = false;
	convertionDone = false;
	progressString = "0%";
	loaderObject = loader.addResource(settings.in, settings.loadPage);
	updateWebSettings(loaderObject->page.settings(), settings.web);
	currentPhase=0;
	emit out. phaseChanged();
	loadProgress(0);
	loader.load();
}


void ImageConverterPrivate::clearResources() {
	loader.clearResources();
}

void ImageConverterPrivate::pagesLoaded(bool ok) {
	if (errorCode == 0) errorCode = loader.httpErrorCode();
	if (!ok) {
		fail(); 
		return;
	}

	// if fmt is empty try to get it from file extension in out
	if(settings.fmt==""){
		if (settings.out == "-")
			settings.fmt = "jpg";
		else {
			QFileInfo fi(settings.out);
			settings.fmt = fi.suffix();
		}
	}
	
	// check whether image format is supported (for writing)
//	QImageWriter test;
//	test.setFormat(settings.fmt);
//	if(!test.canWrite()){
//		if(!settings.quiet)printf("error: file format not supported\n");
//		httpErrorCode=EFAULT;
//		return false;
//	}
	// create webkit frame and load website
	
	currentPhase=1;
	emit out. phaseChanged();
	loadProgress(0);
	loaderObject->page.setViewportSize(loaderObject->page.mainFrame()->contentsSize());
	QImage image(loaderObject->page.viewportSize(), QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&image);
	if (!settings.transparent || settings.fmt != "png")
		painter.fillRect(image.rect(), Qt::white);

	loaderObject->page.mainFrame()->render(&painter);
	painter.end();
	loadProgress(30);
	// perform filter(s)
	if (settings.crop.width > 0 && settings.crop.height > 0) 
		image=image.copy(settings.crop.left,settings.crop.top,settings.crop.width,settings.crop.height);

	loadProgress(50);
	if (settings.scale.width > 0 && settings.scale.height > 0) {
		// todo: perhaps get more user options to change aspect ration and scaling mode?
		image=image.scaled(settings.scale.width,settings.scale.height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	}
	loadProgress(80);


	QFile file;
	bool openOk;
	// output image
	if (settings.out != "-" ) {
		file.setFileName(settings.out);
		openOk = file.open(QIODevice::WriteOnly);
	} else {
		openOk = file.open(stdout, QIODevice::WriteOnly);
	}
	if (!openOk) {
		emit out.error("Could not write to output file");
		fail();
	}

	QByteArray fmt=settings.fmt.toLatin1();
	if (!image.save(&file,fmt.data(),-1)) {
		emit out.error("Could not save image");
		fail();
	} 

	loadProgress(100);

	currentPhase = 2;
	emit out.phaseChanged();
	convertionDone = true;
	emit out.finished(true);

	qApp->exit(0); // quit qt's event handling
}

Converter & ImageConverterPrivate::outer() {
	return out;
}

ImageConverter::~ImageConverter() {
	delete d;
}

ConverterPrivate & ImageConverter::priv() {
	return *d;
}


ImageConverter::ImageConverter(wkhtmltopdf::settings::Global & s) {
	d = new ImageConverterPrivate(*this, s);
}

}
