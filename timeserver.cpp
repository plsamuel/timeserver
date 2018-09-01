
#include <QtNetwork>
#include <QtCore>

#include "timeserver.hpp"
#include "datetimemode.hpp"

TimeServer::TimeServer(int inPort)
 : QObject(),
   port(inPort)
{
}

QByteArray TimeServer::buildTimeResponse(DateTimeMode mode)
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);

    out << DateTimeModeParser::generateTimestamp(QDateTime::currentDateTime(), mode).toLatin1() << "\r\n";
    return byteArray;
}
