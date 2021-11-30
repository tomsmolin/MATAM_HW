#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "priority_queue.h"

#define INITIAL_QUANTITY 0
#define SAME_PRIORITY 0

/*-------------------------------------------------------------------------
    data_structures:
-------------------------------------------------------------------------*/
typedef struct element_node 
{
    PQElement element;
    PQElementPriority priority;
    struct element_node *next;

} *PQElementNode;

struct PriorityQueue_t 
{

    PQElementNode elements_head;
    PQElementNode elements_last;

    PQElementNode iterator;

    int elements_quantity;

    CopyPQElement copy_element;
    FreePQElement free_element;
    EqualPQElements equal_elements;

    CopyPQElementPriority copy_priority;
    FreePQElementPriority free_priority;
    ComparePQElementPriorities compare_priority;

};

/*-------------------------------------------------------------------------
    static function declaration:
-------------------------------------------------------------------------*/
static void insertTheFirstElement(PriorityQueue queue, PQElementNode inserted_element);
static void insertElementToTheEndOfTheList(PriorityQueue queue, PQElementNode inserted_element);
static void insertElementToTheHeadOfTheList(PriorityQueue queue, PQElementNode inserted_element);
static void insertElementBetweenTwoElementsAccordingToPriority(PriorityQueue queue,PQElementNode inserted_element);
static void insertElementInPlace(PriorityQueue queue, PQElementNode inserted_element);
static void freeUnlinkedElement (PriorityQueue queue, PQElementNode element_node);
static void redefineElementPriorityAndInitializeNext(PriorityQueue queue,
                                                     PQElementNode element, PQElementPriority new_priority);

/*-------------------------------------------------------------------------
    priority_queue's functions implementation:
-------------------------------------------------------------------------*/
//1
PriorityQueue pqCreate(CopyPQElement copy_element,
                       FreePQElement free_element,
                       EqualPQElements equal_elements,
                       CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority,
                       ComparePQElementPriorities compare_priority) {
    if(!copy_element || !free_element || !equal_elements ||
       !copy_priority || !free_priority || !compare_priority)
    {
        return NULL;
    }

    PriorityQueue queue = malloc(sizeof(*queue));
    if (queue == NULL) {
        return NULL;
    }

    queue->elements_quantity = INITIAL_QUANTITY;
    queue->iterator = NULL;
    queue->elements_head = NULL;
    queue->elements_last = NULL;

    queue->copy_element = copy_element;
    queue->free_element = free_element;
    queue->equal_elements = equal_elements;

    queue->copy_priority = copy_priority;
    queue->free_priority = free_priority;
    queue->compare_priority = compare_priority;

    return queue;
}

//2
void pqDestroy(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return;
    }

    while (queue->elements_head)
    {
        PQElementNode to_delete = queue->elements_head;
        queue->elements_head = queue->elements_head->next;
        freeUnlinkedElement(queue, to_delete);
    }
    free(queue);
}

//3
PriorityQueue pqCopy(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return NULL;
    }
    //Iterator's value is undefined after operating this function.
    queue->iterator = NULL;

    PriorityQueue copied_queue = pqCreate(queue->copy_element, queue->free_element, queue->equal_elements,
                                       queue->copy_priority, queue->free_priority, queue->compare_priority);
    if (copied_queue == NULL)
    {
        return NULL;
    }

    PQElementNode iterator_temp = queue->elements_head;
    while (iterator_temp)
    {
        if (pqInsert(copied_queue, iterator_temp->element, iterator_temp->priority) != PQ_SUCCESS) {
            pqDestroy(copied_queue);
            return NULL;
        }
        iterator_temp = iterator_temp->next;
    }

    return copied_queue;
}

//4
int pqGetSize(PriorityQueue queue)
{
    if(queue == NULL)
    {
        return -1;
    }
    return queue->elements_quantity;
}

//5
bool pqContains(PriorityQueue queue, PQElement element)
{
    if (queue == NULL || element == NULL)
    {
        return false;
    }

    PQElementNode iterator_temp = queue->elements_head;
    while (iterator_temp)
    {
        if (queue->equal_elements(iterator_temp->element, element))
        {
            return true;
        }
        iterator_temp = iterator_temp->next;
    }
    return false;
}

//6
PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if (queue == NULL || element == NULL || priority == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }

    //Iterator's value is undefined after operating this function.
    queue->iterator = NULL;

    PQElementNode pq_element_node = malloc(sizeof(*pq_element_node));
    if (pq_element_node == NULL) {
        return PQ_OUT_OF_MEMORY;
    }

    //Fill the Element with its fields:
    pq_element_node->element = queue->copy_element(element);
    pq_element_node->priority = queue->copy_priority(priority);
    pq_element_node->next = NULL;

    if(pq_element_node->element == NULL || pq_element_node->priority == NULL)
    {
        freeUnlinkedElement(queue, pq_element_node);
        return PQ_OUT_OF_MEMORY;
    }

    insertElementInPlace(queue, pq_element_node);
    return PQ_SUCCESS;
}

//7
PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority)
{
    if (queue == NULL || element == NULL || old_priority == NULL || new_priority == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }

    //Iterator's value is undefined after operating this function.
    queue->iterator = NULL;
    bool element_found = false;
    bool allocation_failed = false;
    PQElementNode previous_element_node = NULL;
    PQElementNode iterator_temp = queue->elements_head;
    while (iterator_temp)
    {
        if (queue->equal_elements(iterator_temp->element, element))
        {
            if (queue->compare_priority(iterator_temp->priority, old_priority) == SAME_PRIORITY)
            {
                element_found = true;
                if(iterator_temp == queue->elements_head)
                {
                    queue->elements_head = iterator_temp->next;
                    redefineElementPriorityAndInitializeNext(queue, iterator_temp, new_priority);
                    if(iterator_temp->priority == NULL)
                    {
                        allocation_failed = true;
                    }
                    if (queue->elements_quantity == 1)
                    {
                        queue->elements_last = iterator_temp->next;
                    }
                    break;
                }
                if(iterator_temp == queue->elements_last)
                {
                    previous_element_node->next = NULL;
                    queue->elements_last = previous_element_node;
                    redefineElementPriorityAndInitializeNext(queue, iterator_temp, new_priority);
                    if(iterator_temp->priority == NULL)
                    {
                        allocation_failed = true;
                    }
                    break;
                }
                previous_element_node->next = iterator_temp->next;
                redefineElementPriorityAndInitializeNext(queue, iterator_temp, new_priority);
                if(iterator_temp->priority == NULL)
                {
                    allocation_failed = true;
                }
                break;
            }
        }
        previous_element_node = iterator_temp;
        iterator_temp = iterator_temp->next;
    }
    if (allocation_failed)
    {
        queue->elements_quantity--;
        freeUnlinkedElement(queue, iterator_temp);
        return PQ_OUT_OF_MEMORY;
    }
    if (element_found)
    {
        //Re-inserting the element:
        queue->elements_quantity--;
        insertElementInPlace(queue, iterator_temp);
        return PQ_SUCCESS;
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS;
}

//8
PriorityQueueResult pqRemove(PriorityQueue queue)
{
    if(queue == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }

    //Iterator's value is undefined after operating this function.
    queue->iterator = NULL;
    if(queue->elements_quantity == 0)
    {
        return PQ_SUCCESS;
    }

    PQElementNode to_delete = queue->elements_head;
    queue->elements_head = to_delete->next;//If head == Last than head.next = Null.

    freeUnlinkedElement(queue, to_delete);
    queue->elements_quantity--;
    if (queue->elements_quantity == 0)
    {
        queue->elements_last = NULL;
    }

    return PQ_SUCCESS;
}

//9
PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element)
{
    if (queue == NULL || element == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }

    //Iterator's value is undefined after operating this function.
    queue->iterator = NULL;

    bool element_removed = false;
    PQElementNode previous_element_node = NULL;
    PQElementNode iterator_temp = queue->elements_head;
    while (iterator_temp)
    {
        if (queue->equal_elements(iterator_temp->element, element))
        {
            if (iterator_temp == queue->elements_head)
            {
                return pqRemove(queue);
            }
            if (iterator_temp == queue->elements_last)
            {
                previous_element_node->next = iterator_temp->next;
                queue->elements_last = previous_element_node;
                element_removed = true;
                break;
            }

            //If the element is in the middle
            previous_element_node->next = iterator_temp->next;
            element_removed = true;
            break;
        }
        previous_element_node = iterator_temp;
        iterator_temp = iterator_temp->next;
    }
    if (element_removed)
    {
        queue->elements_quantity--;
        freeUnlinkedElement(queue, iterator_temp);
        return PQ_SUCCESS;
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS;
}

//10
PriorityQueueResult pqClear(PriorityQueue queue)
{
    if(queue == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    while (queue->elements_quantity > 0)
    {
        pqRemove(queue);
    }
    return PQ_SUCCESS;
}

//11
PQElement pqGetFirst(PriorityQueue queue)
{
    if(queue == NULL || queue->elements_quantity == 0)
    {
        return NULL;
    }
    queue->iterator = queue->elements_head;
    return queue->elements_head->element;
}

//12
PQElement pqGetNext(PriorityQueue queue)
{
    if(queue == NULL || queue->iterator == NULL )
    {
        return NULL;
    }
    if (queue->iterator->next == NULL)
    {
        queue->iterator = NULL;
        return NULL;
    }
    queue->iterator = queue->iterator->next;
    return queue->iterator->element;
}

/*-------------------------------------------------------------------------
    static functions implementation:
-------------------------------------------------------------------------*/
static void insertTheFirstElement(PriorityQueue queue, PQElementNode inserted_element)
{
    queue->elements_head = inserted_element;
    queue->elements_last = inserted_element;
    queue->elements_quantity++;
}

static void insertElementToTheEndOfTheList(PriorityQueue queue, PQElementNode inserted_element)
{
    queue->elements_last->next = inserted_element;
    queue->elements_last = inserted_element;
    queue->elements_quantity++;
}

static void insertElementToTheHeadOfTheList(PriorityQueue queue, PQElementNode inserted_element)
{
    inserted_element->next = queue->elements_head;
    queue->elements_head = inserted_element;
    queue->elements_quantity++;
}

static void insertElementBetweenTwoElementsAccordingToPriority(PriorityQueue queue,PQElementNode inserted_element)
{
    PQElementNode previous_element_node = queue->elements_head;
    PQElementNode iterator_temp = queue->elements_head->next;

    //Compare priority with the elements in the middle.
    while (iterator_temp)
    {
        int left_compare_result = queue->compare_priority(previous_element_node->priority, inserted_element->priority);
        int right_compare_result = queue->compare_priority(inserted_element->priority, iterator_temp->priority);

        /**Inserted element priority should be less or equal to the element from it left
         * and better than the priority of the elmenet to it right.
         * */
        if (left_compare_result >= 0 && right_compare_result > 0)
        {
            inserted_element->next = iterator_temp;
            previous_element_node->next = inserted_element;
            queue->elements_quantity++;
            return;
        }
        previous_element_node = iterator_temp;
        iterator_temp = iterator_temp->next;
    }
}

static void insertElementInPlace(PriorityQueue queue, PQElementNode inserted_element)
{
    //If the queue is empty.
    if (queue->elements_head == NULL)
    {
        insertTheFirstElement(queue,inserted_element);
        return;
    }

    //If the new element's priority is higher than the head element.
    if (queue->compare_priority(inserted_element->priority, queue->elements_head->priority) > 0) {
        insertElementToTheHeadOfTheList(queue,inserted_element);
        return;
    }

    //If the new element's priority is lower or equal to the last element.
    if (queue->compare_priority(queue->elements_last->priority, inserted_element->priority) >= 0)
    {
        insertElementToTheEndOfTheList(queue,inserted_element);
        return;
    }

    insertElementBetweenTwoElementsAccordingToPriority(queue,inserted_element);
}

static void freeUnlinkedElement (PriorityQueue queue, PQElementNode element_node)
{
    queue->free_element(element_node->element);
    queue->free_priority(element_node->priority);
    free(element_node);
}

static void redefineElementPriorityAndInitializeNext(PriorityQueue queue, PQElementNode element,
                                                                     PQElementPriority new_priority)
{
    queue->free_priority(element->priority);
    element->priority = queue->copy_priority(new_priority);
    element->next = NULL;
}


