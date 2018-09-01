
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

//    QString ipAddress;
//    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
//    // use the first non-localhost IPv4 address
//    for (int i = 0; i < ipAddressesList.size(); ++i) {
//        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
//            ipAddressesList.at(i).toIPv4Address()) {
//            ipAddress = ipAddressesList.at(i).toString();
//            break;
//        }
//    }
//    // if we did not find one, use IPv4 localhost
//    if (ipAddress.isEmpty())
//        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();


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
    QHostAddress clientAddress;
    quint16 clientPort;
    QByteArray datagram;

    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size(), &clientAddress, &clientPort);
        QString msg = QString::fromLatin1(datagram.constData()).trimmed();
        try {
            DateTimeMode mode = DateTimeModeParser::parseDateTime(msg);

            QUdpSocket responseSocket(this);
            responseSocket.writeDatagram(buildResponse(mode), clientAddress, clientPort);
            qWarning(QString("%1 %2").arg(clientAddress.toString()).arg(clientPort).toLocal8Bit());

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
