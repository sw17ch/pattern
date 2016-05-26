#include <stdio.h>
#include "pattern.h"
#include "user_types.h"

#define ALEN(ARR) (sizeof(ARR) / sizeof(ARR[0]))

#define EVENT_COUNT (10)

struct task_description {
    struct pattern_task buff;
    char const * name;
    pattern_task_fn * entry;
};

static enum pattern_status task_tick(struct pattern_task * task);
static enum pattern_status task_thing(struct pattern_task * task);


static struct user_event_wrapper wrappers[EVENT_COUNT];
static struct user_event_set event_set = {
    .count = ALEN(wrappers),
    .next_get_ix = 0,
    .wrappers = wrappers,
};

static struct user_event sched_tmp_event;

static struct pattern sched;
static struct task_description task_data[] = {
    { .name = "tick", .entry = task_tick },
    { .name = "thing", .entry = task_thing },
};


int main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    // Initialize the scheduler first.
    pattern_sched_init(
        &sched,
        &event_set,
        &sched_tmp_event);

    // Once we have a scheduler, add all our tasks.
    for (size_t i = 0; i < ALEN(task_data); i++) {
        pattern_sched_add_task(
            &sched,
            &task_data[i].buff,
            task_data[i].name,
            task_data[i].entry);
    }

    struct user_event const initial_ev = {
        .tag = user_event_tick,
    };
    struct user_event ev;

    pattern_usr_write_event(&event_set, &initial_ev);
    pattern_usr_write_event(&event_set, &initial_ev);
    pattern_usr_write_event(&event_set, &initial_ev);
    pattern_usr_write_event(&event_set, &initial_ev);
    pattern_usr_write_event(&event_set, &initial_ev);
    pattern_usr_write_event(&event_set, &initial_ev);
    pattern_usr_write_event(&event_set, &initial_ev);

    while (true) {
        enum pattern_status const s = pattern_usr_get_event(&event_set, &ev);
        if (pattern_err == s) {
            break;
        }

        // Now create an iterator of all the tasks.
        struct pattern_task_iter iter;
        pattern_sched_task_iter_init(&sched, &iter);

        // Then we walk over each task in order until there are no
        // more to run.
        struct pattern_task * task = NULL;
        while ((task = pattern_sched_task_iter_next(&iter))) {

            enum pattern_status const run_stat =
                pattern_sched_run_task(task, &ev);

            switch (run_stat) {
            case pattern_err:
                printf("ERROR running %03zu/%s: %d\n",
                       task->id, task->name, run_stat);
                break;
            case pattern_err_skip:
                printf("No match %03zu/%s\n", task->id, task->name);
                break;
            case pattern_ok:
                if (sched.msg.len > 0) {
                    printf("MSG from %03zu/%s: %s\n",
                           task->id, task->name, sched.msg.chars);
                }
                break;
            }
        }
    }

    return 0;
}

static enum pattern_status task_tick(struct pattern_task * task) {
    uint64_t tick = 0;
    uint32_t const mask = 1 << user_event_tick;

    void * vout_event = NULL;
    bool event_ready = false;

    do {
        pattern_task_yield(task, mask, event_ready, NULL, &vout_event);

        struct user_event * const out_event = vout_event;

        out_event->tag = user_event_tick_count;
        out_event->u.tick_count = tick;
        tick += 1;
        event_ready = true;
    } while (true);


    while (true) {
    }

    return pattern_ok;
}

static enum pattern_status task_thing(struct pattern_task * task) {
    void const * vin_event = NULL;

    while (true) {
        uint32_t const mask = 1 << user_event_tick_count;
        pattern_task_yield(task, mask, false, &vin_event, NULL);

        struct user_event const * in_event = vin_event;

        TASK_PRINTF(task, "thing %llu", in_event->u.tick_count);
    }

    return pattern_ok;
}
