#include <stdio.h>

struct buf {
    size_t size;
    size_t cap;
    char *str;
};

char pal[MAX_PAL + 1][MAX_COL + 1];

void pal_read(void);
void pal_morph(const int );
void pal_write(const char *str);

void seq_add(struct buf *, const char *, const int, const char *);
