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


typedef QPair<int, OutlineItem *> line_t;
/*!
  We imploy a cheep strategy of not using a render tree, 
  insted we just store the state render state for every page start,
  the render state is uniquly defined by a OutlineItem, 
 */
class TocPrinterPrivate {
public:
	static const uint levels = Settings::TOCSettings::levels;

	Outline * outline;
	QPrinter * printer;
	QPainter & painter;
	double dw[levels];
	double step[levels];
	QVector<OutlineItem *> pageItems;

	QList< QList< line_t > > pages;
	
	TocPrinterPrivate(Outline * o, QPrinter * pr, QPainter & pa);
	void revLinkChildren(OutlineItem * i, QVector<QPair<QWebElement, QString> > & links, int level);
	void renderPage(OutlineItem * & cur, bool first, bool dry);
	void layoutChildren(OutlineItem * item, double & y, int level);
	void incChildren(OutlineItem * item);
};


void TocPrinterPrivate::incChildren(OutlineItem * item) {
	foreach (OutlineItem * i, item->children) {
		i->page += pages.size();
		incChildren(i);
	}
}

void TocPrinterPrivate::layoutChildren(OutlineItem * item, double & y, int level) {
	const Settings::TOCSettings & s = outline->d->settings.toc;
	if (level >= s.depth) return;
	QRect pr = printer->pageRect();
	foreach (OutlineItem * i, item->children) {
		y += step[level];
		if(y > pr.height()) {
			pages.push_back( QList< QPair<int, OutlineItem *> >() );
			y = step[level];
		}
		pages.back().push_back( qMakePair(level, i) );
		layoutChildren(i, y, level+1);
	}
}

TocPrinterPrivate::TocPrinterPrivate(Outline * o, QPrinter * pr, QPainter & pa):
	outline(o), printer(pr), painter(pa) {
	const Settings::TOCSettings & s = outline->d->settings.toc;
	painter.save();
	painter.resetTransform();
	QRectF pageRect = printer->pageRect();
	for(uint level=0; level < levels; ++level) {
		painter.setFont(QFont(s.fontName, s.fontSize[level]));
		step[level] = painter.fontMetrics().height();
		dw[level] = painter.boundingRect(pageRect, Qt::AlignRight | Qt::AlignTop, ".").width();
	}
	painter.setFont(QFont(s.fontName, s.captionFontSize));
	//The height of the caption	
	double y = painter.boundingRect(pageRect, Qt::AlignTop | Qt::AlignHCenter, s.captionText).height() * 3;
	painter.restore();
	pages.push_back( QList< QPair<int, OutlineItem *> >() );

	OutlineItem * root = new OutlineItem();
	OutlineItem * toc = new OutlineItem();
	root->children.push_back(toc);
	toc->value = s.captionText;
	toc->anchor = "_WK_TOC";
	toc->parent = root;
	toc->page = outline->d->settings.pageOffset;

	y += step[0];
	pages.back().push_back( qMakePair(0, toc) );

	foreach (OutlineItem * i, o->d->documentOutlines)
		layoutChildren(i, y, 0); 	

	outline->d->pageCount += pages.size();

	foreach (OutlineItem * i, outline->d->documentOutlines) 
		incChildren(i);

	outline->d->documentOutlines.push_front(root);
}


void TocPrinterPrivate::revLinkChildren(OutlineItem * i, QVector<QPair<QWebElement, QString> > & links, int level) {
	const Settings::TOCSettings & s = outline->d->settings.toc;
	if (level >= s.depth) return;
	foreach (OutlineItem * j,i->children) {
		links.push_back( qMakePair(j->element, j->anchor+QString("_R") ) );
		revLinkChildren(j, links, level+1);
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
}

TocPrinter::~TocPrinter() {
  delete d;
}

/*!
  \brief Return the number of pages in the table of content
*/
int TocPrinter::pageCount() {
	return d->pages.size();
}

/*!
  \brief Output a given page to the printer
  \param The page to output, between 1 and pageCount()
*/
void TocPrinter::spoolPage(int page) {
	d->painter.save();
	d->painter.resetTransform();
	d->painter.setBackgroundMode(Qt::TransparentMode);
	d->painter.setBrush(QBrush(Qt::black));

	double y = 0;
	const Settings::TOCSettings & s = d->outline->d->settings.toc;
	QRect pr = d->printer->pageRect();
	
	if(page == 0) {
		double h = d->painter.boundingRect(pr,Qt::AlignTop | Qt::AlignHCenter, s.captionText).height();
		QRect r((int)h,0,pr.width(),(int)h*3);
		
		d->painter.setFont(QFont(s.captionFontName.isEmpty()?s.fontName:s.captionFontName, s.captionFontSize));
		d->painter.drawText(r, Qt::AlignVCenter | Qt::AlignHCenter, s.captionText);
		d->painter.addAnchor(r, "_WK_TOC");
		y += r.height();
	}

	int lvl=-1;
	foreach (const line_t & line, d->pages[page]) {
		if (line.first != lvl) {
			lvl = line.first;
			d->painter.setFont(QFont(s.fontName, s.fontSize[lvl]));
		}

		double startX = pr.width()*s.indentation[lvl]/1000.0;
		
		QRectF lineRect(startX,y,pr.width()-startX, d->step[lvl]);
		
		QRectF br;
		d->painter.drawText(lineRect,Qt::AlignBottom | Qt::AlignRight, QString(" ") + QString::number(line.second->page),&br);
		QString v = line.second->value;
		lineRect.setRight(br.left());
		if (s.useDots) {
			v.append(' ');
			//Calculate the number of dots needed to fill the line
			int ndots = (lineRect.right() - d->painter.boundingRect(lineRect, Qt::AlignLeft, v).right())/d->dw[lvl];
			for (int i=0; i < ndots; ++i) v.append(".");
		}
		d->painter.drawText(lineRect,Qt::AlignBottom | Qt::AlignLeft, v);
		
		QRectF r(0,y, pr.width(), d->step[lvl]);
		if (s.forwardLinks)
			d->painter.addLink(r, line.second->anchor);
		if (s.backLinks)
			d->painter.addAnchor(r, line.second->anchor+"_R");
		y += d->step[lvl];
	}
	d->painter.restore();
}

/*!
  \brief Fill in document link structure with links linking back to the toc.
  \param doc The 0' indexed document to fill the structure for
  \param links The structure to fill
*/
void TocPrinter::fillLinks(int doc, QVector<QPair<QWebElement, QString> > & links) {
	if (doc < 0 || doc >= d->outline->d->documentOutlines.size()) return;
	d->revLinkChildren(d->outline->d->documentOutlines[doc], links, 0);
}


#endif //__EXTENSIVE_WKHTMLTOPDF_QT_HACK__
