#ifndef PATTERN_TYPES_H
#define PATTERN_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "pattern_config.h"
#include "pattern/ports/port_types.h"

enum pattern_status {
    pattern_ok,
    pattern_err,
};

struct pattern_msg_buf {
    size_t const capacity;
    char * const chars;
    int len;
};

#define TASK_PRINTF(TASK, FMT, ...)                             \
    do {                                                        \
        TASK->pat->msg.len = snprintf(TASK->pat->msg.chars,     \
                                      TASK->pat->msg.capacity,  \
                                      FMT,                      \
                                      __VA_ARGS__);             \
    } while (0)


struct pattern_task;

typedef enum pattern_status pattern_task_fn (
    struct pattern_task * task);

struct pattern_task {
    struct pattern * pat;
    struct pattern_task * next;
    size_t id;
    char const * const name;
    pattern_task_fn * entry;
    bool dead;

    struct pattern_task_port port;
};

struct pattern {
    struct pattern_task * head;
    struct pattern_task * tail;

    struct pattern_task * next;

    size_t next_id;

    struct pattern_msg_buf msg;
    char msg_chars[MAX_MSG_LEN];
};

#endif /* PATTERN_TYPES_H */
