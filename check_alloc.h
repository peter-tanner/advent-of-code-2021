#include <stdlib.h>

#ifndef CHECK_ALLOC
#define CHECK_ALLOC(v)      \
    if ((v) == NULL)        \
    {                       \
        exit(EXIT_FAILURE); \
    }
#endif