#ifndef HW3_FESTIVAL_H
#define HW3_FESTIVAL_H
#include "event_container.h"



namespace mtm
{
    class Festival : public EventContainer
    {
        private:
            DateWrap date;
        public:
            Festival(const DateWrap& date) : EventContainer(), date(date) {}
            ~ Festival() = default; 
            
            void add(const BaseEvent& base_event) override;
    };

}
#endif //HW3_FESTIVAL_H
