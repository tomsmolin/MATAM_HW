#ifndef HW3_OPEN_EVENT_H
#define HW3_OPEN_EVENT_H
#include <iostream>
#include "base_event.h"


namespace mtm
{
    class OpenEvent : public BaseEvent
    {
        public:
            OpenEvent(const DateWrap& date, const std::string& name) : BaseEvent(date, name){}
            OpenEvent(const OpenEvent& open_event) : BaseEvent(open_event.event_date, open_event.event_name) 
            {
                student_list = open_event.student_list;
            }
            ~OpenEvent() = default; 
            OpenEvent* clone() const override 
            {
                return new OpenEvent(*this);
            }
    };
}


#endif //HW3_OPEN_EVENT_H
