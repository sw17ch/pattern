#include "pattern.h"
#include "pattern/ports/port.h"

void pattern_init(struct pattern * pat) {
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

void pattern_add_task(struct pattern * pat, struct pattern_task * buf, char const * name, pattern_task_fn * task) {
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

    pattern_port_start_task(buf);
}

void pattern_yield(struct pattern_task const * task) {
    struct pattern_task * const mut_task = (struct pattern_task *)task;
    pattern_port_task_yield(mut_task);
}

enum pattern_status pattern_run_one(struct pattern * pat) {
    PRASSERT(pat);
    PRASSERT(pat->head);

    if (pat->next == NULL) {
        pat->next = pat->head;
    }

    struct pattern_task * const t = pat->next;

    pattern_port_run_task(t);

    if (pat->msg.len > 0) {
        printf("MSG from %s: %s\n", t->name, pat->msg.chars);
        pat->msg.len = 0;
    }

    pat->next = t->next;

    return pattern_ok;
}
