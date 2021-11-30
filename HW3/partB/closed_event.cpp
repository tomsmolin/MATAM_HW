#include "closed_event.h"
using mtm::ClosedEvent;


ClosedEvent::ClosedEvent(const ClosedEvent& closed_event)
: BaseEvent(closed_event.event_date, closed_event.event_name) 
{
    student_list = closed_event.student_list;
    invited_list = closed_event.invited_list;
}

void ClosedEvent::addInvitee(int student)
{
    isStudentValid(student);
    if (invited_list.listContains(student))
    {
        throw mtm::AlreadyInvited();
    }

    invited_list.insert(student);
}

void ClosedEvent::registerParticipant(int student)
{
    isStudentValid(student);
    isStudentAlreadyRegistered(student);
    if (!invited_list.listContains(student))
    {
        throw mtm::RegistrationBlocked();
    }
    
    student_list.insert(student);
}

bool ClosedEvent::allowToRegister(int student)
{
    if(invited_list.listContains(student))
    {
        return true;
    }
    return false;
}

ClosedEvent* ClosedEvent::clone() const  
{
    return new ClosedEvent(*this);
}
