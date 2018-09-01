

#ifndef TIMESERVER_HPP
#define TIMESERVER_HPP

#include <QObject>
#include "datetimemode.hpp"

class TimeServer : public QObject
{
    Q_OBJECT

public:
    explicit TimeServer(int port);

protected slots:
    virtual void receiveInboundConnection() = 0;

protected:
    QByteArray buildTimeResponse(DateTimeMode mode);
    int port;
};


#endif
