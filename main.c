#include <stdio.h>
#include "pattern.h"

#define ALEN(ARR) (sizeof(ARR) / sizeof(ARR[0]))

static enum pattern_status task_entry(struct pattern_task * task);
static enum pattern_status task_other_entry(struct pattern_task * task);

static struct {
    struct pattern_task buff;
    char const * name;
    pattern_task_fn * entry;
} task_data[] = {
    { .name = "t1", .entry = task_entry },
    { .name = "t2", .entry = task_entry },
    { .name = "t3", .entry = task_other_entry },
};

static struct pattern sched;

static enum pattern_status task_entry(struct pattern_task * task) {
    size_t tick = 0;

    while (true) {
        TASK_PRINTF(task, "tick %zu", tick++);
        pattern_task_yield(task);
    }

    return pattern_ok;
}

static enum pattern_status task_other_entry(struct pattern_task * task) {
    size_t tick = 0;

    while (true) {
        TASK_PRINTF(task, "other tick %zu", tick++);
        pattern_task_yield(task);
    }

    return pattern_ok;
}

int main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    pattern_sched_init(&sched);

    for (size_t i = 0; i < ALEN(task_data); i++) {
        pattern_sched_add_task(
            &sched,
            &task_data[i].buff,
            task_data[i].name,
            task_data[i].entry);
    }

    for (size_t i = 0; i < 100; i++) {
        struct pattern_task const * task = NULL;

        if (pattern_ok != pattern_sched_run_one(&sched, &task)) {
            return 1;
        } else {
            if (sched.msg.len > 0) {
                printf("MSG from %s (%zu): %s\n",
                       task->name,
                       task->id,
                       sched.msg.chars);
                sched.msg.len = 0;
            }
        }
    }

    return 0;
}
