
#include <QtNetwork>
#include <QtCore>

#include "server.hpp"
#include "datetimemode.hpp"

Server::Server(int inPort)
 : QObject(),
   port(inPort)
{
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        QNetworkConfiguration config = manager.defaultConfiguration();

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);

        networkSession->open();
    } else {
        qWarning("meuh");
        sessionOpened();
    }

}

void Server::sessionOpened()
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qFatal(QString("Unable to start TCP server: %1.").arg(tcpServer->errorString()).toLocal8Bit());
    }
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::receiveInboundTCPConnection);


    udpSocket = new QUdpSocket(this);
    if (!udpSocket->bind(port, QUdpSocket::ShareAddress)) {
        qFatal(QString("Unable to start UDP server: %1.").arg(tcpServer->errorString()).toLocal8Bit());
    }

    connect(udpSocket, &QUdpSocket::readyRead, this, &Server::receiveInboundUDPData);

    qInfo() << QString("Started server on port %1").arg(port);

}


void Server::receiveInboundTCPConnection()
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

                clientConnection->write(buildResponse(mode));

            } catch (DateTimeParseException e) {
                qWarning(e.what());
            }

        }
    }

    clientConnection->disconnectFromHost();
}


void Server::receiveInboundUDPData()
{
    QByteArray datagram;

    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        QNetworkDatagram receivedData = udpSocket->receiveDatagram();
        QString msg = QString::fromLatin1(receivedData.data().constData()).trimmed();
        try {
            DateTimeMode mode = DateTimeModeParser::parseDateTime(msg);

            udpSocket->writeDatagram(receivedData.makeReply(buildResponse(mode)));

        } catch (DateTimeParseException e) {
            qWarning(e.what());
        }
    }
}


QByteArray Server::buildResponse(DateTimeMode mode)
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);

    out << DateTimeModeParser::generateTimestamp(QDateTime::currentDateTime(), mode).toLatin1() << "\r\n";
    return byteArray;
}
