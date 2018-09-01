#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "../app/datetimemode.hpp"
#include <QTimeZone>
#include <QDateTime>

using namespace testing;

namespace date_test {
    QDateTime testTimestamp = QDateTime::fromString("2018-01-02T13:14:15-05:00", Qt::ISODate);
}

TEST(generateTimestamp_date, DateTimeGenerateTest)
{
    EXPECT_STREQ("2018-01-02", DateTimeModeParser::generateTimestamp(date_test::testTimestamp, DATE).toLatin1());
}
TEST(generateTimestamp_time, DateTimeGenerateTest)
{
    EXPECT_STREQ("13:14:15-05:00", DateTimeModeParser::generateTimestamp(date_test::testTimestamp, TIME).toLatin1());
}
TEST(generateTimestamp_datetime, DateTimeGenerateTest)
{
    EXPECT_STREQ("2018-01-02T13:14:15-05:00", DateTimeModeParser::generateTimestamp(date_test::testTimestamp, DATETIME).toLatin1());
}
