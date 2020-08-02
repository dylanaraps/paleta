#define msg(...) \
    do { \
        fprintf(stderr, __VA_ARGS__); \
        puts(""); \
    } while (0)

#define die(...) \
    do { \
        msg(__VA_ARGS__); \
        exit(EXIT_FAILURE); \
    } while (0)
