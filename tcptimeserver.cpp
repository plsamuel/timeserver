
#include <QtNetwork>
#include <QtCore>

#include "tcptimeserver.hpp"
#include "datetimemode.hpp"

TcpTimeServer::TcpTimeServer(int inPort)
 : TimeServer(inPort)
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qFatal(QString("Unable to start TCP server: %1.").arg(tcpServer->errorString()).toLocal8Bit());
    }
    connect(tcpServer, &QTcpServer::newConnection, this, &TcpTimeServer::receiveInboundConnection);

    qInfo() << QString("Started TCP server on port %1").arg(port);
}


void TcpTimeServer::receiveInboundConnection()
{
    const int timeoutMs = 5000;

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);

    while (clientConnection->waitForReadyRead(timeoutMs)) {
        char buf[1024];
        qint64 lineLength = clientConnection->readLine(buf, sizeof(buf));
        if (lineLength != -1) {
            QString msg = QString::fromLatin1(buf).trimmed();
            try {
                DateTimeMode mode = DateTimeModeParser::parseDateTime(msg);

                clientConnection->write(buildTimeResponse(mode));

            } catch (DateTimeParseException e) {
                qWarning(e.what());
            }

        }
    }

    clientConnection->disconnectFromHost();
}

