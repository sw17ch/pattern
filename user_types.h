#ifndef USER_TYPES_H
#define USER_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define USER_EVENT_COUNT (3)

enum user_event_tag {
    user_event_tick,
    user_event_tick_count,
};

struct user_event {
    enum user_event_tag tag;

    union {
        // No data for tick
        uint64_t tick_count;
    } u;
};

struct user_event_wrapper {
    bool used;
    struct user_event event;
};

struct user_event_set {
    size_t count;
    size_t next_get_ix;
    struct user_event_wrapper * wrappers;
};

#endif /* USER_TYPES_H */
