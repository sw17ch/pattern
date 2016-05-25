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
 * @breif Get an iterator for the tasks in the scheduler.
 *
 * This lets the host program iterate over tasks in the scheduler.
 *
 * @param pat The scheduler from which to build the iterator.
 * @param iter The iterator to be populated.
 */
void pattern_sched_task_iter_init(
    struct pattern * pat,
    struct pattern_task_iter * iter);

/**
 * @breif Return a task and advance the iterator
 *
 * Return the current task referenced by the iterator and advance the
 * iterator to the next task. The return value will be NULL when the
 * last value has already been returned.
 *
 * The iterator can be reset using @c pattern_sched_task_iter_init.
 *
 * @param iter The iterator to use.
 */
struct pattern_task * pattern_sched_task_iter_next(
    struct pattern_task_iter * iter);

/**
 * @breif Run the specified task
 *
 * This runs the specified task.
 *
 * @param task The task to run.
 */
enum pattern_status pattern_sched_run_task(
    struct pattern_task * task);

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
