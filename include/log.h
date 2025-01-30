#ifndef __LOG_H__
#define __LOG_H__

#include <sys/types.h>
#include <stdio.h>

#define CMDBUFLEN 100

#define print_debug(str, ...)                       \
    printf(str" - %s:%u\n", ##__VA_ARGS__, __FILE__, __LINE__);

#define print_err(str, ...)                     \
    fprintf(stderr, str, ##__VA_ARGS__);

#endif