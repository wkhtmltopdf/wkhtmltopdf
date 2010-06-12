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
#include "converter.hh"

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

void Converter::loaded(bool ok) {

}

Converter::Converter(wkhtmltopdf::settings::Global & s){
  httpErrorCode=0;
	settings=s;
}

Converter::~Converter() {
}

bool Converter::convert() {
	// init error code
	httpErrorCode=0;
	// quickfix; if out is "-", set quiet to ON
	if(settings.out=="-")settings.quiet=true;
	// if fmt is empty try to get it from file extension in out
	if(settings.fmt==""){
		QFileInfo fi(settings.out);
        settings.fmt = fi.suffix();
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
	QWebPage page;
	if(settings.in!="-"){
		// get html from url/file
		if(!settings.quiet)printf("loading from url...\n");
		page.mainFrame()->load(settings.in);
	}else{
		// get html from stdin
		// TODO: grab from stdin
	}
	// begin looping till loadFinished is encountered
	QEventLoop loop;
	QObject::connect(&page, SIGNAL(loadFinished(bool)), &loop, SLOT( quit( )));
	loop.exec( );
	// paint image
	if(!settings.quiet)printf("downloading complete, converting...\n");
	page.setViewportSize(page.mainFrame()->contentsSize());

	
	QImage image(page.viewportSize(), QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&image);
	//painter.begin();
	page.mainFrame()->render(&painter);
	painter.end();
	// perform filter(s)
	if(settings.crop.width!=-1 && settings.crop.height!=-1){
		if(!settings.quiet)printf("cropping...\n");
		// note: the returned image object is a copy; is this a memleak? does c++ have a GC?
		image=image.copy(settings.crop.left,settings.crop.top,settings.crop.width,settings.crop.height);
	}
	if(settings.scale.width!=-1 && settings.scale.height!=-1){
		if(!settings.quiet)printf("scaling...\n");
		// todo: perhaps get more user options to change aspect ration and scaling mode?
		// note: the returned image object is a copy; is this a memleak? does c++ have a GC?
		image=image.scaled(settings.scale.width,settings.scale.height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	}
	// output image
	if(settings.out!="-"){
		// save to file
		if(!settings.quiet)printf("saving image...\n");
		QByteArray ba=settings.fmt.toLatin1();
		if(!image.save(QString(settings.out),ba.data(),-1)){
			if(!settings.quiet)printf("error: couldn't save image\n");
			httpErrorCode=402; //EACCES; // file could not be written
			return false;
		}else{
			if(!settings.quiet)printf("finished.\n");
			return true;
		}
	}else{
		// save to stdout
		// TODO: print to stdout
	}
	return false;
}
