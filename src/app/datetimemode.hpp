#ifndef DATEMODE_HPP
#define DATEMODE_HPP

#include "datetimeparseexception.hpp"
#include <QString>
#include <QDateTime>

enum DateTimeMode {
    DATETIME = 0,
    DATE,
    TIME
};

class DateTimeModeParser {
public:
    static DateTimeMode parseDateTime(QString dateTimeStr) {
        QString dateTimeStrLower(dateTimeStr.toLower());
        if (dateTimeStrLower == "datetime") {
            return DATETIME;
        } else if (dateTimeStrLower == "date") {
            return DATE;
        } else if (dateTimeStrLower == "time") {
            return TIME;
        }
        throw DateTimeParseException();
    }

    static QString generateTimestamp(QDateTime dateTime, DateTimeMode mode) {
        switch (mode) {
        case DATETIME:
            return dateTime.toString("yyyy-MM-ddThh:mm:ss") + offsetString(dateTime.offsetFromUtc());
        case DATE:
            return dateTime.toString("yyyy-MM-dd");
        case TIME:
            return dateTime.toString("hh:mm:ss") + offsetString(dateTime.offsetFromUtc());
        }
    }
private:
    // Return offset in [+-]HH:mm format
    static QString offsetString(int offset)
    {
        const int SECS_PER_HOUR = 3600;
        return QString::asprintf("%c%02d:%02d",
                                 offset >= 0 ? '+' : '-',
                                 qAbs(offset) / SECS_PER_HOUR,
                                 (qAbs(offset) / 60) % 60);
    }
};

#endif // DATEMODE_HPP
