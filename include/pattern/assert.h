#ifndef PATTERN_ASSERT_H
#define PATTERN_ASSERT_H

#include <assert.h>

#define PSASSERT(EXP) \
    _Static_assert(EXP, #EXP)

#define PRASSERT(EXP) \
    assert(EXP)


#endif /* PATTERN_ASSERT_H */
