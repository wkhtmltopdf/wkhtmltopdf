/*
 * File:   main.cpp
 * Author: Christian Sciberras
 *
 * Created on 18 ta’ Mejju 2010, 20:43
 */

#include <QCleanlooksStyle>
#include <QCommonStyle>
#include <QPainter>
#include <QStyleOption>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QImageWriter>

/**
 * Custom simplistic style, borrowed from WKHTMLTOPDF
 */
class MyLooksStyle: public QCleanlooksStyle {
public:
  //	Settings & settings;

	typedef QCleanlooksStyle parent_t;

  //MyLooksStyle(Settings & s): settings(s) {}

	void drawPrimitive( PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget = 0 ) const {
		painter->setBrush(Qt::white);
		painter->setPen(QPen(Qt::black, 0.7));
		QRect r = option->rect;
		if (element == QStyle::PE_PanelLineEdit) {
			painter->drawRect(r);
		} else if(element == QStyle::PE_IndicatorCheckBox) {
			painter->drawRect(r);
			//if (!settings.produceForms && (option->state & QStyle::State_On)) {
			r.translate(int(r.width()*0.075), int(r.width()*0.075));
			painter->drawLine(r.topLeft(), r.bottomRight());
			painter->drawLine(r.topRight(), r.bottomLeft());
			//}
		} else if(element == QStyle::PE_IndicatorRadioButton) {
			painter->drawEllipse(r);
			//if (!settings.produceForms && (option->state & QStyle::State_On)) {
			r.translate(int(r.width()*0.20), int(r.width()*0.20));
			r.setWidth(int(r.width()*0.70));
			r.setHeight(int(r.height()*0.70));
			painter->setBrush(Qt::black);
			painter->drawEllipse(r);
			//}
		} else {
			parent_t::drawPrimitive(element, option, painter, widget);
		}
	}
};

/*!
  \brief Guess a url, by looking at a string
  (shamelessly copied from Arora Project)
  \param string The string the is suppose to be some kind of url
*/
QUrl guessUrlFromString(const QString &string) {
	QString urlStr = string.trimmed();

	// check if the string is just a host with a port
	QRegExp hostWithPort(QLatin1String("^[a-zA-Z\\.]+\\:[0-9]*$"));
	if (hostWithPort.exactMatch(urlStr))
		urlStr = QLatin1String("http://") + urlStr;

	// Check if it looks like a qualified URL. Try parsing it and see.
	QRegExp test(QLatin1String("^[a-zA-Z]+\\:.*"));
	bool hasSchema = test.exactMatch(urlStr);
	if (hasSchema) {
		bool isAscii = true;
		foreach (const QChar &c, urlStr) {
			if (c >= 0x80) {
				isAscii = false;
				break;
			}
		}

		QUrl url;
		if (isAscii) {
			url = QUrl::fromEncoded(urlStr.toAscii(), QUrl::TolerantMode);
		} else {
			url = QUrl(urlStr, QUrl::TolerantMode);
		}
		if (url.isValid())
			return url;
	}

	// Might be a file.
	if (QFile::exists(urlStr)) {
		QFileInfo info(urlStr);
		return QUrl::fromLocalFile(info.absoluteFilePath());
	}

	// Might be a shorturl - try to detect the schema.
	if (!hasSchema) {
		int dotIndex = urlStr.indexOf(QLatin1Char('.'));
		if (dotIndex != -1) {
			QString prefix = urlStr.left(dotIndex).toLower();
			QString schema = (prefix == QLatin1String("ftp")) ? prefix : QLatin1String("http");
			QUrl url(schema + QLatin1String("://") + urlStr, QUrl::TolerantMode);
			if (url.isValid())
				return url;
		}
	}

	// Fall back to QUrl's own tolerant parser.
	QUrl url = QUrl(string, QUrl::TolerantMode);

	// finally for cases where the user just types in a hostname add http
	if (url.scheme().isEmpty())
		url = QUrl(QLatin1String("http://") + string, QUrl::TolerantMode);
	return url;
}

void printSupportedWriteformats(){
	QList<QByteArray> formats = QImageWriter::supportedImageFormats();
	for ( QList<QByteArray>::Iterator f = formats.begin(); f != formats.end(); ++ f )
	printf("%s , ", f->constData() );
}
