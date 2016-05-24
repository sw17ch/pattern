#include <stdio.h>
#include "pattern.h"

struct pattern sched;
struct pattern_task task_buffers[1];

enum pattern_status task_entry(struct pattern_task * task) {
    size_t tick = 0;

    while (true) {
        TASK_PRINTF(task, "tick %zu", tick++);
        pattern_task_yield(task);
    }

    return pattern_ok;
}

int main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    pattern_sched_init(&sched);
    pattern_sched_add_task(&sched, &task_buffers[0], "first", task_entry);
    pattern_sched_add_task(&sched, &task_buffers[1], "second", task_entry);

    for (size_t i = 0; i < 100; i++) {
        struct pattern_task const * task = NULL;

        if (pattern_ok != pattern_sched_run_one(&sched, &task)) {
            return 1;
        } else {
            if (sched.msg.len > 0) {
                printf("MSG from %s: %s\n",
                       task->name, sched.msg.chars);
                sched.msg.len = 0;
            }
        }
    }

    return 0;
}
