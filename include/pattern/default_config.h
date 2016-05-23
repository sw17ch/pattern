#ifndef PATTERN_DEFAULT_CONFIG_H
#define PATTERN_DEFAULT_CONFIG_H

#include <stdint.h>
#include "pattern/assert.h"

#ifndef YIELDTYPE
#define YIELDTYPE uint32_t
#endif

PSASSERT(sizeof(YIELDTYPE) > 0);

#ifndef YIELDCOUNT
#define YIELDCOUNT 1
#endif

PSASSERT(YIELDCOUNT > 0);

#ifndef MAX_MSG_LEN
#define MAX_MSG_LEN 1024
#endif

PSASSERT(MAX_MSG_LEN > 0);

#endif /* PATTERN_DEFAULT_CONFIG_H */
