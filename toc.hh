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
#ifndef __toc_hh__
#define __toc_hh__

#include <QPainter>
#include <QPrinter>
#include <QString>
#include <QtWebKit>

class TocItem {
public:
	QVector<TocItem*> children;
	TocItem * parent;
	uint page;
	QString value;
	QPointF location;
	~TocItem() {for(int i=0; i < children.size(); ++i) delete children[i];}
};

void buildToc(TocItem * root, const QVector<QWebFrame::Heading> & headings, uint firstPage);

class TocPrinter: public QObject {
	Q_OBJECT
public:
	static const uint levels = 7;
	bool useDots;
	const char * font_name;
	int depth;
	int outline_depth;
	int indentation[levels];
	int font_size[levels];
	int header_font_size;
	const char * header_text;
private:
	void printChildren(TocItem * item, QPrinter * printer, QPainter * painter, bool dryRun, uint level, uint & page, double & y); 
	uint print(TocItem * root, QPrinter * printer, QPainter * painter, bool dryRun);
	void outlineChildren(TocItem * item, QPrinter * printer, int level);
public:
	uint print(TocItem * root, QPrinter * printer, QPainter * painter) {
		return print(root,printer,painter,false);
	}
	uint countPages(TocItem * root, QPrinter * printer, QPainter * painter) {
		return print(root,printer,painter,true);
	}
	void outline(TocItem * root, QPrinter * printer) { 
		outlineChildren(root,printer,0);
	}
Q_SIGNALS:
	void printingNewPage(QPrinter *p, int fromPage, int toPage, int Page) const;
};
#endif
