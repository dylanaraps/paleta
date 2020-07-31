
void _log(const char *, ...);

#define msg(...) _log(__VA_ARGS__)
#define die(...) _log(__VA_ARGS__),exit(1)
