#include "pattern/ports/osx.h"

static void * pattern_surrogate(void * arg) {
    struct pattern_task * t = arg;

    pthread_mutex_lock(&t->port.mutex_run);
    pthread_mutex_unlock(&t->port.mutex_running);
    t->entry(t);

    return NULL;
}

enum pattern_status pattern_port_start_task(
    struct pattern_task * t) {
    pthread_mutex_init(&t->port.mutex_run, NULL);
    pthread_mutex_init(&t->port.mutex_running, NULL);
    pthread_mutex_init(&t->port.mutex_yielded, NULL);

    pthread_mutex_lock(&t->port.mutex_run);

    pthread_create(&t->port.thread, NULL, pattern_surrogate, t);
}

enum pattern_status pattern_port_run_task(struct pattern_task * t) {
    // Lock the running and yielded mutexes so that the scheduled task
    // must unlock them before we can continue.
    pthread_mutex_lock(&t->port.mutex_running);
    pthread_mutex_lock(&t->port.mutex_yielded);

    // Unlock the run mutex so that the scheduled task can run. Wait
    // for the task to enter the running state before waiting for it
    // to yield.
    pthread_mutex_unlock(&t->port.mutex_run);
    pthread_mutex_lock(&t->port.mutex_running);
    pthread_mutex_lock(&t->port.mutex_yielded);

    // Unlock the running and yielded mutexes to return to our initial condition.
    pthread_mutex_unlock(&t->port.mutex_running);
    pthread_mutex_unlock(&t->port.mutex_yielded);

    return pattern_ok;
}

enum pattern_status pattern_port_task_yield(struct pattern_task * t) {
    // Unlock the yielded mutex to indicate that the task is done.
    pthread_mutex_unlock(&t->port.mutex_yielded);

    // Resume waiting for the task to be resumed by the scheduler.
    pthread_mutex_lock(&t->port.mutex_run);

    // Inform the scheduler that the task is now running.
    pthread_mutex_unlock(&t->port.mutex_running);
}
