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
#include "outline_p.hh"
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__

/*!
  \file outline_p.hh
  \brief Defines the classes OutlinePrivate and OutlineItem
*/

/*!
  \class OutlineItem
  \brief Class describing an item in the outline
*/

OutlineItem::OutlineItem():
	parent(NULL), page(-1)
{}

/*!
  \brief Recursivily delete the subtree
*/
OutlineItem::~OutlineItem() {
	foreach (OutlineItem * i, children)
		delete i;
}

/*!
  \class OutlinePrivate
  \brief Class providing implemenation details of Outline
*/

OutlinePrivate::OutlinePrivate(const Settings & s):
	settings(s), pageCount(s.pageOffset-1), anchorCounter(0) {
}

OutlinePrivate::~OutlinePrivate() {
	foreach (OutlineItem * i, documentOutlines) 
		delete i;
}

void OutlinePrivate::fillChildAnchors(OutlineItem * item, QHash<QString, QWebElement> & anchors) {
	foreach (OutlineItem * i, item->children) {
		if (i->anchor.isEmpty()) continue;
		anchors[i->anchor] = i->element;
		fillChildAnchors(i, anchors);
	}	
}

#include <iostream>
using namespace std;
void OutlinePrivate::outlineChildren(OutlineItem * item, QPrinter * printer, int level) {
	if (level + 1 > settings.outlineDepth) return;
	foreach (OutlineItem * i, item->children) {
		printer->beginSectionOutline(i->value, i->anchor);
		outlineChildren(i, printer, level+1);
		printer->endSectionOutline();
	}	
}


/*!
  \file outline.hh
  \brief Defiens the Outline class
*/

/*!
  \class Outline
  \brief Class responcible for building and keeping an outline of a document.
*/

/*!
  \brief Construct a new outline class
  \param settings The settings to use
*/
Outline::Outline(const Settings & settings): d(new OutlinePrivate(settings)) {}
Outline::~Outline() {delete d;}

/*!
  \brief Add a new webpage to the outline
  \param name The name of the webpage
  \param wp A webprinter for the page
  \param frame The frame containing the webpage
*/
void Outline::addWebPage(const QString & name, QWebPrinter & wp, QWebFrame * frame) {
	Q_UNUSED(name);
	QMap< QPair<int, QPair<qreal,qreal> >, QWebElement> headings;
	
	foreach(const QWebElement & e, frame->findAllElements("h1,h2,h3,h4,h5,h6,h7,h8,h9")) {
		QPair<int, QRectF> location = wp.elementLocation(e);
		headings[ qMakePair(location.first, qMakePair(location.second.y(), location.second.x()) ) ] = e;
	}

	//This huristic is a little strange, it tries to create a real tree,
	//even though someone puts a h5 below a h1 or stuff like that
	//The way this is handled is having a level stack, indicating what h-tags
	//a level level in the tree currently represents
	QVector<uint> levelStack;
	levelStack.push_back(0);
	OutlineItem * root = new OutlineItem();
	OutlineItem * old = root;
	for(QMap< QPair<int, QPair<qreal,qreal> >, QWebElement>::iterator i = headings.begin(); i != headings.end(); ++i) {
		const QWebElement & element = i.value();
		
		uint level = element.tagName().mid(1).toInt();
		OutlineItem * item = new OutlineItem();
		item->page = d->pageCount + i.key().first;
		item->value = element.toPlainText().replace("\n", " ");
		item->element = element;
		item->anchor = QString("__WKANCHOR_")+QString::number(d->anchorCounter++,36);
		while(levelStack.back() >= level) {
			old = old->parent;
			levelStack.pop_back();
		}
		item->parent = old;
		old->children.push_back(item);
		old = item;
		levelStack.push_back(level);
	}
	d->documentOutlines.push_back(root);
	d->pageCount += wp.pageCount();
}


void OutlinePrivate::buildHFCache(OutlineItem * i, int level) {
	if (level >= hfCache.size()) return;
	foreach (OutlineItem * j, i->children) {
		while (hfCache[level].size() < (int)j->page)
			hfCache[level].push_back(hfCache[level].back());
		
		if (hfCache[level].size() == (int)j->page) 
			hfCache[level].push_back(j);
		buildHFCache(j, level+1);
	}
}



/*!
  \brief Fill in header footer parameters for a given page
  \param page The page to fill in for
  \param parms The structure to fill
 */
void Outline::fillHeaderFooterParms(int page, QHash<QString, QString> & parms) {
	//Build hfcache
	if (d->hfCache.size() == 0) {
		for (int i=0; i < 3; ++i) {
			QList< OutlineItem *> x;
			x.push_back(NULL);
			d->hfCache.push_back(x);
		}
		foreach (OutlineItem * i, d->documentOutlines) 
			d->buildHFCache(i, 0);
	}
	for (int i=0; i < 3; ++i)
		while (d->hfCache[i].size() <= page) 
			d->hfCache[i].push_back(d->hfCache[i].back());

	int off = d->settings.pageOffset;
	typedef QPair<QString,QString> SP;
	foreach (const SP & rep, d->settings.replacements) 
		parms[rep.first] = rep.second;
		
	parms["frompage"] = QString::number(off);
	parms["topage"] = QString::number(off+d->pageCount-1);
	parms["page" ] = QString::number(page+off-1);
	parms["webpage"] = "foobar";

	parms["section" ] = d->hfCache[0][page]?d->hfCache[0][page]->value:QString("");
	parms["subsection" ] = d->hfCache[1][page]?d->hfCache[1][page]->value:QString("");
	parms["subsubsection" ] = d->hfCache[2][page]?d->hfCache[2][page]->value:QString("");
}

/*!
  \brief Fill in anchor as to add to a given document
  \param doc The 0 indexed document number (in order of addWebPage)
  \param anchors The structure to fill
*/
void Outline::fillAnchors(int doc, QHash<QString, QWebElement> & anchors) {
  if (doc < 0 || doc >= d->documentOutlines.size()) return;
  d->fillChildAnchors( d->documentOutlines[doc], anchors );
}

/*!
  \brief return the number of pages in the outlined document
*/
int Outline::pageCount() {
	return d->pageCount;
}

/*!
  \brief Print the document outline to a given printer
  \param printer The printer to print to
*/
void Outline::printOutline(QPrinter * printer) {
	if (!d->settings.outline) return;
	foreach(OutlineItem * i, d->documentOutlines)
		d->outlineChildren(i, printer, 0);
}
#endif //__EXTENSIVE_WKHTMLTOPDF_QT_HACK__
