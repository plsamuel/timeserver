#ifndef DATETIMEPARSEEXCEPTION_HPP
#define DATETIMEPARSEEXCEPTION_HPP

#include <stdexcept>

class DateTimeParseException : public std::exception
{
public:
    DateTimeParseException();
    virtual const char* what() const throw ();
};

#endif // DATETIMEPARSEEXCEPTION_HPP
