#include "tcptimeserver.hpp"
#include "udptimeserver.hpp"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QStringList args = app.arguments();

    int port = 8080;
    if (args.count() > 2) {
        qFatal("Only 1 argument supported: port number");
    } else if (args.count() == 2) {
        QString argument = args[1];
        if (argument == "-h" || argument == "--help") {
            qFatal("Starts a local date/time server on the port given as argument.");
        };
        bool ok = false;
        port = argument.toInt(&ok);
        if (!ok || port <= 0) {
            qFatal("Port argument must be numerical and greater than 0.");
        }
    }

    TcpTimeServer tcpTimeServer(port);
    UdpTimeServer udpTimeServer(port);

    return app.exec();
}
