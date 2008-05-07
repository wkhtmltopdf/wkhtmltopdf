#ifndef __wkhtmltopdf_hh__
#define __wkhtmltopdf_hh__
#include <QObject>
#include <QtGui>
#include <QWebView>
class WKHtmlToPdf : public QObject {
	Q_OBJECT
public:
	QWebView v;
	char * out;
	void run(int argc, char** argv);
public slots:
	void loadFinished();
	void loadProgress(int progress);
};
#endif //__wkhtmltopdf_hh__
