#ifndef HW3_EXCEPTIONS_H
#define HW3_EXCEPTIONS_H
#include <iostream>

namespace mtm 
{
    class Exception : public std::exception{};

    //Part A
    class InvalidDate : public Exception {};
    class NegativeDays : public Exception {};

    //Part B
    class AlreadyRegistered : public Exception {};
    class AlreadyInvited : public Exception {};
    class NotRegistered : public Exception {};
    class RegistrationBlocked : public Exception {};
    class InvalidStudent : public Exception {};
    class DateMismatch : public Exception {};
    class InvalidNumber : public Exception {};
    class InvalidInterval : public Exception {};
    class NotSupported : public Exception {};

    //Part C
    class EventAlreadyExists : public Exception {};
    class EventDoesNotExist : public Exception {};
}

#endif //HW3_EXCEPTIONS_H
