// Pattern
//
// A cooperative scheduling system.

#ifndef PATTERN_H
#define PATTERN_H

#include "pattern_config.h"
#include "pattern/types.h"

void pattern_init(struct pattern * pat);

void pattern_add_task(
    struct pattern * pat,
    struct pattern_task * buf,
    char const * name,
    pattern_task_fn * task);

void pattern_yield(struct pattern_task const * task);

enum pattern_status pattern_run_one(struct pattern * pat);

#endif /* PATTERN_H */
