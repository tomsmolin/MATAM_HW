#include "festival.h"

using mtm::Festival;
using mtm::BaseEvent;


void Festival::add(const BaseEvent& base_event)
{
    if (date != base_event.getDate())
    {
        throw mtm::DateMismatch();
    }

    BaseEvent* ptr = base_event.clone();
    BaseEvent& base_event_reference = *ptr;
    events_list.insert(base_event_reference);  
}