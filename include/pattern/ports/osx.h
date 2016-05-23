#ifndef PATTERN_PORTS_OSX_H
#define PATTERN_PORTS_OSX_H

#include "pattern/types.h"
#include "pattern/ports/osx.h"

enum pattern_status pattern_port_start_task(
    struct pattern_task * p);

enum pattern_status pattern_port_run_task(
    struct pattern_task * p);

enum pattern_status pattern_port_task_yield(
    struct pattern_task * p);

#endif /* PATTERN_PORTS_OSX_H */
