

#ifndef UDPTIMESERVER_HPP
#define UDPTIMESERVER_HPP

#include <QObject>
#include "timeserver.hpp"

class QUdpSocket;

class UdpTimeServer : public TimeServer
{
    Q_OBJECT

public:
    explicit UdpTimeServer(int port);

protected slots:
    virtual void receiveInboundConnection();

private:
    QUdpSocket *udpSocket = nullptr;
};


#endif
