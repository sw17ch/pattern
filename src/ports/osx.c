#include "pattern/ports/port.h"
#include "pattern/ports/osx_types.h"

enum pattern_status pattern_port_sched_init(struct pattern * p) {
    pthread_mutex_init(&p->port.mutex_running, NULL);
    pthread_mutex_init(&p->port.mutex_yielded, NULL);

    return pattern_ok;
}

enum pattern_status pattern_port_create_task(struct pattern_task * t) {
    struct pattern * const sched = t->pat;

    // Create the thread's run mutex.
    pthread_mutex_init(&t->port.mutex_run, NULL);

    // Lock it initially to keep the task from from proceeding past
    // its first yield. Also lock the yield mutex so that we can
    // detect when the task has yielded for the first time. Once it
    // yields, the task has completed its startup operations and we
    // can return to the scheduler.
    pthread_mutex_lock(&t->port.mutex_run);
    pthread_mutex_lock(&sched->port.mutex_yielded);

    // Create the thread that hosts the task's stack.
    pthread_create(
        &t->port.thread, NULL,
        (void *(*)(void *))t->entry, t);

    // We acquire another lock on the yield mutex so that we're
    // guaranteed that the task is yielded.
    pthread_mutex_lock(&sched->port.mutex_yielded);

    // Once we get the yield lock, we can return all the mutexes to
    // their required state for the next scheduling event.
    pthread_mutex_unlock(&sched->port.mutex_running);
    pthread_mutex_unlock(&sched->port.mutex_yielded);


    return pattern_ok;
}

enum pattern_status pattern_port_run_task(struct pattern_task * t) {
    struct pattern * const sched = t->pat;

    // Lock the running and yielded mutexes so that the scheduled task
    // must unlock them before we can continue.
    pthread_mutex_lock(&sched->port.mutex_running);
    pthread_mutex_lock(&sched->port.mutex_yielded);

    // Unlock the run mutex so that the scheduled task can run. Wait
    // for the task to enter the running state before waiting for it
    // to yield.
    pthread_mutex_unlock(&t->port.mutex_run);
    pthread_mutex_lock(&sched->port.mutex_running);
    pthread_mutex_lock(&sched->port.mutex_yielded);

    // Unlock the running and yielded mutexes to return to our initial condition.
    pthread_mutex_unlock(&sched->port.mutex_running);
    pthread_mutex_unlock(&sched->port.mutex_yielded);

    return pattern_ok;
}

enum pattern_status pattern_port_task_yield(struct pattern_task * t) {
    struct pattern * const sched = t->pat;

    // Unlock the yielded mutex to indicate that the task is done.
    pthread_mutex_unlock(&sched->port.mutex_yielded);

    // Resume waiting for the task to be resumed by the scheduler.
    pthread_mutex_lock(&t->port.mutex_run);

    // Inform the scheduler that the task is now running.
    pthread_mutex_unlock(&sched->port.mutex_running);

    return pattern_ok;
}
