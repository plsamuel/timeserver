

#ifndef SERVER_HPP
#define SERVER_HPP

#include <QString>
#include <QObject>
#include "datetimemode.hpp"

class QTcpServer;
class QUdpSocket;
class QNetworkSession;

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(int port);

private slots:
    void sessionOpened();
    void receiveInboundTCPConnection();
    void receiveInboundUDPData();

private:
    QByteArray buildResponse(DateTimeMode mode);

    QTcpServer *tcpServer = nullptr;
    QUdpSocket *udpSocket = nullptr;
    QNetworkSession *networkSession = nullptr;
    int port;
};


#endif
