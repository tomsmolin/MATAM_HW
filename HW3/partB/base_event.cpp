#include "base_event.h"
using std::string;
using mtm::BaseEvent;
using mtm::DateWrap;


void BaseEvent::registerParticipant(int student)
{
    isStudentValid(student);
    isStudentAlreadyRegistered(student);
    student_list.insert(student);
}

void BaseEvent::unregisterParticipant(int student)
{
    isStudentValid(student);
    isStudentNotRegistered(student);
    student_list.erase(student);
}

std::ostream& BaseEvent::printShort(std::ostream& out)
{
    out << event_name << ' ' << event_date << std::endl;
    return out;
} 

std::ostream& BaseEvent::printLong(std::ostream& out)
{
    this->printShort(out);
    this->student_list.printData(out);

    return out;
} 


/*
* The "smallest" BaseEvent object will be the object with the earlier date.
* If the dates are equal The operator's logic will be  
* as 'string::compare' where the operands are the compared event's names.
*/
bool BaseEvent::operator<(const BaseEvent& base_event_two) const
{
    if (event_date < base_event_two.event_date)

    {
        return true;
    }

    if (event_date > base_event_two.event_date)
    {
        return false;
    }
    if (event_name.compare(base_event_two.event_name) < 0)
    {
        return true;
    }
    return false;
}

bool BaseEvent::operator>(const BaseEvent& base_event_two) const
{ 
    if (*this < base_event_two)
    {
        return false;
    }
    return true;
}

DateWrap BaseEvent::getDate() const
{
    return event_date; 
}

std::string BaseEvent::getName() const 
{
    return event_name;
}

bool BaseEvent::allowToRegister(int student) 
{
    return true;
}

bool BaseEvent::operator==(const BaseEvent& base_event_two) const
{
    if (event_name.compare(base_event_two.event_name) == 0 
         && event_date == base_event_two.event_date)
            {
                return true;
            }
    return false;
}

bool BaseEvent::studentInRegisterationList(int student)
{
    if (student_list.listContains(student))
    {
        return true;
    }
    return false;
}

void BaseEvent::isStudentValid(int student)
{
    if (student < 1 || student > MAX_STUDENT)
    {
        throw mtm::InvalidStudent();
    }
}

void BaseEvent::isStudentNotRegistered(int student)
{
    if (!student_list.listContains(student))
    {
        throw mtm::NotRegistered();
    }
}


void BaseEvent::isStudentAlreadyRegistered(int student)
{
    if (student_list.listContains(student))
    {
        throw mtm::AlreadyRegistered();
    }
}



