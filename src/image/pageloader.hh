/* 
 * File:   pageloader.h
 * Author: Christian Sciberras
 *
 * Created on 20 ta’ Mejju 2010, 00:21
 */

#include <QObject>
#include <QWebPage>
#include "settings.hh"

class PageLoader: public QObject {
	Q_OBJECT
public:
	PageLoader(Settings & s);
	~PageLoader();
	int httpErrorCode;
	Settings settings;
	bool convert();
public slots:
	void loaded(bool ok);
};

