#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "priority_queue.h"
#include "date.h"
#include "event_manager.h"

#define FIRST_ARGUMENT_IS_BIGGER 1
#define SECOND_ARGUMENT_IS_BIGGER -1
#define ARGUMENTS_IS_EQUAL 0
#define SAME_ARGUMENTS 0
#define NO_EVENTS 0
#define POSITIVE_INTEGER 1
#define NEGATIVE_INTEGER -1

/*-------------------------------------------------------------------------
    data_structures:
-------------------------------------------------------------------------*/
//Members priority_queue's element:
typedef struct MemberElement_t
{
    int member_id;
    char *member_name;
    int events_quantity;
} *MemberElement;

//Members priority_queue's priorities.
typedef struct MemberPriority_t 
{
    int events_quantity;
    int member_id;
} *MemberPriority;

//Events priority_queue's element.
typedef struct EventElement_t 
{
    Date date;
    char *event_name;
    int event_id;
    PriorityQueue event_member_team;
} *EventElement;

//TeamMembers priority_queue's element.
typedef struct EventTeamMemberElement_t 
{
    int member_id;
    MemberElement member_element_ptr;
} *EventTeamMemberElement;


struct EventManager_t 
{
    Date start_date;
    PriorityQueue events;
    PriorityQueue members;
};

/*-------------------------------------------------------------------------
    pqCreate's functions declarations:
-------------------------------------------------------------------------*/
//Declaration for PQ members's functions:
PQElement copyMemberElement(PQElement member_element);

void freeMemberElement(PQElement member_element);

PQElementPriority copyMemberPriority(PQElementPriority member_priority);

void freeMemberPriority(PQElementPriority member_priority);

bool equalMemberElements(PQElement member_element_one, PQElement member_element_two);

int compareMemberPriorities(PQElementPriority member_priority_one, PQElementPriority member_priority_two);

//Declaration for PQ event_team_members's functions:
PQElement copyTeamMemberElement(PQElement team_member_element);

void freeEventTeamMemberElement(PQElement team_member_element);

PQElementPriority copyTeamMemberPriority(PQElementPriority team_member_priority);

void freeEventTeamMemberPriority(PQElementPriority team_member_priority);

bool equalEventTeamMemberElements(PQElement team_member_element_one, PQElement team_member_element_two);

int compareEventTeamMemberPriorities(PQElementPriority team_member_priority_one,
                                     PQElementPriority team_member_priority_two);

//Declaration for PQ event's functions:
PQElement copyEventElement(PQElement event_element);

void freeEventElement(PQElement event_element);

PQElementPriority copyEventPriority(PQElementPriority event_priority);

void freeEventPriority(PQElementPriority event_priority);

bool equalEventElement(PQElement event_one, PQElement event_two);

int compareEventPriorities(PQElementPriority event_priority_one, PQElementPriority event_priority_two);

/*-------------------------------------------------------------------------
    static functions declaration:
-------------------------------------------------------------------------*/
/*Get EventElement field and create EventElement type argument from it.
 * Returns NULL if some allocation failed.
 * */
static EventElement createEventElement(Date date, char *event_name, int event_id);

/*Get MemberElement field and create MemberElement type argument from it.
 * Returns NULL if some allocation failed.
 * */
static MemberElement createMemberElement(int member_id, char *member_name);

/*Get EventTeamMemberElement field and create EventTeamMemberElement type argument from it.
 * Returns NULL if some allocation failed.
 * */
static EventTeamMemberElement createTeamMemberElement(int member_id, MemberElement member_element);

/*Get MemberPriority field and create MemberPriority type argument from it.
 * Returns NULL if some allocation failed.
 * */
static MemberPriority createMemberPriority(int member_id, int events_quantity);

/*Returns NULL if some allocation failed.
 * */
static int *createTeamMemberPriority(int member_id);

/*Create MemberPriority type arguments with 'old_events_quantity' and with 'new_events_quantity' arguments,
 * Send the above arguments to pqChangePriority.
 * Return- PQ_OUT_OF_MEMORY if  an allocation failed, otherwise, return 'pqChangePriority'.
 * */
static PriorityQueueResult changeMemberPriority(EventManager em, MemberElement member_element,
                                                int old_events_quantity, int new_events_quantity);

//Remove the given team member for the desired event and changes it priority accordingly.
static EventManagerResult removeTeamMemberFromEvent(EventManager em, EventElement event_element,
                                                    EventTeamMemberElement team_member_element);

/*Allocating memory for string duplications.
returns pointer of char in case of succcess
NULL if allocation fails
**/
static char *copyName(char *name);

//Return the EventElement element of an event with event_id
static EventElement getEventElement(PriorityQueue events_queue, int event_id);

//Return the MemberElement element of a member with member_id
static MemberElement getMemberElement(PriorityQueue members_queue, int member_id);

//Return if a given queue contain an event with a given name at a given date.
static bool isEventNameAndDateExistInQueue(PriorityQueue events_queue, char *event_name, Date event_date);

//Return if a given queue contain an event with a given event_id.
static bool isEventIdExistInQueue(PriorityQueue events_queue, int event_id);

//Return if a given queue contain member with a given member_id.
static bool isMemberIdExistInQueue(PriorityQueue members_queue, int member_id);

//Return if a given member with a given member_id is running a given event.
static bool isMemberLinkedToEvent(PriorityQueue members_queue, int member_id);

/*Compare two integers and return the biggest.*/
static int isBigger(int argument_one, int argument_two);

/*-------------------------------------------------------------------------
    event_manager's functions implementation:
-------------------------------------------------------------------------*///1
//1
EventManager createEventManager(Date date)
{
    if (date == NULL) {
        return NULL;
    }

    EventManager event_manager = malloc(sizeof(*event_manager));
    if (event_manager == NULL) {
        return NULL;
    }

    event_manager->start_date = dateCopy(date);
    if (event_manager->start_date == NULL) {
        free(event_manager);
        return NULL;
    }
    //Events priorities are determined by their date or by insertion's order in case of equal dates
    event_manager->events = pqCreate(&copyEventElement, &freeEventElement, &equalEventElement,
                                     &copyEventPriority, &freeEventPriority, &compareEventPriorities);

    if (event_manager->events == NULL) {
        dateDestroy(event_manager->start_date);
        free(event_manager);
        return NULL;
    }

    //Members priorities by their'e events quantity or by their'e id in case of an equal events quantity.
    event_manager->members = pqCreate(&copyMemberElement, &freeMemberElement, &equalMemberElements,
                                      &copyMemberPriority, &freeMemberPriority, &compareMemberPriorities);
    if (event_manager->members == NULL) {
        destroyEventManager(event_manager);
        return NULL;
    }

    return event_manager;
}

//2
void destroyEventManager(EventManager em)
{
    if (em == NULL) {
        return;
    }

    pqDestroy(em->events);
    pqDestroy(em->members);
    dateDestroy(em->start_date);
    free(em);
}

//3
EventManagerResult emAddEventByDate(EventManager em, char *event_name, Date date, int event_id) 
{
    if (em == NULL || event_name == NULL || date == NULL) {
        return EM_NULL_ARGUMENT;
    }

    if (dateCompare(date,em->start_date) < 0) {
        return EM_INVALID_DATE;
    }

    if (event_id < 0) {
        return EM_INVALID_EVENT_ID;
    }

    if (isEventNameAndDateExistInQueue(em->events, event_name, date)) {
        return EM_EVENT_ALREADY_EXISTS;
    }
    if (isEventIdExistInQueue(em->events, event_id)) {
        return EM_EVENT_ID_ALREADY_EXISTS;
    }

    //Building the event's data structure and inserts it to event_manager (to events priority queue field):
    EventElement event_element = createEventElement(date, event_name, event_id);
    if (event_element == NULL) {
        return EM_OUT_OF_MEMORY;
    }
    PriorityQueueResult result = pqInsert(em->events, event_element, event_element->date);

    freeEventElement(event_element);
    if (result == PQ_OUT_OF_MEMORY) {
        return EM_OUT_OF_MEMORY;
    }
    return EM_SUCCESS;
}

//4
EventManagerResult emAddEventByDiff(EventManager em, char *event_name, int days, int event_id) 
{
    if (em == NULL || event_name == NULL) {
        return EM_NULL_ARGUMENT;
    }
    if (days < 0) {
        return EM_INVALID_DATE;
    }
    if (event_id < 0) {
        return EM_INVALID_EVENT_ID;
    }
    //Create Date-type argument using 'days':
    Date event_date = dateCopy(em->start_date);

    if (event_date == NULL) {
        return EM_OUT_OF_MEMORY;
    }

    //Advance 'event_date' by 'days'. if 'days' is 0, than 'event_date' will be 'start_day':
    while (days > 0) {
        dateTick(event_date);
        days--;
    }

    if (isEventNameAndDateExistInQueue(em->events, event_name, event_date)) {
        dateDestroy(event_date);
        return EM_EVENT_ALREADY_EXISTS;
    }
    if (isEventIdExistInQueue(em->events, event_id)) {
        dateDestroy(event_date);
        return EM_EVENT_ID_ALREADY_EXISTS;
    }

    //Building the event's data structure and inserts it to event_manager  (to events priority queue field):
    EventElement event_element = createEventElement(event_date, event_name, event_id);
    if (event_element == NULL) {
        dateDestroy(event_date);
        return EM_OUT_OF_MEMORY;
    }

    PriorityQueueResult insert_result = pqInsert(em->events, event_element, event_date);
    dateDestroy(event_date);
    freeEventElement(event_element);
    if (insert_result == PQ_OUT_OF_MEMORY) {
        return EM_OUT_OF_MEMORY;
    }
    return EM_SUCCESS;
}

//5
EventManagerResult emRemoveEvent(EventManager em, int event_id) 
{
    if (em == NULL) {
        return EM_NULL_ARGUMENT;
    }

    if (event_id < 0) {
        return EM_INVALID_EVENT_ID;
    }

    EventElement event_element = getEventElement(em->events, event_id);
    if (event_element == NULL) {
        return EM_EVENT_NOT_EXISTS;
    }
    int initial_team_size = pqGetSize(event_element->event_member_team);

    for (int i = 0; i < initial_team_size; i++) {
        EventTeamMemberElement team_iterator = pqGetFirst(event_element->event_member_team);
        EventManagerResult remove_from_event_team = removeTeamMemberFromEvent(em, event_element, team_iterator);
        if (remove_from_event_team == EM_OUT_OF_MEMORY) {
            return EM_OUT_OF_MEMORY;
        }
    }
    //If reached here, event exists, and is not a NULL argument
    pqRemoveElement(em->events, event_element);
    return EM_SUCCESS;
}

//6
EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date) 
{
    if (em == NULL || new_date == NULL) {
        return EM_NULL_ARGUMENT;
    }
    if (dateCompare(new_date, em->start_date) < 0) {
        return EM_INVALID_DATE;
    }
    if (event_id < 0) {
        return EM_INVALID_EVENT_ID;
    }
    EventElement event_element = getEventElement(em->events, event_id);
    if (event_element == NULL) {
        return EM_EVENT_ID_NOT_EXISTS;
    }
    char *event_name = event_element->event_name;
    if (isEventNameAndDateExistInQueue(em->events, event_name, new_date)) {
        return EM_EVENT_ALREADY_EXISTS;
    }

    //Creating copy, in order to avoid using the original address at pqChangePriority.
    Date current_date = dateCopy(event_element->date);
    if (current_date == NULL) {
        return EM_OUT_OF_MEMORY;
    }

    //Update the event priority due to the date change:
    PriorityQueueResult change_result = pqChangePriority(em->events, event_element, current_date, new_date);
    if (change_result == PQ_OUT_OF_MEMORY) {
        dateDestroy(current_date);
        return EM_OUT_OF_MEMORY;
    }

    //Re-locate the rescheduled event in case the generic function changes it's address:
    EventElement rescheduled_event = getEventElement(em->events, event_id);

    dateDestroy(current_date);
    dateDestroy(rescheduled_event->date);//freeing date from "Element Node":
    rescheduled_event->date = dateCopy(new_date);
    if (rescheduled_event->date == NULL) {
        return EM_OUT_OF_MEMORY;
    }

    return EM_SUCCESS;
}

//7
EventManagerResult emAddMember(EventManager em, char *member_name, int member_id) 
{
    if (em == NULL || member_name == NULL) {
        return EM_NULL_ARGUMENT;
    }

    if (member_id < 0) {
        return EM_INVALID_MEMBER_ID;
    }

    if (isMemberIdExistInQueue(em->members, member_id)) {
        return EM_MEMBER_ID_ALREADY_EXISTS;
    }

    //Building the member's data structure and inserts it to event_manager (to member priority queue field):
    MemberElement new_member_element = createMemberElement(member_id, member_name);
    if (new_member_element == NULL) {
        return EM_OUT_OF_MEMORY;
    }

    MemberPriority new_member_priority = createMemberPriority(member_id, NO_EVENTS);
    if (new_member_priority == NULL) {
        freeMemberElement(new_member_element);
        return EM_OUT_OF_MEMORY;
    }

    PriorityQueueResult result = pqInsert(em->members, new_member_element, new_member_priority);

    freeMemberElement(new_member_element);
    freeMemberPriority(new_member_priority);
    if (result == PQ_OUT_OF_MEMORY) {
        return EM_OUT_OF_MEMORY;
    }

    return EM_SUCCESS;
}

//8
EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id) 
{
    if (em == NULL) {
        return EM_NULL_ARGUMENT;
    }

    if (event_id < 0) {
        return EM_INVALID_EVENT_ID;
    }

    if (member_id < 0) {
        return EM_INVALID_MEMBER_ID;
    }
    EventElement event_element = getEventElement(em->events, event_id);
    if (event_element == NULL) {
        return EM_EVENT_ID_NOT_EXISTS;
    }
    MemberElement member_element = getMemberElement(em->members, member_id);
    if (member_element == NULL) {
        return EM_MEMBER_ID_NOT_EXISTS;
    }
    if (isMemberLinkedToEvent(event_element->event_member_team, member_id)) {
        return EM_EVENT_AND_MEMBER_ALREADY_LINKED;
    }

    /*Since we want to add an event to the member associated with 'member_element',
     * we need to increase it events_quantity field by 1, and change his priority accordingly:
    */
    int old_events_quantity = member_element->events_quantity;
    int new_events_quantity = old_events_quantity + 1;
    PriorityQueueResult change_result = changeMemberPriority(em, member_element,
                                                             old_events_quantity, new_events_quantity);
    if (change_result == PQ_OUT_OF_MEMORY) {
        return EM_OUT_OF_MEMORY;
    }

    //Re-locates the member_element in case the generic function changes it's address:
    MemberElement updated_member_element = getMemberElement(em->members, member_id);

    //Building the Team member's data structure and link  the member to 'event_element':
    EventTeamMemberElement team_member_element = createTeamMemberElement(member_id, updated_member_element);
    if (team_member_element == NULL) {
        return EM_OUT_OF_MEMORY;
    }

    int *team_member_priority = createTeamMemberPriority(member_id);
    if (team_member_priority == NULL) {
        freeEventTeamMemberElement(team_member_element);
        return EM_OUT_OF_MEMORY;
    }
    PriorityQueueResult insert_result = pqInsert(event_element->event_member_team,
                                                 team_member_element, team_member_priority);

    freeEventTeamMemberElement(team_member_element);
    freeEventTeamMemberPriority(team_member_priority);
    if (insert_result == PQ_OUT_OF_MEMORY) {
        return EM_OUT_OF_MEMORY;
    }
    return EM_SUCCESS;
}

//9
EventManagerResult emRemoveMemberFromEvent(EventManager em, int member_id, int event_id) 
{
    if (em == NULL) {
        return EM_NULL_ARGUMENT;
    }
    if (event_id < 0) {
        return EM_INVALID_EVENT_ID;
    }
    if (member_id < 0) {
        return EM_INVALID_MEMBER_ID;
    }

    EventElement event_element = getEventElement(em->events, event_id);
    if (event_element == NULL) {
        return EM_EVENT_ID_NOT_EXISTS;
    }
    if (!isMemberIdExistInQueue(em->members, member_id)) {
        return EM_MEMBER_ID_NOT_EXISTS;
    }
    if (!isMemberLinkedToEvent(event_element->event_member_team, member_id)) {
        return EM_EVENT_AND_MEMBER_NOT_LINKED;
    }

    //Unlinking the given member from the desired event:
    EventManagerResult remove_result = EM_NULL_ARGUMENT;
    PQ_FOREACH(EventTeamMemberElement, team_iterator, event_element->event_member_team) {
        if (team_iterator->member_id == member_id) {
            remove_result = removeTeamMemberFromEvent(em, event_element, team_iterator);
            if (remove_result == EM_OUT_OF_MEMORY) {
                return EM_OUT_OF_MEMORY;
            }
            break;
        }
    }

    return EM_SUCCESS;
}

//10
EventManagerResult emTick(EventManager em, int days) 
{
    if (em == NULL) {
        return EM_NULL_ARGUMENT;
    }
    if (days <= 0) {
        return EM_INVALID_DATE;
    }
    //Advance 'em->start_day' by 'days'.
    while (days > 0) {
        dateTick(em->start_date);
        days--;
    }
    int events_amount = pqGetSize(em->events);

    for (int i = 0; i < events_amount; i++) {
        EventElement events_iterator = pqGetFirst(em->events);
        if (dateCompare(events_iterator->date, em->start_date) < 0) {
            EventManagerResult remove_result = emRemoveEvent(em, events_iterator->event_id);
            if (remove_result == EM_OUT_OF_MEMORY) {
                return EM_OUT_OF_MEMORY;
            }
        }
    }
    return EM_SUCCESS;
}

//11
int emGetEventsAmount(EventManager em) 
{
    if (em == NULL) {
        return -1;
    }

    return pqGetSize(em->events);
}

//12
char *emGetNextEvent(EventManager em) 
{
    if (em == NULL) {
        return NULL;
    }
    EventElement next_event = pqGetFirst(em->events);
    if (next_event == NULL) {
        return NULL;
    }
    return next_event->event_name;
}

//13
void emPrintAllEvents(EventManager em, const char *file_name) 
{

    if (em == NULL) {
        return;
    }
    FILE *stream = fopen(file_name, "w");
    if (stream == NULL) {
        return;
    }
    int day = 0, month = 0, year = 0;
    int *day_ptr = &day, *month_ptr = &month, *year_ptr = &year;

    PQ_FOREACH(EventElement, events_iterator, em->events) {
        int team_size = pqGetSize(events_iterator->event_member_team);
        fprintf(stream, "%s,", events_iterator->event_name);
        dateGet(events_iterator->date, day_ptr, month_ptr, year_ptr);
        fprintf(stream, "%d.%d.%d", day, month, year);
        if (team_size > 0) {
            fprintf(stream, ",");
        }

        PQ_FOREACH(EventTeamMemberElement, team_iterator, events_iterator->event_member_team) {

            fprintf(stream, "%s", team_iterator->member_element_ptr->member_name);
            if (team_size > 1) {

                fprintf(stream, ",");
            }
            team_size--;
        }
        fprintf(stream, "\n");
    }
    fclose(stream);
}

//14
void emPrintAllResponsibleMembers(EventManager em, const char *file_name) 
{
    if (em == NULL) {
        return;
    }
    FILE *stream = fopen(file_name, "w");
    if (stream == NULL) {
        return;
    }
    PQ_FOREACH(MemberElement, members_iterator, em->members) {
        if (members_iterator->events_quantity > 0) {
            fprintf(stream, "%s,", members_iterator->member_name);
            fprintf(stream, "%d\n", members_iterator->events_quantity);
        }
    }
    fclose(stream);
}

/*-------------------------------------------------------------------------
    event_manager's date structure's functions implementation:
-------------------------------------------------------------------------*/
//Functions for Members Priority Queue:
PQElement copyMemberElement(PQElement member_element) 
{
    if (member_element == NULL) {
        return NULL;
    }

    MemberElement copied_member_element = malloc(sizeof(*copied_member_element));
    if (copied_member_element == NULL) {
        return NULL;
    }
    char *copied_name = malloc((strlen(((MemberElement) member_element)->member_name) + 1) * sizeof(char));
    if (copied_name == NULL) {
        free(copied_member_element);
        return NULL;
    }
    copied_member_element->member_name = strcpy(copied_name, ((MemberElement) member_element)->member_name);
    copied_member_element->events_quantity = ((MemberElement) member_element)->events_quantity;
    copied_member_element->member_id = ((MemberElement) member_element)->member_id;
    return copied_member_element;
}

PQElementPriority copyMemberPriority(PQElementPriority member_priority) 
{
    if (member_priority == NULL) {
        return NULL;
    }
    MemberPriority copied_member_priority = malloc(sizeof(*copied_member_priority));
    if (copied_member_priority == NULL) {
        return NULL;
    }
    copied_member_priority->events_quantity = ((MemberPriority) member_priority)->events_quantity;
    copied_member_priority->member_id = ((MemberPriority) member_priority)->member_id;

    return copied_member_priority;
}

void freeMemberPriority(PQElementPriority member_priority) 
{
    if (member_priority == NULL) {
        return;
    }
    free((MemberPriority) member_priority);
}

void freeMemberElement(PQElement member_element) 
{
    if (member_element == NULL) {
        return;
    }
    char *member_name = ((MemberElement) member_element)->member_name;
    free(member_name);
    free(((MemberElement) member_element));
}

bool equalMemberElements(PQElement member_element_one, PQElement member_element_two)
{
    if (member_element_one == NULL || member_element_two == NULL) {
        return false;
    }
    if (((MemberElement) member_element_one)->member_id ==
        ((MemberElement) member_element_two)->member_id) {
        return true;
    }
    return false;
}

int compareMemberPriorities(PQElementPriority member_priority_one, PQElementPriority member_priority_two)
{
    int member_one_events_quantity = ((MemberPriority) member_priority_one)->events_quantity;
    int member_two_events_quantity = ((MemberPriority) member_priority_two)->events_quantity;
    int compare_events_quantity_result = isBigger(member_one_events_quantity, member_two_events_quantity);

    if (compare_events_quantity_result == FIRST_ARGUMENT_IS_BIGGER) {
        return POSITIVE_INTEGER;
    }
    if (compare_events_quantity_result == SECOND_ARGUMENT_IS_BIGGER) {
        return NEGATIVE_INTEGER;
    }

    int member_one_id = ((MemberPriority) member_priority_one)->member_id;
    int member_two_id = ((MemberPriority) member_priority_two)->member_id;
    int compare_members_id_result = isBigger(member_one_id, member_two_id);
    if (compare_members_id_result == FIRST_ARGUMENT_IS_BIGGER) {
        return NEGATIVE_INTEGER;
    }
    if (compare_members_id_result == SECOND_ARGUMENT_IS_BIGGER) {
        return POSITIVE_INTEGER;
    }
    return SAME_ARGUMENTS;
}

//Functions for Events Priority Queue:
PQElement copyEventElement(PQElement event_element) 
{
    if (event_element == NULL) {
        return NULL;
    }

    EventElement copied_event_element = malloc(sizeof(*copied_event_element));
    if (copied_event_element == NULL) {
        return NULL;
    }

    char *copied_name = malloc((strlen(((EventElement) event_element)->event_name) + 1) * sizeof(char));
    if (copied_name == NULL) {
        freeEventElement(copied_event_element);
        return NULL;
    }

    copied_event_element->event_name = strcpy(copied_name, ((EventElement) event_element)->event_name);
    copied_event_element->date = dateCopy(((EventElement) event_element)->date);
    if (copied_event_element->date == NULL) {
        freeEventElement(copied_event_element);
        return NULL;
    }

    copied_event_element->event_id = ((EventElement) event_element)->event_id;

    copied_event_element->event_member_team = pqCopy(((EventElement) event_element)->event_member_team);
    if (copied_event_element->event_member_team == NULL) {
        freeEventElement(copied_event_element);
        return NULL;
    }

    return copied_event_element;
}

void freeEventElement(PQElement event_element) 
{
    if (event_element == NULL) {
        return;
    }
    if (((EventElement) event_element)->event_name != NULL) {
        free(((EventElement) event_element)->event_name);
    }
    dateDestroy(((EventElement) event_element)->date);
    pqDestroy(((EventElement) event_element)->event_member_team);
    free(((EventElement) event_element));
}

PQElementPriority copyEventPriority(PQElementPriority event_priority) 
{

    //If argument is NULL, dateCopy return NULL
    Date copied_priority = dateCopy((Date) event_priority);
    return copied_priority;
}

void freeEventPriority(PQElementPriority event_priority) 
{
    if (event_priority == NULL) {
        return;
    }
    dateDestroy((Date) event_priority);
}

bool equalEventElement(PQElement event_element_one, PQElement event_element_two) 
{
    if (event_element_one == NULL || event_element_two == NULL) {
        return false;
    }
    if (((EventElement) event_element_one)->event_id == ((EventElement) event_element_two)->event_id) {
        return true;
    }
    return false;
}

int compareEventPriorities(PQElementPriority event_priority_one, PQElementPriority event_priority_two) 
{

    //If event2 after event1 returns 1-> event one is more important, and vice-versa
    return dateCompare((Date) event_priority_two, (Date) event_priority_one);
}

//Function for TeamMember Priority Queue:
PQElement copyTeamMemberElement(PQElement team_member_element) 
{
    if (team_member_element == NULL) {
        return NULL;
    }

    EventTeamMemberElement copied_member_element = malloc(sizeof(*copied_member_element));
    if (copied_member_element == NULL) {
        return NULL;
    }

    copied_member_element->member_id = ((EventTeamMemberElement) team_member_element)->member_id;
    copied_member_element->member_element_ptr = ((EventTeamMemberElement) team_member_element)->member_element_ptr;
    return copied_member_element;
}

PQElementPriority copyTeamMemberPriority(PQElementPriority team_member_priority) 
{
    if (team_member_priority == NULL) {
        return NULL;
    }

    int *copied_member_priority = malloc(sizeof(int));
    if (copied_member_priority == NULL) {
        return NULL;
    }

    *copied_member_priority = *((int *) team_member_priority);
    return copied_member_priority;
}

void freeEventTeamMemberElement(PQElement team_member_element) 
{
    if (team_member_element == NULL) {
        return;
    }
    ((EventTeamMemberElement) team_member_element)->member_element_ptr = NULL;
    free((EventTeamMemberElement) team_member_element);
}

void freeEventTeamMemberPriority(PQElementPriority team_member_priority) {
    if (team_member_priority == NULL) {
        return;
    }
    free((int *) team_member_priority);
}

bool equalEventTeamMemberElements(PQElement team_member_element_one, PQElement team_member_element_two) 
{
    if (team_member_element_one == NULL || team_member_element_two == NULL) {
        return false;
    }
    if (((EventTeamMemberElement) team_member_element_one)->member_id ==
        ((EventTeamMemberElement) team_member_element_two)->member_id) {
        return true;
    }
    return false;
}

int compareEventTeamMemberPriorities(PQElementPriority team_member_priority_one,
                                     PQElementPriority team_member_priority_two) 
{
    int priority_one_value = *((int *) team_member_priority_one);
    int priority_two_value = *((int *) team_member_priority_two);
    int compare_result = isBigger(priority_one_value, priority_two_value);
    if (compare_result == FIRST_ARGUMENT_IS_BIGGER) {
        return NEGATIVE_INTEGER;
    }
    if (compare_result == SECOND_ARGUMENT_IS_BIGGER) {
        return POSITIVE_INTEGER;
    }
    return SAME_ARGUMENTS;
}


/*-------------------------------------------------------------------------
    static functions implementation:
-------------------------------------------------------------------------*/
static EventElement createEventElement(Date date, char *event_name, int event_id) 
{
    EventElement event_element = malloc(sizeof(*event_element));
    if (event_element == NULL) {
        return NULL;
    }

    event_element->date = dateCopy(date);
    if (event_element->date == NULL) {
        free(event_element);
        return NULL;
    }
    event_element->event_name = copyName(event_name);
    if (event_element->event_name == NULL) {
        dateDestroy(event_element->date);
        free(event_element);
        return NULL;
    }

    event_element->event_id = event_id;

    event_element->event_member_team = pqCreate(&copyTeamMemberElement, &freeEventTeamMemberElement,
                                            &equalEventTeamMemberElements, &copyTeamMemberPriority,
                                            &freeEventTeamMemberPriority, &compareEventTeamMemberPriorities);
    if (event_element->event_member_team == NULL) {
        freeEventElement(event_element);
        return NULL;
    }

    return event_element;
}

static MemberPriority createMemberPriority(int member_id, int events_quantity) 
{
    MemberPriority member_priority = malloc(sizeof(*member_priority));
    if (member_priority == NULL) {
        return NULL;
    }

    member_priority->events_quantity = events_quantity;
    member_priority->member_id = member_id;

    return member_priority;
}

static MemberElement createMemberElement(int member_id, char *member_name) 
{
    MemberElement member_element = malloc(sizeof(*member_element));
    if (member_element == NULL) {
        return NULL;
    }

    char *copied_name = copyName(member_name);
    if (copied_name == NULL) {
        free(member_element);
        return NULL;
    }

    member_element->member_name = copied_name;
    member_element->member_id = member_id;
    member_element->events_quantity = NO_EVENTS;

    return member_element;
}

static EventTeamMemberElement createTeamMemberElement(int member_id, MemberElement member_element) 
{
    EventTeamMemberElement team_member_element = malloc(sizeof(*team_member_element));
    if (team_member_element == NULL) {
        return NULL;
    }

    team_member_element->member_id = member_id;
    team_member_element->member_element_ptr = member_element;
    return team_member_element;
}

static int *createTeamMemberPriority(int member_id) 
{
    int *team_member_priority = malloc(sizeof(*team_member_priority));
    if (team_member_priority == NULL) {
        return NULL;
    }
    *team_member_priority = member_id;
    return team_member_priority;
}

static PriorityQueueResult changeMemberPriority(EventManager em, MemberElement member_element,
                                                int old_events_quantity, int new_events_quantity) 
{
    if (em == NULL || member_element == NULL) {
        return PQ_NULL_ARGUMENT;
    }
    int member_id = member_element->member_id;

    //Create MemberPriority-type arguments to use at- pqChangePriority:
    MemberPriority old_member_priority = createMemberPriority(member_element->member_id, old_events_quantity);
    if (old_member_priority == NULL) {
        return PQ_OUT_OF_MEMORY;
    }

    MemberPriority new_member_priority = createMemberPriority(member_element->member_id, new_events_quantity);
    if (new_member_priority == NULL) {
        freeMemberPriority(old_member_priority);
        return PQ_OUT_OF_MEMORY;
    }

    PriorityQueueResult change_result = pqChangePriority(em->members, member_element, old_member_priority,
                                                         new_member_priority);

    //Re-get the member_element in case the generic function changes it address:
    MemberElement updated_member_element = getMemberElement(em->members, member_id);
    updated_member_element->events_quantity = new_events_quantity;

    /**Updating relevant pointers to main Member Element,in all events teams,
     * in case the generic function changes  member_element address
     **/
    PQ_FOREACH(EventElement, events_iterator, em->events) {
        PQ_FOREACH(EventTeamMemberElement, teams_iterator, events_iterator->event_member_team) {
            if (teams_iterator->member_id == member_id) {
                teams_iterator->member_element_ptr = updated_member_element;
                break;
            }
        }
    }

    freeMemberPriority(old_member_priority);
    freeMemberPriority(new_member_priority);

    return change_result;
}

static EventManagerResult removeTeamMemberFromEvent(EventManager em, EventElement event_element,
                                                    EventTeamMemberElement team_member_element)
{
    //Searching for the desired member:
    PriorityQueueResult change_result = PQ_NULL_ARGUMENT;
    int old_events_quantity = team_member_element->member_element_ptr->events_quantity;
    int new_events_quantity = old_events_quantity - 1;
    change_result = changeMemberPriority(em, team_member_element->member_element_ptr,
                                                  old_events_quantity, new_events_quantity);
    if (change_result == PQ_OUT_OF_MEMORY) {
        return EM_OUT_OF_MEMORY;
    }

    //To avoid unwanted freeing of MemberElement:
    team_member_element->member_element_ptr = NULL;
    pqRemoveElement(event_element->event_member_team, team_member_element);

    return EM_SUCCESS;
}

static char *copyName(char *name) 
{
    char *copied_name = malloc(((strlen(name) + 1)) * sizeof(char));
    if (copied_name == NULL) {
        return NULL;
    }
    strcpy(copied_name, name);

    return copied_name;
}

static EventElement getEventElement(PriorityQueue events_queue, int event_id) 
{
    PQ_FOREACH(EventElement, events_iterator, events_queue) {
        if (events_iterator->event_id == event_id) {
            return events_iterator;
        }
    }
    return NULL;
}

static MemberElement getMemberElement(PriorityQueue members_queue, int member_id) 
{
    PQ_FOREACH(MemberElement, members_iterator, members_queue) {
        if (members_iterator->member_id == member_id)
            return members_iterator;
    }
    return NULL;
}

static bool isEventNameAndDateExistInQueue(PriorityQueue events_queue, char *event_name, Date event_date) 
{
    PQ_FOREACH(EventElement, events_iterator, events_queue) {
        int compare_result = strcmp(events_iterator->event_name, event_name);
        if (compare_result == 0 && dateCompare(event_date, events_iterator->date) == 0) {
            return true;
        }
    }
    return false;
}

static bool isEventIdExistInQueue(PriorityQueue events_queue, int event_id) 
{
    PQ_FOREACH(EventElement, events_iterator, events_queue) {
        if (events_iterator->event_id == event_id) {
            return true;
        }
    }
    return false;
}

static bool isMemberIdExistInQueue(PriorityQueue members_queue, int member_id) 
{
    PQ_FOREACH(MemberElement, members_iterator, members_queue) {
        if (members_iterator->member_id == member_id) {
            return true;
        }
    }
    return false;
}

static bool isMemberLinkedToEvent(PriorityQueue members_queue, int member_id) 
{
    PQ_FOREACH(EventTeamMemberElement, members_iterator, members_queue) {
        if (members_iterator->member_id == member_id) {
            return true;
        }
    }
    return false;
}

static int isBigger(int argument_one, int argument_two) 
{
    if (argument_one > argument_two) {
        return FIRST_ARGUMENT_IS_BIGGER;
    }
    if (argument_one < argument_two) {
        return SECOND_ARGUMENT_IS_BIGGER;
    }
    return ARGUMENTS_IS_EQUAL;
}