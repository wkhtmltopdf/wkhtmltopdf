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

#ifndef __wkhtmltopdf_hh__
#define __wkhtmltopdf_hh__
#include <QObject>
#include <QtGui>
#include <QWebView>

//Class responsible for the convertion
class WKHtmlToPdf : public QObject {
	Q_OBJECT
public:
	//The webview is used to fetch and render the webpage using webkit
	QWebView v; 
	//out is the path of the output file
	char * out;
	void run(int argc, char** argv);
public slots:
	void loadFinished(bool ok);
	void loadProgress(int progress);
};
#endif //__wkhtmltopdf_hh__
