#include <stdio.h>
#include "pattern.h"

#define ALEN(ARR) (sizeof(ARR) / sizeof(ARR[0]))

static enum pattern_status task_entry(struct pattern_task * task);
static enum pattern_status task_other_entry(struct pattern_task * task);

struct task_description {
    struct pattern_task buff;
    char const * name;
    pattern_task_fn * entry;
};

static struct task_description task_data[] = {
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

    // Initialize the scheduler first.
    pattern_sched_init(&sched);

    // Once we have a scheduler, add all our tasks.
    for (size_t i = 0; i < ALEN(task_data); i++) {
        pattern_sched_add_task(
            &sched,
            &task_data[i].buff,
            task_data[i].name,
            task_data[i].entry);
    }

    // Now that the scheduler has tasks, we're going to perform
    // several iterations over all the tasks.
    for (size_t i = 0; i < 10; i++) {
        // We start by creating an iterator of all the tasks.
        struct pattern_task_iter iter;
        pattern_sched_task_iter_init(&sched, &iter);

        // Then we walk over each task in order until there are no
        // more to run.
        struct pattern_task * task = NULL;
        while ((task = pattern_sched_task_iter_next(&iter))) {
            enum pattern_status const run_stat =
                pattern_sched_run_task(task);

            if (pattern_ok != run_stat) {
                printf("ERROR running %03zu/%s: %d\n",
                       task->id, task->name, run_stat);
            } else {
                if (sched.msg.len > 0) {
                    printf("MSG from %03zu/%s: %s\n",
                           task->id, task->name, sched.msg.chars);
                }
            }
        }
    }

    return 0;
}
