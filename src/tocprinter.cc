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
#include "tocprinter.hh"
#include "outline_p.hh"
#include "settings.hh"
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__


/*!
  We imploy a cheep strategy of not using a render tree, 
  insted we just store the state render state for every page start,
  the render state is uniquly defined by a OutlineItem, 
 */
class TocPrinterPrivate {
public:
	Outline * outline;
	QPrinter * printer;
	QPainter & painter;
		
	QVector<OutlineItem *> pageItems;
	
	TocPrinterPrivate(Outline * o, QPrinter * pr, QPainter & pa);
	void revLinkChildren(OutlineItem * i, QVector<QPair<QWebElement, QString> > & links);

	void renderPage(OutlineItem * & cur, bool first, bool dry);
};


TocPrinterPrivate::TocPrinterPrivate(Outline * o, QPrinter * pr, QPainter & pa):
	outline(o), printer(pr), painter(pa) {
}


void TocPrinterPrivate::revLinkChildren(OutlineItem * i, QVector<QPair<QWebElement, QString> > & links) {
	foreach (OutlineItem * j,i->children) {
		links.push_back( qMakePair(j->element, j->anchor+QString("_R") ) );
		revLinkChildren(j, links);
	}
}

void TocPrinterPrivate::renderPage(OutlineItem * & cur, bool first, bool dry) {
	double y=0;
	const Settings::TOCSettings & s = outline->d->settings.toc;
	QRect pr = printer->pageRect();

	int lvl = 0;
	for(OutlineItem * x=cur; x; x = x->parent) ++lvl;

	if(first) {
		double h = painter.boundingRect(pr,Qt::AlignTop | Qt::AlignHCenter, s.captionText).height();
		QRect r((int)h,0,pr.width(),(int)h*3);
		if(!dry) 
			painter.drawText(r, Qt::AlignVCenter | Qt::AlignHCenter, s.captionText);
		y += r.height();
	}

	while(true) {
		painter.setFont(QFont(s.fontName, s.fontSize[lvl]));		
		double step=painter.fontMetrics().height();
		if (y+step > pr.height()) return;
		if (!dry) {
			double startX = pr.width()*s.indentation[lvl]/1000.0;
			QRectF lineRect(startX,y,pr.width()-startX,step);
			QRectF br;
			painter.drawText(lineRect,Qt::AlignBottom | Qt::AlignRight, QString::number(cur->page),&br);
			QString v = cur->value;
			if (s.useDots) {
				v += " ";
				//Calculate the number of dots needed to fill the line
				double dw = painter.boundingRect(pr, Qt::AlignRight | Qt::AlignTop, ".").width();
				int ndots = (br.left() - startX - painter.boundingRect(lineRect, Qt::AlignLeft, v+" ").width())/dw;
				for (int i=0; i < ndots; ++i) v += ".";
			}
			painter.drawText(lineRect,Qt::AlignBottom | Qt::AlignLeft, v);
		}
		y += step;
		
		++lvl;
		if (cur->children.size() != 0 && lvl+1 < s.depth) //TODO check for max depth here
			cur = cur->children[0];
		else {
			while(true) {
				--lvl;
				OutlineItem * i = cur->parent;
				if (i == NULL) {
					cur = NULL;
					return;
				}
				int j = i->children.indexOf(cur);
				if (j+1 < i->children.size()) {
					cur = i->children[j+1];
					break;
				}
			}
		}
	}
}


/*!
  \file tocprinter.hh
  \brief Defines the TocPrinter class
*/

/*!
  \class TocPrinter
  \brief Class responsible for rendering a table of content
*/


TocPrinter::TocPrinter(Outline * outline, QPrinter * printer, QPainter & painter):
  d(new TocPrinterPrivate(outline, printer, painter)) {
	

	
	QVector<OutlineItem *> pageItems;
}

TocPrinter::~TocPrinter() {
  delete d;
}

/*!
  \brief Return the number of pages in the table of content
*/
int TocPrinter::pageCount() {
	return 0;
}

/*!
  \brief Output a given page to the printer
  \param The page to output, between 1 and pageCount()
*/
void TocPrinter::spoolPage(int page) {
	
}

/*!
  \brief Fill in document link structure with links linking back to the toc.
  \param doc The 0' indexed document to fill the structure for
  \param links The structure to fill
*/
void TocPrinter::fillLinks(int doc, QVector<QPair<QWebElement, QString> > & links) {
	if (doc < 0 || doc >= d->outline->d->documentOutlines.size()) return;
	d->revLinkChildren(d->outline->d->documentOutlines[doc], links);
}


#endif //__EXTENSIVE_WKHTMLTOPDF_QT_HACK__
