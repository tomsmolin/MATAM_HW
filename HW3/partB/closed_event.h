#ifndef HW3_CLOSED_EVENT_H
#define HW3_CLOSED_EVENT_H

#include "base_event.h"

namespace mtm
{
    class ClosedEvent : public BaseEvent
    {
        private:
        LinkedList<int> invited_list;
        
        public:
            ClosedEvent(const DateWrap& date, const std::string& name) : BaseEvent(date, name){}
            ClosedEvent(const ClosedEvent& closed_event);
            ~ClosedEvent() = default; 

            void addInvitee(int student);
            void registerParticipant(int student) override;
            bool allowToRegister(int student) override;
            
            ClosedEvent* clone() const override;

    };
}



#endif //HW3_CLOSED_EVENT_H
