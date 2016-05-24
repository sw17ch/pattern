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
}

void pattern_sched_add_task(struct pattern * pat, struct pattern_task * buf, char const * name, pattern_task_fn * task) {
    PRASSERT(pat);
    PRASSERT(buf);
    PRASSERT(name);
    PRASSERT(task);
    PRASSERT(buf->next == NULL);

    if (NULL == pat->head) {
        pat->head = pat->tail = buf;
    } else {
        pat->tail->next = buf;
    }

    *buf = (struct pattern_task) {
        .pat = pat,
        .next = NULL,
        .id = pat->next_id++,
        .name = name,
        .entry = task,
        .dead = false,
    };

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

    pat->msg.len = 0;
    pattern_port_run_task(t);

    pat->next = t->next;

    return pattern_ok;
}

void pattern_task_yield(struct pattern_task const * task) {
    struct pattern_task * const mut_task = (struct pattern_task *)task;
    pattern_port_task_yield(mut_task);
}
