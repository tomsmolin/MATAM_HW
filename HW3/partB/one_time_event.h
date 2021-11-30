#ifndef HW3_ONE_TIME_EVENT_H
#define HW3_ONE_TIME_EVENT_H
#include "event_container.h"
# define FIRST 0

namespace mtm
{
    template <class EventType>
    class OneTimeEvent : public EventContainer
    {
        public:
            OneTimeEvent(const DateWrap& date, const std::string& name);
            ~OneTimeEvent() = default;
        
            void add(const BaseEvent& base_event) override
            {
                throw mtm::NotSupported();
            }
              
    };

    template <class EventType>
    OneTimeEvent<EventType>::OneTimeEvent(const DateWrap& date, const std::string& name)
    : EventContainer()
    {
        EventType* ptr = new EventType(date, name);
        BaseEvent& event_reference = *ptr;
        events_list.insert(event_reference);
    }

}





#endif //HW3_ONE_TIME_EVENT_H
