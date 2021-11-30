#ifndef HW3_CUSTOM_EVENT_H
#define HW3_CUSTOM_EVENT_H

#include "base_event.h"

namespace mtm
{
    // We are assuming that CanRegister has copy ctor and operator=.
    template <typename CanRegister>
    class CustomEvent : public BaseEvent
    {
        private:
            CanRegister is_registeration_approved;

        public:
            CustomEvent(const DateWrap& date, const std::string& name, CanRegister is_registeration_approved)
             : BaseEvent(date, name)
             {
                this->is_registeration_approved = is_registeration_approved;
             }
            CustomEvent(const CustomEvent<CanRegister>& custom_event)
             : BaseEvent(custom_event.event_date, custom_event.event_name) 
             {
                student_list = custom_event.student_list;
                this->is_registeration_approved = custom_event.is_registeration_approved;
             }
            ~CustomEvent() = default;
            void registerParticipant(int student) override;

            CustomEvent* clone() const override 
            {
                return new CustomEvent(*this);
            }
            
            bool allowToRegister(int student) override;
    };
}

template <typename CanRegister>
void mtm::CustomEvent<CanRegister>::registerParticipant(int student)
{
    isStudentValid(student);
    isStudentAlreadyRegistered(student);
    
    if (!is_registeration_approved(student))
    {
        throw mtm::RegistrationBlocked();
    }
    
    student_list.insert(student);
}

template <typename CanRegister>
bool mtm::CustomEvent<CanRegister>::allowToRegister(int student)
{
    if(is_registeration_approved(student))
    {
        return true;
    }
    return false;
}

#endif //HW3_CUSTOM_EVENT_H
