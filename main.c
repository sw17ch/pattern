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

    // Now that the scheduler has tasks, we're going to do a
    // single-step of the scheduler in a loop.
    for (size_t i = 0; i < 100; i++) {
        struct pattern_task const * task = NULL;

        // Step the scheduler once. Capture a reference to the task
        // that the scheduler tried to run.
        enum pattern_status const run_stat =
            pattern_sched_run_one(&sched, &task);

        if (pattern_ok != run_stat) {
            // If running the task failed for some reason, report the
            // error on the console.
            printf("ERROR running %s (%zu): %d\n",
                   task->name,
                   task->id,
                   run_stat);
        } else {
            // If running the task succeeded, print any message it
            // reported on the console.
            if (sched.msg.len > 0) {
                printf("MSG from %s (%zu): %s\n",
                       task->name,
                       task->id,
                       sched.msg.chars);
            }
        }
    }

    return 0;
}
