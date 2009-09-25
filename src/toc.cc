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
#include "toc.hh"
#include <QObject>
#include <QtGui>
#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__

/*!
 * Build a toc structure out of headings elements
 * \param root The root node of the Toc tree
 * \param headings The headings extracted from the html document
 * \paran firstPage Offset numbering of the pages
 */
// void buildToc(TocItem * root, 
// 			  const QList<QWebElement> & headings, 
// 			  QHash<QString, QWebElement> & anchors,
// 			  QVector< QPair<QWebElement, QString> >  & links,
// 			  uint firstPage) {
// 	//This huristic is a little strange, it tries to create a real tree,
// 	//even though someone puts a h5 below a h1 or stuff like that
// 	//The way this is handled is having a level stack, indicating what h-tags
// 	//a level level in the tree currently represents
// 	QVector<uint> levelStack;
// 	levelStack.push_back(0);
// 	TocItem * old = root;
// 	for (QList<QWebElement>::const_iterator i=headings.begin(); i != headings.end(); ++i) {
// 		const QWebElement & h = *i;
		
// 		//Todo extract text;
// 		TocItem * item = new TocItem();
// 		//item->page = firstPage + h.page;
// 		//item->value = h.text;
// 		//item->location = h.locationOnPage;
// 		while (levelStack.back() >= h.level) {
// 			old = old->parent;
// 			levelStack.pop_back();
// 		}
// 		item->parent = old;
// 		old->children.push_back(item);
// 		levelStack.push_back(h.level);
// 		old = item;
// 	}
// }

// /*!
//  * Print the toc represeted by a subtree
//  * \param item The root of the subtree
//  * \param printer The printer to print on
//  * \param painter The painter to paint with
//  * \param dryRun Do not actualy print anything, just count the number of pages
//  * \param level What level are we currently at?
//  * \param page The current page
//  * \param y The y coordinate of printing on the current page
//  */
// void TocPrinter::printChildren(TocItem * item, QPrinter * printer, QPainter * painter, bool dryRun, uint level, uint & page, double & y) {
// 	painter->save();
// 	painter->setFont(QFont(font_name, font_size[level]));

// 	double step=painter->fontMetrics().height();
// 	QRect pr = printer->pageRect();
// 	double dw = painter->boundingRect(pr, Qt::AlignRight | Qt::AlignTop, ".").width();
// 	for (int i=0; i < item->children.size(); ++i) {
// 		if (y + step > pr.height()) {
// 			++page;
// 			y=0;
// 			if (!dryRun) {
// 				printer->newPage();
// 				//Print headers and footers
// 				emit printingNewPage(printer, 0, 0, page);
// 			}
// 		}
// 		if (!dryRun) {
// 			double startX = pr.width()*indentation[level]/1000.0;;
// 			QRectF lineRect(startX,y,pr.width()-startX,step);
// 			QRectF br;
// 			painter->drawText(lineRect,Qt::AlignBottom | Qt::AlignRight, QString::number(item->children[i]->page),&br);
// 			QString v =item->children[i]->value;
// 			if (useDots) {
// 				v += " ";
// 				//Calculate the number of dots needed to fill the line
// 				int ndots = (br.left() - startX - painter->boundingRect(lineRect, Qt::AlignLeft, v+" ").width())/dw;
// 				for (int i=0; i < ndots; ++i) v += ".";
// 			}
// 			painter->drawText(lineRect,Qt::AlignBottom | Qt::AlignLeft, v);
// 		}
// 		y += step;
// 		//If it is not too deep, print my children
// 		if (item->children[i]->children.size() && level+1 < (uint)depth)
// 			printChildren(item->children[i], printer, painter, dryRun, level+1, page, y);
// 	}
// 	painter->restore();
// }

// /*!
//  * Print the toc
//  * \param root The root of the toc tree
//  * \param printer The printer to print on
//  * \param painter The painter to paint with
//  * \param dryRun Do not actualy print anything, just count the pages
//  */
// uint TocPrinter::print(TocItem * root, QPrinter * printer, QPainter * painter, bool dryRun) {
// 	if (!dryRun)
// 		emit printingNewPage(printer, 0, 0, 0);
// 	painter->save();
// 	painter->resetTransform();
// 	painter->setFont(QFont(font_name, header_font_size));
// 	QRect pr = printer->pageRect();
// 	double h = painter->boundingRect(pr,Qt::AlignTop | Qt::AlignHCenter, header_text).height();
// 	QRect r((int)h,0,pr.width(),(int)h*3);
// 	if (!dryRun) {
// 		painter->drawText(r,Qt::AlignVCenter | Qt::AlignHCenter, header_text);
// 	}
// 	uint page = 0;
// 	double y = r.height();
// 	//Todo print Title here
// 	printChildren(root,printer,painter,dryRun, 0, page, y);
// 	painter->restore();
// 	return page+1;
// }

// void TocPrinter::outlineChildren(TocItem * item, QPrinter * printer, int level) {
// 	if (level >= outline_depth) return;
// 	for (int i=0; i < item->children.size(); ++i) {
// 		TocItem * j = item->children[i];
// 		printer->printEngine()->beginSectionOutline(j->value,j->page-1,j->location);
// 		outlineChildren(j,printer,level+1);
// 		printer->printEngine()->endSectionOutline();
// 	}
// }

// /**
//  * Fill in structure telling on which page a toc item begin
//  */
// void TocPrinter::populateSectionsChildren(TocItem * item, int level) {
// 	for (int i=0; i < item->children.size(); ++i) {
// 		TocItem * j = item->children[i];
// 		if (page2sectionslow[level].count(j->page) == 0
// 		        || page2sectionslow[level][j->page]->location.y() > j->location.y())
// 			page2sectionslow[level][j->page] = j;
// 		if (page2sectionshigh[level].count(j->page) == 0
// 		        || page2sectionshigh[level][j->page]->location.y() < j->location.y())
// 			page2sectionshigh[level][j->page] = j;
// 		populateSectionsChildren(j,level+1);
// 	}
// }
#endif
