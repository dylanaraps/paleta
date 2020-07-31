#include <stdio.h>
#include <stdarg.h>

#include "log.h"

void _log(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    puts("");
}
