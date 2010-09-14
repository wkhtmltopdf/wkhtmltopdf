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

MyLooksStyle::MyLooksStyle(): weAreDrawingForms(false) {
	if (checkbox == 0)
		checkbox = new QSvgRenderer(QByteArray(
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
"<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
"     version=\"1.1\" baseProfile=\"full\" width=\"12px\" height=\"12px\" viewBox=\"0 0 12px 12px\">\n"
"  <rect x=\"0\" y=\"0\" width=\"1\" height=\"12\" fill=\"#808080\" />\n"
"  <rect x=\"1\" y=\"0\" width=\"11\" height=\"1\" fill=\"#808080\" />\n"
"  <rect x=\"1\" y=\"11\" width=\"11\" height=\"1\" fill=\"#d4d0c8\" />\n"
"  <rect x=\"11\" y=\"1\" width=\"1\" height=\"11\" fill=\"#d4d0c8\" />\n"
"  <rect x=\"1\" y=\"1\" width=\"1\" height=\"10\" fill=\"#404040\" />\n"
"  <rect x=\"2\" y=\"1\" width=\"9\" height=\"1\" fill=\"#404040\" />\n"
"  <rect x=\"2\" y=\"10\" width=\"9\" height=\"1\" fill=\"white\" />\n"
"  <rect x=\"10\" y=\"2\" width=\"1\" height=\"9\" fill=\"white\" />\n"
"  <rect x=\"2\" y=\"2\" width=\"8\" height=\"8\" fill=\"white\" />\n"
"</svg>\n"));
	if (checkbox_checked == 0)
		checkbox_checked = new QSvgRenderer(QByteArray(
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
"<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
"     xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
"     version=\"1.1\" baseProfile=\"full\" width=\"12px\" height=\"12px\" viewBox=\"0 0 12px 12px\">\n"
"  <rect x=\"0\" y=\"0\" width=\"1\" height=\"12\" fill=\"#808080\" />\n"
"  <rect x=\"1\" y=\"0\" width=\"11\" height=\"1\" fill=\"#808080\" />\n"
"  <rect x=\"1\" y=\"11\" width=\"11\" height=\"1\" fill=\"#d4d0c8\" />\n"
"  <rect x=\"11\" y=\"1\" width=\"1\" height=\"11\" fill=\"#d4d0c8\" />\n"
"  <rect x=\"1\" y=\"1\" width=\"1\" height=\"10\" fill=\"#404040\" />\n"
"  <rect x=\"2\" y=\"1\" width=\"9\" height=\"1\" fill=\"#404040\" />\n"
"  <rect x=\"2\" y=\"10\" width=\"9\" height=\"1\" fill=\"white\" />\n"
"  <rect x=\"10\" y=\"2\" width=\"1\" height=\"9\" fill=\"white\" />\n"
"  <rect x=\"2\" y=\"2\" width=\"8\" height=\"8\" fill=\"white\" />\n"
"  <path d=\"M 3 5.5 L 3 8 L 5.5 10.5 L 10 5.5 L 10 2.5 L 5.5 7.5\" fill=\"black\" />\n"
"</svg>\n"));
	if (radiobutton == 0)
		radiobutton = new QSvgRenderer(QByteArray(
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
"<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
"     xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
"     version=\"1.1\" baseProfile=\"full\" width=\"11px\" height=\"11px\" viewBox=\"0 0 11px 11px\">\n"
"  <clipPath id=\"upleftclip\"><path d=\"M 11 0 L 0 0 L 0 11\"/></clipPath>\n"
"  <clipPath id=\"downrightclip\"><path d=\"M 11 0 L 11 11 L 0 11\"/></clipPath>\n"
"  <circle cx=\"5.5\" cy=\"5.5\" r=\"5.5\" fill=\"#808080\" clip-path=\"url(#upleftclip)\" />\n"
"  <circle cx=\"5.5\" cy=\"5.5\" r=\"5.5\" fill=\"white\" clip-path=\"url(#downrightclip)\" />\n"
"  <circle cx=\"5.5\" cy=\"5.5\" r=\"4.5\" fill=\"#404040\" clip-path=\"url(#upleftclip)\" />\n"
"  <circle cx=\"5.5\" cy=\"5.5\" r=\"4.5\" fill=\"#d4d0c8\" clip-path=\"url(#downrightclip)\" />\n"
"  <circle cx=\"5.5\" cy=\"5.5\" r=\"3.5\" fill=\"white\"/>\n"
"</svg>\n"));
	if (radiobutton_checked == 0)
		radiobutton_checked = new QSvgRenderer(QByteArray(
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
"<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
"     xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
"     version=\"1.1\" baseProfile=\"full\" width=\"11px\" height=\"11px\" viewBox=\"0 0 11px 11px\">\n"
"  <clipPath id=\"upleftclip\"><path d=\"M 11 0 L 0 0 L 0 11\"/></clipPath>\n"
"  <clipPath id=\"downrightclip\"><path d=\"M 11 0 L 11 11 L 0 11\"/></clipPath>\n"
"  <circle cx=\"5.5\" cy=\"5.5\" r=\"5.5\" fill=\"#808080\" clip-path=\"url(#upleftclip)\" />\n"
"  <circle cx=\"5.5\" cy=\"5.5\" r=\"5.5\" fill=\"white\" clip-path=\"url(#downrightclip)\" />\n"
"  <circle cx=\"5.5\" cy=\"5.5\" r=\"4.5\" fill=\"#404040\" clip-path=\"url(#upleftclip)\" />\n"
"  <circle cx=\"5.5\" cy=\"5.5\" r=\"4.5\" fill=\"#d4d0c8\" clip-path=\"url(#downrightclip)\" />\n"
"  <circle cx=\"5.5\" cy=\"5.5\" r=\"3.5\" fill=\"white\" />\n"
"  <circle cx=\"5.5\" cy=\"5.5\" r=\"1.5\" fill=\"black\" />\n"
"</svg>\n"));
}

void MyLooksStyle::producingForms(bool f) {
	weAreDrawingForms=f;
}

void MyLooksStyle::drawPrimitive( PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget) const {
	painter->setBrush(Qt::white);
	painter->setPen(QPen(Qt::black, 0.7));
	painter->setBackground(Qt::NoBrush);
	painter->setBackgroundMode(Qt::TransparentMode);
	QRect r = option->rect;
	if (element == QStyle::PE_PanelLineEdit) {
		painter->drawRect(r);
	} else if (element == QStyle::PE_IndicatorCheckBox) {
		if (weAreDrawingForms)
			painter->drawRect(r);
		else if (option->state & QStyle::State_On)
			checkbox_checked->render(painter, r);
		else
			checkbox->render(painter, r);
	} else if (element == QStyle::PE_IndicatorRadioButton) {
		if (weAreDrawingForms)
			painter->drawEllipse(r);
		else if (option->state & QStyle::State_On)
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
		fprintf(stderr, "Exit with code %d due to http error: %d %s\n", c, errorCode, m);
		return c;
	}
	return success?EXIT_SUCCESS:EXIT_FAILURE;
}

QSvgRenderer * MyLooksStyle::checkbox = 0;
QSvgRenderer * MyLooksStyle::checkbox_checked = 0;
QSvgRenderer * MyLooksStyle::radiobutton = 0;
QSvgRenderer * MyLooksStyle::radiobutton_checked = 0;

// void printSupportedWriteformats() {
//   QList<QByteArray> formats = QImageWriter::supportedImageFormats();
//   for ( QList<QByteArray>::Iterator f = formats.begin(); f != formats.end(); ++ f )
//     printf("%s , ", f->constData() );
// }
