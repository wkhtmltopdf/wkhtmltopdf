#include "srv.h"

#include <QDateTime>
#include <QHttpRequestHeader>
#include <QUrl>

#include <iostream>

#include "progressfeedback.hh"
#include <QApplication>
#include <QWebFrame>
#include <wkhtmltox/imagesettings.hh>
#include <wkhtmltox/utilities.hh>

#include <QTcpSocket>
#include <QTemporaryFile>
#include <QTextStream>
#include <QImage>
#include <QBuffer>
#include <QFontDatabase>
#include <QFileInfo>

Server::Server(quint16 port, int verbosity, QObject *parent)
    : QTcpServer(parent)
    , m_verbosity(verbosity)
{
    listen(QHostAddress::Any, port);
    MyLooksStyle * style = new MyLooksStyle();
    qApp->setStyle(style);
    m_verbosity && std::cout << "Listening on port:" << port << std::endl;
}

void Server::incomingConnection(int socket)
{
    m_data = QByteArray();
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);
}

QString Server::unescape(QByteArray in)
{
    // first step: decode '+' to spaces
    for(int i = 0; i < in.length(); ++i) {
        QByteRef c = in[i];
        if (c == '+') {
            c = ' ';
        }
    }
    // now decode as usual
    return QUrl::fromPercentEncoding(in);
};

void Server::outputResponse(QTcpSocket *socket, 
                            wkhtmltopdf::settings::ImageGlobal& settings)
{
    const QString status = "200";
    const QString JSON_MIMETYPE = "application/json";
    QString json = QString("{\"path\": \"%1\", \"clickzones\": \"{}\"}").arg(settings.out);
    int content_length = json.length();
    QString header = QString("HTTP/1.1 %1 Ok\r\n"
                             "Content-Type: %2\r\n"
                             "Content-Length: %3\r\n")
        .arg(status).arg(JSON_MIMETYPE).arg(content_length);
    header += "\r\n"; 
    if ( socket )
    {
        m_verbosity>2 && std::cout << "returning response:" << header.toLocal8Bit().constData() << json.toLocal8Bit().constData() << std::endl;
        socket->write(header.toAscii() + json.toAscii());
        socket->disconnectFromHost();
    }
}

void Server::readClient()
{
    // This slot is called when the client sent data to the server.
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if ( !socket )
    {
        return;
    }
    m_data += socket->readAll();
    int end_of_header = m_data.indexOf("\r\n\r\n");
    if (end_of_header < 0) 
    {
        return; // wait for more data
    }

    // Parse header
    QHttpRequestHeader m_header = QHttpRequestHeader(m_data.left(end_of_header));
    if ( m_data.size() - end_of_header - 4 < m_header.contentLength() )
    {
        return; // wait for more data
    }

    QMap<QString, QString> m_getMap;
    QMap<QString, QString> m_postMap;

    QUrl url = QUrl::fromEncoded(m_header.path().toAscii());
    QString m_path = url.path();

    // Parse GET parameters
    QListIterator<QPair<QString, QString> > i(url.queryItems());
    while (i.hasNext()) {
        QPair<QString, QString> pair = i.next();
        m_getMap[pair.first] = pair.second;
    }

    // Parse message content
    if (m_header.hasContentLength()) {
        QByteArray content = m_data.mid(end_of_header + 4, m_header.contentLength()); // +4 to skip "\r\n\r\n"
        // Parse POST data
        if(m_header.contentType() == "application/x-www-form-urlencoded") {
            QUrl url;
            url.setEncodedQuery(content);
            QList<QPair<QByteArray, QByteArray> > items =  url.encodedQueryItems();
            QListIterator<QPair<QByteArray, QByteArray> > i(items);
            while (i.hasNext()){
                QPair<QByteArray, QByteArray> pair = i.next();
                m_postMap[QString(pair.first)] = unescape(pair.second);
            }
        }
    }

    QTemporaryFile file(QString::number(socket->socketDescriptor()) + QString("_XXXXXX.html"));
    file.open();
    QTextStream out(&file);
    out << m_postMap["html"].toLocal8Bit();
    out.flush();
    wkhtmltopdf::settings::ImageGlobal settings;
    settings.in = file.fileName();
    settings.quality = 0;
    if ( m_postMap["format"].startsWith(".") )
    {
        m_postMap["format"] = m_postMap["format"].mid(1);
    }
    settings.fmt = m_postMap["format"];
    if ( settings.fmt.isEmpty() )
    {
        settings.fmt = "png";
    }    

    settings.out = m_postMap["output"];
    if ( settings.out.isEmpty() )
    {
        QTemporaryFile file(QString("/tmp/") + QString::number(socket->socketDescriptor()) + QString("_XXXXXX.") + settings.fmt);
        file.setAutoRemove( false );
        file.open();
        settings.out = file.fileName();
    }

    settings.screenWidth = m_postMap["width"].toInt();
    settings.screenHeight = m_postMap["height"].toInt();

    wkhtmltopdf::ImageConverter converter(settings);
    connect(&converter, SIGNAL(checkboxSvgChanged(const QString &)), qApp->style(), SLOT(setCheckboxSvg(const QString &)));
    connect(&converter, SIGNAL(checkboxCheckedSvgChanged(const QString &)), qApp->style(), SLOT(setCheckboxCheckedSvg(const QString &)));
    connect(&converter, SIGNAL(radiobuttonSvgChanged(const QString &)), qApp->style(), SLOT(setRadioButtonSvg(const QString &)));
    connect(&converter, SIGNAL(radiobuttonCheckedSvgChanged(const QString &)), qApp->style(), SLOT(setRadioButtonCheckedSvg(const QString &)));

    wkhtmltopdf::ProgressFeedback feedback(true, converter);
    bool success = converter.convert();
    if ( m_verbosity )
    {
        std::cout << "Rasterized html--------------------"<< std::endl;
        std::cout << "         peer: " << socket->peerAddress().toString().toLocal8Bit().constData() << std::endl;        
        std::cout << "      success: " << success << std::endl;        
        std::cout << "           in: " << settings.in.toLocal8Bit().constData() << std::endl;
        std::cout << "          out: " << settings.out.toLocal8Bit().constData() << std::endl;
        std::cout << "      quality: " << settings.quality<< std::endl;
        std::cout << "          fmt: " << settings.fmt.toLocal8Bit().constData() << std::endl;
        std::cout << "  screenWidth: " << settings.screenWidth << std::endl;
        std::cout << " screenHeight: " << settings.screenHeight << std::endl;
        if ( m_verbosity > 1 )
        {
            QFileInfo fi(settings.out);
            std::cout << "        bytes: " << fi.size() << std::endl;
            QImage img(settings.out, settings.fmt.toLocal8Bit().constData());
            std::cout << "         size: " << img.size().width() << "x" << img.size().height() << std::endl;
            std::cout << "         html: " << m_postMap["html"].toLocal8Bit().constData() << std::endl;
        }
    }
    outputResponse( socket, settings );
    closeConnection(socket);
}

void Server::discardClient()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if ( !socket )
    {
        return;
    }
    socket->deleteLater();
}

void Server::closeConnection(QTcpSocket *socket)
{
    if ( socket )
    {
        socket->close();
        if (socket->state() == QTcpSocket::UnconnectedState)
        {
            socket->deleteLater();
        }
    }
}
