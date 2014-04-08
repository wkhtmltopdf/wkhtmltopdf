#ifndef SRV_H
#define SRV_H

#include <QStringList>
#include <QTcpServer>

#include <wkhtmltox/imageconverter.hh>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(quint16 port, int verbosity = 0, QObject *parent = 0);

protected:
    void incomingConnection(int socketDescriptor);

private slots:
    void readClient();
    void discardClient();

private:
    wkhtmltopdf::ImageConverter* converter;
    QByteArray m_data;
    int m_verbosity;
    QString unescape(QByteArray in);
    void outputResponse(QTcpSocket *socket, 
                        wkhtmltopdf::settings::ImageGlobal& settings);
    void closeConnection(QTcpSocket *socket);
};

#endif
