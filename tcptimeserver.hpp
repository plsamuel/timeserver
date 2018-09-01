

#ifndef TCPTIMESERVER_HPP
#define TCPTIMESERVER_HPP

#include <QObject>
#include "timeserver.hpp"

class QTcpServer;

class TcpTimeServer : public TimeServer
{
    Q_OBJECT

public:
    explicit TcpTimeServer(int port);

protected slots:
    virtual void receiveInboundConnection();

private:
    QTcpServer *tcpServer = nullptr;
};


#endif
