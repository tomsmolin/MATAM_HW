#ifndef HW3_RECURRING_EVENT_H
#define HW3_RECURRING_EVENT_H
#include "event_container.h"

namespace mtm
{   
    template <class EventType>
    class RecurringEvent : public EventContainer
    {
     public:
        RecurringEvent(const DateWrap& first_date, const std::string& name,
                                    int num_occurrences, int interval_days);
        ~RecurringEvent() = default;

        void add(const BaseEvent& base_event) override
        {
            throw mtm::NotSupported();
        }


    };

    template <class EventType>
    RecurringEvent<EventType>::RecurringEvent(const DateWrap& first_date, const std::string& name,
                                    int num_occurrences, int interval_days)
    : EventContainer() 
    {
        if (num_occurrences < 1)
        {
            throw mtm::InvalidNumber();
        }
        if (interval_days < 1)
        {
            throw mtm::InvalidInterval();
        }
        DateWrap date(first_date);
        for (int i = 0; i < num_occurrences; i++) 
        {
            EventType* event = new EventType(date, name);
            BaseEvent& event_reference = *event;
            events_list.insert(event_reference);
            date += interval_days;
        }

    }

}



#endif //HW3_RECURRING_EVENT_H
