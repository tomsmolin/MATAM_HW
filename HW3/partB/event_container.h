#ifndef HW3_EVENT_CONTAINER_H
#define HW3_EVENT_CONTAINER_H

#include "base_event.h"

namespace mtm
{

    class EventContainer
    {
        protected:
            LinkedList<BaseEvent&> events_list;

        public:
            EventContainer() = default;
            virtual ~EventContainer();

            EventContainer(const EventContainer& container) = delete;
            EventContainer& operator=(const EventContainer&) = delete;

            virtual void add(const BaseEvent& base_event) = 0;

            class EventIterator
            {
                public:
                    Node<BaseEvent&>* node;

                    EventIterator() : node(nullptr) {}
                    EventIterator(const EventIterator& iterator) 
                    {
                        node = iterator.node;
                    }
                    ~EventIterator() = default;

                    EventIterator& operator=(const EventIterator& iterator);
                    EventIterator& operator++();

                    BaseEvent& operator*();
                    bool operator==(const EventIterator& iterator);
                    bool operator!=(const EventIterator& iterator);

            };

            EventIterator begin() const;
            EventIterator end() const;

            EventIterator begin();
            EventIterator end();

    };

}
#endif //HW3_EVENT_CONTAINER_H
