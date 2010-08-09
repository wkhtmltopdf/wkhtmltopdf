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

#ifdef __WKHTMLTOX_UNDEF_QT_DLL__
#ifdef QT_DLL
#undef QT_DLL
#endif
#endif

#include "utilities.hh"

#ifdef QT_STATIC
//When doing a static build, we need to load the plugins to make images work
Q_IMPORT_PLUGIN(qjpeg)
Q_IMPORT_PLUGIN(qgif)
Q_IMPORT_PLUGIN(qtiff)
Q_IMPORT_PLUGIN(qmng)
#endif

MyLooksStyle::MyLooksStyle(): weAreDrawingForms(false) {}

void MyLooksStyle::producingForms(bool f) {
	weAreDrawingForms=f;
}

void MyLooksStyle::drawPrimitive( PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget) const {
	painter->setBrush(Qt::white);
	painter->setPen(QPen(Qt::black, 0.7));
	QRect r = option->rect;
	if (element == QStyle::PE_PanelLineEdit) {
		painter->drawRect(r);
	} else if (element == QStyle::PE_IndicatorCheckBox) {
		painter->drawRect(r);
		if (!weAreDrawingForms && (option->state & QStyle::State_On)) {
			r.translate(int(r.width()*0.075), int(r.width()*0.075));
			painter->drawLine(r.topLeft(), r.bottomRight());
			painter->drawLine(r.topRight(), r.bottomLeft());
		}
	} else if (element == QStyle::PE_IndicatorRadioButton) {
		painter->drawEllipse(r);
		if (!weAreDrawingForms && (option->state & QStyle::State_On)) {
			r.translate(int(r.width()*0.20), int(r.width()*0.20));
			r.setWidth(int(r.width()*0.70));
			r.setHeight(int(r.height()*0.70));
			painter->setBrush(Qt::black);
			painter->drawEllipse(r);
		}
	} else {
		parent_t::drawPrimitive(element, option, painter, widget);
	}
}

int handleError(bool success, int errorCode) {
	QHash<int, const char *> cm;
	cm[400] = "Bad Request";
	cm[401] = "Unauthorized";
	cm[402] = "Payment Required";
	cm[403] = "Forbidden";
	cm[404] = "Page not found";
	cm[405] = "Method Not Allowed";
	cm[500] = "Internal Server Error";
	cm[501] = "Not Implemented";
	cm[503] = "Service Unavailable";
	cm[505] = "HTTP Version Not Supported";
	QHash<int, int> ce;
	ce[404] = 2;
	ce[401] = 3;
	if (errorCode) {
		int c = EXIT_FAILURE;
		if (ce.contains(errorCode)) c = ce[errorCode];
		const char * m = "";
		if (cm.contains(errorCode)) m = cm[errorCode];
		fprintf(stderr, "Exit with code %d due to http error: %d %s\n", c, errorCode, m);
		return c;
	}
	return success?EXIT_SUCCESS:EXIT_FAILURE;
}


// void printSupportedWriteformats() {
//   QList<QByteArray> formats = QImageWriter::supportedImageFormats();
//   for ( QList<QByteArray>::Iterator f = formats.begin(); f != formats.end(); ++ f )
//     printf("%s , ", f->constData() );
// }
