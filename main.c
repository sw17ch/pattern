#include <stdio.h>
#include "pattern.h"

struct pattern pattern;
struct pattern_task task_buffers[1];

enum pattern_status task_entry(struct pattern_task * task) {
    size_t tick = 0;

    while (true) {
        TASK_PRINTF(task, "tick %zu", tick++);
        pattern_yield(task);
    }

    return pattern_ok;
}

int main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    pattern_init(&pattern);
    pattern_add_task(&pattern, &task_buffers[0], "first", task_entry);
    pattern_add_task(&pattern, &task_buffers[1], "second", task_entry);

    while (true) {
        if (pattern_ok != pattern_run_one(&pattern)) {
            return 1;
        }
    }

    return 0;
}
