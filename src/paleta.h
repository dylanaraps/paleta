#include <stdio.h>

struct sequences {
    size_t size;
    size_t cap;
    char *str;
};

char pal[MAX_PAL][MAX_COL + 1];

void pal_read(void);
void pal_morph(const int max_cols);
void pal_write(struct sequences *seq);

void seq_add(struct sequences *seq, const char *fmt, \
             const int off, const char *col);

