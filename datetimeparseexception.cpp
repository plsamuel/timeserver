#include "datetimeparseexception.hpp"

DateTimeParseException::DateTimeParseException()
{
}

const char *DateTimeParseException::what() const throw()
{
    return "Error parsing datetime argument. Possible arguments are: datetime, date, time";
}
