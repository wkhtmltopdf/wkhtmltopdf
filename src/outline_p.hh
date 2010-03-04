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
#ifndef __OUTLINE_P_HH
#define __OUTLINE_P_HH
#include "outline.hh"
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__

class OutlineItem {
public:
	QList<OutlineItem *> children;
	OutlineItem * parent;
	uint page;
	QString value;
	QWebElement element;
	QString anchor;
	OutlineItem();
	~OutlineItem();
};

class OutlinePrivate {
public:
	const Settings & settings;
	QList<OutlineItem *> documentOutlines;
	int pageCount;
	int anchorCounter;

	QList< QList< OutlineItem *> > hfCache;

	OutlinePrivate(const Settings & settings);
	~OutlinePrivate();
	void fillChildAnchors(OutlineItem * item, QHash<QString, QWebElement> & anchors);
	void outlineChildren(OutlineItem * item, QPrinter * printer, int level);
	void buildHFCache(OutlineItem * i, int level);
	void dumpOutlineChildren(OutlineItem * item, std::ofstream &dumpfile, int level);
};

#endif //__EXTENSIVE_WKHTMLTOPDF_QT_HACK__
#endif //__OUTLINE_P_HH
