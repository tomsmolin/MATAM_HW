#include "schedule.h"
using mtm::BaseEvent;
using mtm::DateWrap;
using mtm::EventContainer;
using std::ostream;


typedef EventContainer::EventIterator Iterator;
namespace mtm
{
    bool compareBaseEventPtr(const BaseEvent* event_one, const BaseEvent* event_two)
    {
        const BaseEvent& event_one_reference = (*event_one);
        const BaseEvent& event_two_reference = (*event_two);
        if(event_one_reference < event_two_reference)
        {
            return true;
        }
        
        return false;
    }

    class EqualBaseEventElement
    {
        private:
            DateWrap date;
            std::string name;
        public:
            EqualBaseEventElement(const DateWrap& event_date, const std::string& event_name)
            : date(event_date), name(event_name) {}
            ~EqualBaseEventElement()= default;
            bool operator()(BaseEvent* event)
            {
                if((event)->getDate() == date && ((event)->getName()).compare(name) == 0)
                {
                    return true;
                }
                return false;
            }
    };

    class sameMonthAndYear
    {
        private:
            int input_month;
            int input_year;
        public:
            sameMonthAndYear(int month, int year) : input_month(month), input_year(year) {}
            ~sameMonthAndYear()= default;
            bool operator()(BaseEvent* event)
            {
                int event_month = ((event)->getDate()).month();
                int event_year = ((event)->getDate()).year();
                if(event_month == input_month && event_year == input_year)
                {
                    return true;
                }
                return false;
            }
    };


    Schedule::~Schedule()
    {
        std::list<BaseEvent*>::iterator it = events.begin();
        while (it != events.end())
        {
            delete (*it);
            ++it;
        } 
    }

    void Schedule::addEvents(const EventContainer& container)
    {
        Iterator iterator;
        for (iterator = container.begin() ; iterator != container.end() ; ++iterator)
        {
            std::list<BaseEvent*>::iterator it = events.begin();
            while (it != events.end())
            {
                if ((*iterator) == (**it))
                {
                    throw  mtm::EventAlreadyExists();
                }
                ++it;
            } 
        }

        iterator = container.begin();
        while(iterator != container.end())
        {
            BaseEvent* event = (*iterator).clone();
            events.push_back(event);
            ++iterator;
        }
        events.sort(compareBaseEventPtr);
    }

    void Schedule::registerToEvent(const DateWrap& date, const std::string& name, int student)
    {
        if (student < 1 || student > MAX_STUDENT)
        {
            throw mtm::InvalidStudent();
        }

        std::list<BaseEvent*>::iterator it = std::find_if(events.begin(), events.end(),
                                                            EqualBaseEventElement(date, name));
        if (it == events.end())
        {
            throw mtm::EventDoesNotExist();
        }

        if ((*it)->studentInRegisterationList(student))
        {
            throw mtm::AlreadyRegistered();
        }

        if (!((*it)->allowToRegister(student)))
        {
            throw mtm::RegistrationBlocked();
        }
        
        (*it)->registerParticipant(student);

    }

    void Schedule::unregisterFromEvent(const DateWrap& date, const std::string& name, int student)
    {
        if (student < 1 || student > MAX_STUDENT)
        {
            throw mtm::InvalidStudent();
        }
        
        std::list<BaseEvent*>::iterator iterator = std::find_if(events.begin(), events.end(),
                                                                EqualBaseEventElement(date, name));
        if (iterator == events.end())
        {
            throw mtm::EventDoesNotExist();
        }

        if(!((*iterator)->studentInRegisterationList(student)))
        {
            throw mtm::NotRegistered();
            
        }
        (*iterator)->unregisterParticipant(student);

    }

    void Schedule::printAllEvents() const
    {
        for (std::list<BaseEvent*>::const_iterator iterator = events.begin() ; iterator != events.end() ; ++iterator)
        {
            (**iterator).printShort();
            std::cout << std::endl;
        }
    }

    void Schedule::printMonthEvents(int month, int year) const
    {
        if (month < MIN_MONTH || month > MAX_MONTH)
        {
            throw mtm::InvalidDate();
        }

        sameMonthAndYear sameMonthAndYear(month, year);
        for (std::list<BaseEvent*>::const_iterator iterator = events.begin() ; iterator != events.end() ; ++iterator)
        {
            if (sameMonthAndYear(*iterator))
            {
                (**iterator).printShort();
                std::cout << std::endl;
            }
        }

    }

    void Schedule::printEventDetails(const DateWrap& date, const std::string& name) const
    {
        std::list<BaseEvent*>::const_iterator iterator = std::find_if(events.begin(), events.end(),
                                                                     EqualBaseEventElement(date, name));
        if (iterator == events.end())
        {
            throw mtm::EventDoesNotExist();
        }
        (**iterator).printLong();
        std::cout << std::endl;
    }
}
