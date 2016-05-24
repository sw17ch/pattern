#ifndef PATTERN_PORTS_OSX_TYPES_H
#define PATTERN_PORTS_OSX_TYPES_H

#include "pattern/types.h"

#include <pthread.h>

struct pattern_task_port {
    pthread_t thread;
    pthread_mutex_t mutex_run;
};

struct pattern_port {
    pthread_mutex_t mutex_running;
    pthread_mutex_t mutex_yielded;
};

#endif /* PATTERN_PORTS_OSX_TYPES_H */
