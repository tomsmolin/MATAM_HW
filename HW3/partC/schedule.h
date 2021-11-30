#ifndef HW3_SCHEDULE_H
#define HW3_SCHEDULE_H
#include <list>
#include <iterator>
#include <algorithm>
#include "linked_list.h"
#include "exceptions.h"
#include "date_wrap.h"
#include "base_event.h"
#include "event_container.h"


namespace mtm
{
    class Schedule
    {       
        private:
            std::list<BaseEvent*> events;
        
        public:
            Schedule() = default;
            ~Schedule();
            Schedule(const Schedule& cchedule) = delete;
            Schedule& operator=(const Schedule&) = delete;


            void addEvents(const EventContainer& event_container);
            void registerToEvent(const DateWrap& date, const std::string& name, int student);
            void unregisterFromEvent(const DateWrap& date, const std::string& name, int student);
            void printAllEvents() const;
            void printMonthEvents(int month, int year) const;
            
            template <typename Predicate>
            void printSomeEvents(Predicate predicate, bool verbose = false) const; 
            void printEventDetails(const DateWrap& date, const std::string& name) const;
    };

    template <typename Predicate>
    void Schedule::printSomeEvents(Predicate predicate, bool verbose) const
    {
        for (std::list<BaseEvent*>::const_iterator iterator = events.begin() ; iterator != events.end() ; ++iterator)
        {
            BaseEvent& event = (**iterator);
            if (predicate(event))
            {
                if(verbose)
                {
                    event.printLong();
                }
                else
                {
                    event.printShort();
                }   
                std::cout << std::endl;
            }
        }

    }


}




#endif //HW3_SCHEDULE_H
