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


#include "utilities.hh"
#include <QDebug>
#include <QTextStream>
#include <QMetaEnum>
#include <QNetworkReply>

void loadSvg(QSvgRenderer * & ptr, const QString & path, const char * def, int w, int h) {
	 delete ptr;
	 ptr = 0;
	 if (path != "") {
	 	ptr = new QSvgRenderer(path);
		if (ptr->isValid()) return;
		qWarning() << "Failed to load " << path;
		delete ptr;
	 }

	 QByteArray a;
	 QTextStream s(&a, QIODevice::WriteOnly );
	 s << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
	   << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
	   << "<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
	   << "  xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
	   << "  version=\"1.1\"\n"
	   << "  baseProfile=\"full\"\n"
	   << "  width=\"" << w << "px\"\n"
	   << "  height=\"" << h << "px\"\n"
	   << "  viewBox=\"0 0 " << w << "px " << h << "px\">\n"
	   << def
	   << "</svg>\n";
	 s.flush();
	 ptr = new QSvgRenderer(a);

	 if (ptr->isValid()) return;
	 delete ptr;
	 ptr = 0;
}

#define RB \
"<path d=\"M0,0 h-1 a1,1 0 0,0 2,0 z\" fill=\"#808080\" stroke=\"\" stroke-width=\"0\" transform=\"translate(5.5,5.5)rotate(-225)scale(5.5,5.5)\"/>\n" \
"<path d=\"M0,0 h-1 a1,1 0 0,0 2,0 z\" fill=\"#ffffff\" stroke=\"\" stroke-width=\"0\" transform=\"translate(5.5,5.5)rotate(-45)scale(5.5,5.5)\"/>\n" \
"<path d=\"M0,0 h-1 a1,1 0 0,0 2,0 z\" fill=\"#404040\" stroke=\"\" stroke-width=\"0\" transform=\"translate(5.5,5.5)rotate(-225)scale(4.5,4.5)\"/>\n" \
 "<path d=\"M0,0 h-1 a1,1 0 0,0 2,0 z\" fill=\"#d4d0c8\" stroke=\"\" stroke-width=\"0\" transform=\"translate(5.5,5.5)rotate(-45)scale(4.5,4.5)\"/>\n" \
"<circle id=\"c1\" cx=\"5.5\" cy=\"5.5\" r=\"3.5\" fill=\"white\" stroke=\"\" stroke-width=\"0\"/>\n"

#define CB \
"<rect x=\"0\" y=\"0\" width=\"1\" height=\"12\" fill=\"#808080\" stroke=\"\" stroke-width=\"0\" />\n" \
"<rect x=\"1\" y=\"0\" width=\"11\" height=\"1\" fill=\"#808080\" stroke=\"\" stroke-width=\"0\" />\n" \
"<rect x=\"1\" y=\"11\" width=\"11\" height=\"1\" fill=\"#d4d0c8\" stroke=\"\" stroke-width=\"0\" />\n" \
"<rect x=\"11\" y=\"1\" width=\"1\" height=\"11\" fill=\"#d4d0c8\" stroke=\"\" stroke-width=\"0\" />\n" \
"<rect x=\"1\" y=\"1\" width=\"1\" height=\"10\" fill=\"#404040\" stroke=\"\" stroke-width=\"0\" />\n" \
"<rect x=\"2\" y=\"1\" width=\"9\" height=\"1\" fill=\"#404040\" stroke=\"\" stroke-width=\"0\" />\n" \
"<rect x=\"2\" y=\"10\" width=\"9\" height=\"1\" fill=\"#ffffff\" stroke=\"\" stroke-width=\"0\" />\n" \
"<rect x=\"10\" y=\"2\" width=\"1\" height=\"9\" fill=\"#ffffff\" stroke=\"\" stroke-width=\"0\" />\n" \
"<rect x=\"2\" y=\"2\" width=\"8\" height=\"8\" fill=\"#ffffff\" stroke=\"\" stroke-width=\"0\" />\n"

void MyLooksStyle::setCheckboxSvg(const QString & path) {
	loadSvg(checkbox, path,
			CB, 12, 12);
}
void MyLooksStyle::setCheckboxCheckedSvg(const QString & path) {
	loadSvg(checkbox_checked, path,
			CB
			"<path d=\"M 3 5.5 L 3 8 L 5.5 10.5 L 10 5.5 L 10 2.5 L 5.5 7.5\" fill=\"black\" />\n",
			12, 12);
}
void MyLooksStyle::setRadioButtonSvg(const QString & path) {
	loadSvg(radiobutton, path, RB, 11, 11);
}
void MyLooksStyle::setRadioButtonCheckedSvg(const QString & path) {
	loadSvg(radiobutton_checked, path,
			RB
			"<circle id=\"c2\" cx=\"5.5\" cy=\"5.5\" r=\"1.5\" fill=\"black\" stroke=\"\" stroke-width=\"0\"/>\n", 11, 11);
}

MyLooksStyle::MyLooksStyle(): weAreDrawingForms(false) {
}

void MyLooksStyle::producingForms(bool f) {weAreDrawingForms=f;}

void MyLooksStyle::drawPrimitive( PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget) const {
	painter->setBrush(Qt::white);
	painter->setPen(QPen(Qt::black, 0.7));
	painter->setBackground(Qt::NoBrush);
	painter->setBackgroundMode(Qt::TransparentMode);
	QRect r = option->rect;
	if (element == QStyle::PE_PanelLineEdit) {
		painter->drawRect(r);
	} else if (element == QStyle::PE_IndicatorCheckBox) {
		if (weAreDrawingForms || ((option->state & QStyle::State_On)? !checkbox_checked: !checkbox)) {
			painter->drawRect(r);
			if (!weAreDrawingForms && (option->state & QStyle::State_On)) {
				r.translate(int(r.width()*0.075), int(r.width()*0.075));
				painter->drawLine(r.topLeft(), r.bottomRight());
				painter->drawLine(r.topRight(), r.bottomLeft());
			}
		} else if (option->state & QStyle::State_On)
			checkbox_checked->render(painter, r);
		else
			checkbox->render(painter, r);
	} else if (element == QStyle::PE_IndicatorRadioButton) {
		if (weAreDrawingForms || ((option->state & QStyle::State_On)? !radiobutton_checked: !radiobutton)) {
			painter->drawEllipse(r);
			if (!weAreDrawingForms && (option->state & QStyle::State_On)) {
				r.translate(int(r.width()*0.20), int(r.width()*0.20));
				r.setWidth(int(r.width()*0.70));
				r.setHeight(int(r.height()*0.70));
				painter->setBrush(Qt::black);
				painter->drawEllipse(r);
			}
		} else if (option->state & QStyle::State_On)
			radiobutton_checked->render(painter, r);
		else
			radiobutton->render(painter, r);
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
		if (errorCode < 1000) {
			fprintf(stderr, "Exit with code %d due to http error: %d %s\n", c, errorCode, m);
		} else {
			QNetworkReply::NetworkError error = (QNetworkReply::NetworkError)(errorCode - 1000);
			QString errorValue;
			QMetaObject meta = QNetworkReply::staticMetaObject;
			for (int i=0; i < meta.enumeratorCount(); ++i) {
				QMetaEnum m = meta.enumerator(i);
				if (m.name() == QLatin1String("NetworkError")) {
					errorValue = QLatin1String(m.valueToKey(error));
					break;
				}
			}
			fprintf(stderr, "Exit with code %d due to network error: %s\n", c, errorValue.toLocal8Bit().data());
		}
		return c;
	} else if (!success) {
		fprintf(stderr, "Exit with code %d, due to unknown error.\n", EXIT_FAILURE);
	}
	return success?EXIT_SUCCESS:EXIT_FAILURE;
}

QSvgRenderer * MyLooksStyle::checkbox = 0;
QSvgRenderer * MyLooksStyle::checkbox_checked = 0;
QSvgRenderer * MyLooksStyle::radiobutton = 0;
QSvgRenderer * MyLooksStyle::radiobutton_checked = 0;
