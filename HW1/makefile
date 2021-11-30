CC = gcc
OBJS_ONE = event_manager_tests.o event_manager.o date.o
EXEC_ONE = event_manager
OBJS_TWO = priority_queue_tests.o priority_queue.o
EXEC_TWO = priority_queue
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 --pedantic-errors -Wall -Werror
PQ_LINK_FLAG = -L. -lpriority_queue

$(EXEC_ONE): $(OBJS_ONE)
	$(CC) $(DEBUG_FLAG) $(OBJS_ONE) -o $@ $(PQ_LINK_FLAG)

$(EXEC_TWO): $(OBJS_TWO)
	$(CC) $(DEBUG_FLAG) $(OBJS_TWO) -o $@

event_manager.o: event_manager.c priority_queue.h date.h event_manager.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

priority_queue.o: priority_queue.c priority_queue.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

date.o: date.c date.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

event_manager_tests.o: tests/event_manager_tests.c tests/test_utilities.h event_manager.h date.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tests/event_manager_tests.c

priority_queue_tests.o: tests/priority_queue_tests.c tests/test_utilities.h priority_queue.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tests/priority_queue_tests.c

clean:
	rm -f $(OBJS_ONE) $(OBJS_TWO) $(EXEC_ONE) $(EXEC_TWO)
	