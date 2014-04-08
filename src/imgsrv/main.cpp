#include <QApplication>
#include <QtCore>

#include <iostream>
#include "srv.h"

int main(int argc, char *argv[])
{
    bool gui = false;
    QApplication app(argc, argv, gui);

    QStringList args = app.arguments();
    QRegExp rxArgPort("--port=([0-9]{1,})");
    QRegExp rxVerbose("--verbosity=([0-2])");

    int verbosity = 0;
    int port = 9090;
    for (int i = 1; i < args.size(); ++i) {
        if (rxArgPort.indexIn(args.at(i)) != -1 )
        {
            port =  rxArgPort.cap(1).toInt();
        }
        else if (rxVerbose.indexIn(args.at(i)) != 1 ) 
        {
            verbosity = rxVerbose.cap(1).toInt();
        }
        else 
        {
            std::cerr << "Uknown arg:" << args.at(i).toLocal8Bit().constData() << std::endl;
            app.exit();
            return -1;
        }
    }
    Server server(port, verbosity);

    return app.exec();
}
