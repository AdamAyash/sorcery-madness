#pragma once

#ifdef DEBUG
#define ASSERT(expression) \
    if (!(expression)) { \
        LOG_ERROR("Assertion failed: {0}, file {1}, line {2}", #expression, __FILE__, __LINE__); \
        __debugbreak(); \
    }
#else 
#define ASSERT(expression)
#endif 
