#include "pattern.h"
#include "user_types.h"

#include <stddef.h>
#include <string.h>

static struct user_event_wrapper * get_free_event(struct user_event_set * set) {
    for (size_t i = 0; i < set->count; i++) {
        if (set->wrappers[i].used) {
            continue;
        } else {
            return &set->wrappers[i];
        }
    }

    return NULL;
}

static struct user_event_wrapper * get_used_event(struct user_event_set * set) {
    struct user_event_wrapper * ev = NULL;

    for (size_t i = 0; i < set->count; i++) {
        size_t adjusted_ix = (i + set->next_get_ix) % set->count;

        if (!set->wrappers[adjusted_ix].used) {
            continue;
        } else {
            ev = &set->wrappers[adjusted_ix];
            set->next_get_ix = (set->next_get_ix + 1) % set->count;
            break;
        }
    }

    return ev;
}

enum pattern_status pattern_usr_write_event(void * param, void const * event) {
    struct user_event_set * const set = param;
    struct user_event_wrapper * const copy_to = get_free_event(set);

    if (NULL == copy_to) {
        return pattern_err;
    }

    copy_to->used = true;
    memcpy(&copy_to->event, event, sizeof(copy_to->event));

    return pattern_ok;
}

enum pattern_status pattern_usr_get_event(void * param, void * event) {
    struct user_event_set * const set = param;
    struct user_event_wrapper * const copy_from = get_used_event(set);

    if (NULL == copy_from) {
        return pattern_err;
    }

    copy_from->used = false;
    memcpy(event, &copy_from->event, sizeof(copy_from->event));

    return pattern_ok;
}
event_id pattern_usr_event_id(void * param, void const * event) {
    event_id id = (event_id)((struct user_event const *)event)->tag;
    PRASSERT(id <= 31);
    return id;
}
