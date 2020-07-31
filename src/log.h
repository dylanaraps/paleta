#include <stdio.h>
#include <stdlib.h>

#define msg(...) \
    do { \
        printf(__VA_ARGS__); \
        puts(""); \
    } while (0)

#define die(...) \
    do { \
        msg(__VA_ARGS__); \
        exit(1); \
    } while (0)
