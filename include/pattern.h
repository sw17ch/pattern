// Pattern
//
// A cooperative scheduling system.

#ifndef PATTERN_H
#define PATTERN_H

#include "pattern_config.h"
#include "pattern/types.h"

/**
 * @breif Initialize a @c pattern structure.
 *
 * Use this function to create a new pattern scheduler.
 *
 * @param pat The object to initialize.
 */
void pattern_sched_init(
    struct pattern * pat);

/**
 * @breif Add a task to a scheduler.
 *
 * Use this function to add another task to an exsiting scheduler.
 *
 * @param pat The scheduler to which the task is added.
 * @param buf The task buffer to use for the task.
 * @param name The name of the task.
 * @param task The entry point of the task.
 */
void pattern_sched_add_task(
    struct pattern * pat,
    struct pattern_task * buf,
    char const * name,
    pattern_task_fn * task);

/**
 * @breif Run the next task until it yields.
 *
 * Use this function to run a single step of the scheduler. This will
 * run the next task until it yields.
 *
 * @param pat The scheduler to run.
 * @param task An optional reference to the task that was run.
 */
enum pattern_status pattern_sched_run_one(
    struct pattern * pat,
    struct pattern_task const ** task);

/**
 * @breif Yield the CPU from inside the task.
 *
 * Use this to yield control back to the scheduler from inside a task.
 *
 * @param task The reference to the running task.
 */
void pattern_task_yield(
    struct pattern_task const * task);

#endif /* PATTERN_H */
