#ifndef ASSERTUTIL_H_
#define ASSERTUTIL_H_

#include <iostream>
#include "Logging/Logger.h"

#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            LOG_ERROR << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::exit(EXIT_FAILURE); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

#define AssertNotReached() ASSERT(false,"AssertNotReached");
#define AssertSwitchBadDefault(Value) ASSERT(false,"In default case in switch("#Value ")with value" << (int)Value);

#endif /* ASSERTUTIL_H_ */
