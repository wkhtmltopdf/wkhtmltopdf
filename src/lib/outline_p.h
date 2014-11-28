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

#ifndef __OUTLINE_P_HH__
#define __OUTLINE_P_HH__

#include "outline.hh"
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
#include "dllbegin.inc"
namespace wkhtmltopdf {

class DLL_LOCAL OutlineItem {
public:
	QList<OutlineItem *> children;
	OutlineItem * parent;
	uint page;
	uint document;
	QString value;
	QWebElement element;
	QString anchor;
	QString tocAnchor;
	bool display;
	bool forwardLinks;
	bool backLinks;
	void fillAnchors(const OutlineItem * other,
					 int & anchorCounter,
					 QVector<QPair<QWebElement, QString> > & local,
					 QHash<QString, QWebElement> & anchors);
	bool differentFrom(const OutlineItem * other) const;
	OutlineItem();
	~OutlineItem();
};

class DLL_LOCAL OutlinePrivate {
public:
	const settings::PdfGlobal & settings;
	QList<OutlineItem *> documentOutlines;
	QList<int> documentPages;
	QList<int> prefixSum;
	int pageCount;
	int anchorCounter;

	QList< QList< OutlineItem *> > hfCache;

	OutlinePrivate(const settings::PdfGlobal & settings);
	~OutlinePrivate();
	void buildPrefixSum();
	void fillChildAnchors(OutlineItem * item, QHash<QString, QWebElement> & anchors);
	void outlineChildren(OutlineItem * item, QPrinter * printer, int level);
	void buildHFCache(OutlineItem * i, int level);
	void dumpChildren(QTextStream & stream, const QList<OutlineItem *> & items, int level) const;
};

#include "dllend.inc"
}
#endif //__EXTENSIVE_WKHTMLTOPDF_QT_HACK__
#endif //__OUTLINE_P_HH__
