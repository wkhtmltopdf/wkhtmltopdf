/* 
 * File:   pageloader.cpp
 * Author: Christian Sciberras
 * 
 * Created on 20 ta’ Mejju 2010, 00:21
 */

#include "pageloader.hh"

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

void PageLoader::loaded(bool ok) {

}

PageLoader::PageLoader(Settings & s){
	httpErrorCode=0;
	settings=s;
}

PageLoader::~PageLoader() {
}

bool PageLoader::convert() {
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
