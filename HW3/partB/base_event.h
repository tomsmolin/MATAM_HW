#ifndef HW3_BASE_EVENT_H
#define HW3_BASE_EVENT_H
#include <iostream>
#include <cstdio>
#include <string>
#include "linked_list.h"
#include "exceptions.h"
#include "date_wrap.h"

#define MAX_STUDENT 1234567890

namespace mtm
{

    class BaseEvent
    {
        protected:
            DateWrap event_date;
            std::string event_name;
            LinkedList<int> student_list;

        public:
            BaseEvent(const DateWrap& date, const std::string& name) : event_date(date), event_name(name) {}
            virtual ~BaseEvent() = default;
            BaseEvent& operator=(const BaseEvent&) = delete;

            virtual void registerParticipant(int student);
            void unregisterParticipant(int student);
            std::ostream& printShort(std::ostream& out = std::cout);
            std::ostream& printLong(std::ostream& out = std::cout);
            
            bool operator<(const BaseEvent& base_event_two) const;
            bool operator>(const BaseEvent& base_event_two) const;

            virtual BaseEvent* clone() const = 0;

            DateWrap getDate() const;
            std::string getName() const;

            bool operator==(const BaseEvent& base_event_two) const;
            bool studentInRegisterationList(int student);

            virtual bool allowToRegister(int student);

            //throw 'InvalidStudent' exception in case of an invalid integer for a student.
            static void isStudentValid(int student);

            //throw 'NotRegistered' exception if the entered sudent is not registered.
            void isStudentNotRegistered(int student);

            //throw 'AlreadyRegistered' exception if the entered sudent is already registered.
            void isStudentAlreadyRegistered(int student);



    };
    std::ostream& printShort(std::ostream& out = std::cout);
    std::ostream& printLong(std::ostream& out = std::cout);
    
}
#endif //HW3_BASE_EVENT_H
