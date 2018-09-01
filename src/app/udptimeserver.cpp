
#include <QtNetwork>
#include <QtCore>

#include "udptimeserver.hpp"
#include "datetimemode.hpp"

UdpTimeServer::UdpTimeServer(int inPort)
 : TimeServer(inPort)
{
    udpSocket = new QUdpSocket(this);
    if (!udpSocket->bind(port, QUdpSocket::ShareAddress)) {
        qFatal(QString("Unable to start UDP server: %1.").arg(udpSocket->errorString()).toLocal8Bit());
    }

    connect(udpSocket, &QUdpSocket::readyRead, this, &UdpTimeServer::receiveInboundConnection);

    qInfo() << QString("Started UDP server on port %1").arg(port);
}


void UdpTimeServer::receiveInboundConnection()
{
    QByteArray datagram;

    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        QNetworkDatagram receivedData = udpSocket->receiveDatagram();
        QString msg = QString::fromLatin1(receivedData.data().constData()).trimmed();
        try {
            DateTimeMode mode = DateTimeModeParser::parseDateTime(msg);

            udpSocket->writeDatagram(receivedData.makeReply(buildTimeResponse(mode)));

        } catch (DateTimeParseException e) {
            qWarning(e.what());
        }
    }
}
