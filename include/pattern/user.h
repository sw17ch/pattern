#ifndef PATTERN_USER_H
#define PATTERN_USER_H

#include <stdint.h>

/**
 * @breif Write an event into the user-specified event set.
 *
 * Called from user tasks to add a new event to the event buffer. The
 * event is copied into the buffer so the event can be created in
 * temporary memory.
 */
enum pattern_status pattern_usr_write_event(void * param, void const * event);

/**
 * @breif Read an event from the user-specified event set.
 *
 * Called from scheduler code in order to pass the event to all tasks
 * pending on the event.
 */
enum pattern_status pattern_usr_get_event(void * param, void * event);

/**
 * @breif Extract the event ID from the event.
 *
 * Converts an event into an event ID that can be used to create a
 * yield mask. This call does not have a failure case.
 *
 * Currently, event IDs can range from 0 to 31.
 */
event_id pattern_usr_event_id(void * param, void const * event);

#endif /* PATTERN_USER_H */
