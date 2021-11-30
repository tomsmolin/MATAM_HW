#include "event_container.h"



namespace mtm
{
    EventContainer::~EventContainer()
    {
        for (int i = 0; i < events_list.getSize(); i++)
        {
            delete &events_list[i];
        }
    }

    EventContainer::EventIterator& EventContainer::EventIterator::operator=(const EventIterator& iterator)
    {
        node = iterator.node;
        return *this;
    }

    EventContainer::EventIterator& EventContainer::EventIterator::operator++()
    {
        if (node->next == nullptr)
        {
            node++;
            return *this;
        }
        node = node->next;
        return *this;
    }

    BaseEvent& EventContainer::EventIterator::operator*()
    {
        return ((*node).data);
    }

    bool EventContainer::EventIterator::operator==(const EventIterator& iterator)
    {
        if(node == iterator.node)
        {
            return true;
        }
        return false;
    }

    bool EventContainer::EventIterator::operator!=(const EventIterator& iterator)
    {
        if (node == iterator.node)
        {
            return false;
        }
        return true;
    }

    EventContainer::EventIterator EventContainer::begin() const
    {
        EventIterator iterator;
        iterator.node = events_list.getFirst();
        return iterator;
    }

    EventContainer::EventIterator EventContainer::end() const
    {
        EventIterator iterator;
        if(events_list.getFirst() == nullptr)
        {
            iterator.node = nullptr;
            return iterator;
        }
        iterator.node = events_list.getLast();
        iterator.node++; 
        return iterator;
    }

    EventContainer::EventIterator EventContainer::begin()
    {
        EventIterator iterator;
        iterator.node = events_list.getFirst();
        return iterator;
    }

    EventContainer::EventIterator EventContainer::end()
    {
        EventIterator iterator;
        if(events_list.getFirst() == nullptr)
        {
            iterator.node = nullptr;
            return iterator;
        }
        iterator.node = events_list.getLast();
        iterator.node++; 
        return iterator;
    }
}