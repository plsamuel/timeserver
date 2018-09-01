#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "../app/datetimemode.hpp"

using namespace testing;

TEST(parseDateTimeMode_date, DateTimeModeTest)
{
    EXPECT_EQ(DATE, DateTimeModeParser::parseDateTime("date"));
}
TEST(parseDateTimeMode_time, DateTimeModeTest)
{
    EXPECT_EQ(TIME, DateTimeModeParser::parseDateTime("time"));
}
TEST(parseDateTimeMode_datetime, DateTimeModeTest)
{
    EXPECT_EQ(DATETIME, DateTimeModeParser::parseDateTime("datetime"));
}

TEST(parseDateTimeMode_exception, DateTimeModeTest)
{
    EXPECT_THROW(DateTimeModeParser::parseDateTime("Mumbojumbo"), DateTimeParseException);
}
