#include "pattern.h"
#include "pattern/ports/port.h"

void pattern_sched_init(struct pattern * pat) {
    *pat = (struct pattern) {
        .head = NULL,
        .tail = NULL,
        .next = NULL,
        .next_id = 0,
        .msg = {
            .capacity = MAX_MSG_LEN,
            .chars = pat->msg_chars,
            .len = 0,
        },
    };

    // TODO: propagate error codes from this call
    pattern_port_sched_init(pat);
}

void pattern_sched_add_task(struct pattern * pat, struct pattern_task * buf, char const * name, pattern_task_fn * task) {
    PRASSERT(pat);
    PRASSERT(buf);
    PRASSERT(name);
    PRASSERT(task);
    PRASSERT(buf->next == NULL);

    *buf = (struct pattern_task) {
        .pat = pat,
        .next = NULL,
        .id = pat->next_id++,
        .name = name,
        .entry = task,
        .dead = false,
    };

    if (NULL == pat->head) {
        pat->head = pat->tail = buf;
    } else {
        pat->tail->next = buf;
        pat->tail = buf;
    }

    pattern_port_create_task(buf);
}

enum pattern_status pattern_sched_run_one(struct pattern * pat, struct pattern_task const ** task) {
    PRASSERT(pat);
    PRASSERT(pat->head);

    if (pat->next == NULL) {
        pat->next = pat->head;
    }

    struct pattern_task * const t = pat->next;

    if (NULL != task) {
        *task = t;
    }

    // Reset the message buffer length so that any message from the
    // previous task is cleared.
    pat->msg.len = 0;

    // Capture the status of running the task.
    enum pattern_status const run_stat = pattern_port_run_task(t);

    // Always advance the task. If one task failed to run, it's
    // possible others may succeed.
    pat->next = t->next;

    return run_stat;
}

void pattern_task_yield(struct pattern_task const * task) {
    struct pattern_task * const mut_task = (struct pattern_task *)task;
    pattern_port_task_yield(mut_task);
}
