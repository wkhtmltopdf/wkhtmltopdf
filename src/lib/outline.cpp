// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010 wkhtmltopdf authors
//
// This file is part of wkhtmltopdf.
//
// wkhtmltopdf is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// wkhtmltopdf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with wkhtmltopdf.  If not, see <http://www.gnu.org/licenses/>.


#include "outline_p.hh"
#include <fstream>
#include <iostream>
using namespace std;
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__

namespace wkhtmltopdf {
/*!
  \file outline_p.hh
  \brief Defines the classes OutlinePrivate and OutlineItem
*/

/*!
  \class OutlineItem
  \brief Class describing an item in the outline
*/

OutlineItem::OutlineItem():
	parent(NULL), page(-1), display(true), forwardLinks(false), backLinks(false) {}

/*!
  \brief Recursivily delete the subtree
*/
OutlineItem::~OutlineItem() {
	foreach (OutlineItem * i, children)
		delete i;
}

void OutlineItem::fillAnchors(const OutlineItem * other,
							  int & anchorCounter,
							  QVector<QPair<QWebElement, QString> > & local,
							  QHash<QString, QWebElement> & anchors) {
	if (!other ||
		other->children.size() != children.size() ||
		other->document != document ||
		other->value != value ||
		other->display != display) other=0;

	if (other) {
		anchor = other->anchor;
		tocAnchor = other->anchor;
	} else {
		anchor = QString("__WKANCHOR_")+QString::number(anchorCounter++,36);
		tocAnchor = QString("__WKANCHOR_")+QString::number(anchorCounter++,36);
	}

	if (forwardLinks)
		anchors[anchor] = element;
	if (backLinks)
		local.push_back( QPair<QWebElement, QString>(element, tocAnchor) );

	for (int i=0; i < children.size(); ++i)
		children[i]->fillAnchors(other?other->children[i]:0, anchorCounter, local, anchors);
}

bool OutlineItem::differentFrom(const OutlineItem * other) const {
	if (other->children.size() != children.size() ||
		other->page != page ||
		other->document != document ||
		other->value != value ||
		other->display != display) return true;

	for (int i=0; i < children.size(); ++i)
		if (children[i]->differentFrom(other->children[i]))
			return true;
	return false;
}


/*!
  \class OutlinePrivate
  \brief Class providing implementation details of Outline
*/

OutlinePrivate::OutlinePrivate(const settings::PdfGlobal & s):
	settings(s), pageCount(0), anchorCounter(0) {
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

void OutlinePrivate::outlineChildren(OutlineItem * item, QPrinter * printer, int level) {
	if (level + 1 > settings.outlineDepth) return;
	foreach (OutlineItem * i, item->children) {
		printer->beginSectionOutline(i->value, i->anchor);
		outlineChildren(i, printer, level+1);
		printer->endSectionOutline();
	}
}

QString escape(QString str) {
	return str.replace('&',"&amp;")
		.replace('<',"&lt;")
		.replace('>',"&gt;")
		.replace('"',"&quot;")
		.replace('\'',"&apos;");
}

void OutlinePrivate::dumpChildren(QTextStream & stream, const QList<OutlineItem *> & items, int level) const {
	foreach (OutlineItem * item, items) {
		for (int i=0; i < level; ++i) stream << "  ";
		stream << "<item title=\"" << escape(item->value) << "\" page=\"" << (item->page + prefixSum[item->document]+ settings.pageOffset) << "\" link=\"" << escape(item->anchor) << "\" backLink=\"" << escape(item->tocAnchor) << "\"";
		if (item->children.empty())
			stream << "/>" << endl;
		else {
			stream << ">" << endl;
			dumpChildren(stream, item->children, level+1);
			for (int i=0; i < level; ++i) stream << "  ";
			stream << "</item>" << endl;
		}
	}
}

void OutlinePrivate::buildPrefixSum() {
	prefixSum.clear();
	prefixSum.push_back(0);
	foreach (int x, documentPages)
		prefixSum.push_back( prefixSum.back() + x);
}

void Outline::dump(QTextStream & stream) const {
	d->buildPrefixSum();
	stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	stream << "<outline xmlns=\"http://wkhtmltopdf.org/outline\">" << endl;
	d->dumpChildren(stream, d->documentOutlines, 1);
	stream << "</outline>" << endl;
}

/*!
  \file outline.hh
  \brief Defines the Outline class
*/

/*!
  \class Outline
  \brief Class responsible for building and keeping an outline of a document.
*/

/*!
  \brief Construct a new outline class
  \param settings The settings to use
*/
Outline::Outline(const settings::PdfGlobal & settings): d(new OutlinePrivate(settings)) {}
Outline::~Outline() {delete d;}


/*!
  \brief Replace a webpage in the outline
  \param document The number of the webpage
  \param name The name of the webpage
  \param wp A webprinter for the page
  \param frame The frame containing the webpage
*/
bool Outline::replaceWebPage(int document,
							 const QString & name,
							 QWebPrinter & wp,
							 QWebFrame * frame,
							 const settings::PdfObject & ps,
							 QVector<QPair<QWebElement, QString> > & local,
							 QHash<QString, QWebElement> & anchors) {
	QMap< QPair<int, QPair<qreal,qreal> >, QWebElement> headings;
	foreach (const QWebElement & e, frame->findAllElements("h1,h2,h3,h4,h5,h6,h7,h8,h9")) {
		QPair<int, QRectF> location = wp.elementLocation(e);
		headings[ qMakePair(location.first, qMakePair(location.second.y(), location.second.x()) ) ] = e;
	}

	//This heuristic is a little strange, it tries to create a real tree,
	//even though someone puts a h5 below a h1 or stuff like that
	//The way this is handled is having a level stack, indicating what h-tags
	//a level level in the tree currently represents
	QVector<uint> levelStack;
	levelStack.push_back(0);
	OutlineItem * root = new OutlineItem();
	root->page = 0;
	root->document = document;
	root->value = name;
	root->display = true;

	OutlineItem * old = root;
	for (QMap< QPair<int, QPair<qreal,qreal> >, QWebElement>::iterator i = headings.begin();
		i != headings.end(); ++i) {
		const QWebElement & element = i.value();
		uint level = element.tagName().mid(1).toInt();
		QString value = element.toPlainText().replace("\n", " ").trimmed();
		if (i.key().first == -1 || value == "") continue;
		OutlineItem * item = new OutlineItem();
		item->page = i.key().first;
		item->document = document;
		item->value = value;
		item->element = element;
		item->forwardLinks = ps.toc.forwardLinks;
		item->backLinks = ps.toc.backLinks;

		while (levelStack.back() >= level) {
			old = old->parent;
			levelStack.pop_back();
		}
		item->parent = old;
		old->children.push_back(item);
		old = item;
		levelStack.push_back(level);
	}

	root->fillAnchors(d->documentOutlines[document], d->anchorCounter, local, anchors);
	bool changed=d->documentOutlines[document]->differentFrom(root);
	delete d->documentOutlines[document];
	d->documentOutlines[document] = root;

	if (d->documentPages[document] != wp.pageCount()) {
		d->pageCount -= d->documentPages[document];
		d->documentPages[document] = wp.pageCount();
		d->pageCount += d->documentPages[document];
		changed=true;
	}
	return changed;
}

/*!
  \brief Add a new webpage to the outline
  \param name The name of the webpage
  \param wp A webprinter for the page
  \param frame The frame containing the webpage
*/
void Outline::addWebPage(const QString & name, QWebPrinter & wp, QWebFrame * frame, const settings::PdfObject & ps,
						 QVector<QPair<QWebElement, QString> > & local,
						 QHash<QString, QWebElement> & anchors) {
	Q_UNUSED(name);
	addEmptyWebPage();
	replaceWebPage(d->documentOutlines.size()-1, name, wp, frame, ps, local, anchors);
}


void Outline::addEmptyWebPage() {
	OutlineItem * root = new OutlineItem();
	root->page = 0;
	root->document = d->documentPages.size();
	root->value = "";
	root->display = true;
	d->documentOutlines.push_back(root);
	d->pageCount += 1;
	d->documentPages.push_back(1);
}

void OutlinePrivate::buildHFCache(OutlineItem * i, int level) {
	buildPrefixSum();
	if (level >= hfCache.size()) return;
	foreach (OutlineItem * j, i->children) {
		int page = j->page + prefixSum[j->document];
		while (hfCache[level].size() < page)
			hfCache[level].push_back(hfCache[level].back());
		if (hfCache[level].size() == page)
			hfCache[level].push_back(j);
		buildHFCache(j, level+1);
	}
}


/*!
  \brief Fill in header footer parameters for a given page
  \param page The page to fill in for
  \param parms The structure to fill
 */
void Outline::fillHeaderFooterParms(int page, QHash<QString, QString> & parms, const settings::PdfObject & ps) {
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
	foreach (const SP & rep, ps.replacements)
		parms[rep.first] = rep.second;

	parms["frompage"] = QString::number(off+1);
	parms["topage"] = QString::number(off+d->pageCount);
	parms["page" ] = QString::number(page+off);
	parms["webpage"] = ps.page;
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
	foreach (OutlineItem * i, d->documentOutlines)
		d->outlineChildren(i, printer, 0);
}

}
#endif //__EXTENSIVE_WKHTMLTOPDF_QT_HACK__
