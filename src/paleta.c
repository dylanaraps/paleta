#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>

#include "config.h"

struct sequences {
    size_t size;
    size_t cap;
    char *str;
};

char pal[MAX_PAL][MAX_COL + 1];

static void pal_read(void);
static void pal_morph(const int max_cols);
static void pal_write(struct sequences *seq);

static void seq_add(struct sequences *seq, const char *fmt, \
             const int off, const char *col);

static void pal_read() {
    char c;
    int color = 0;
    int num = 0;

    while (fscanf(stdin, "%c", &c) != EOF) {
       if ((c >= 'A' && c <= 'F') ||
           (c >= 'a' && c <= 'f') ||
           (c >= '0' && c <= '9')) {

           if (color > MAX_COL) {
               printf("invalid input found on stdin\n");
               exit(1);
           }

           strcpy(&pal[num][color], &c);
           pal[num][MAX_COL] = 0;

           color++;

       } else if (c == '\n') {
           if (color < MAX_COL) {
               printf("invalid input found on stdin\n");
               exit(1);
           }

           color = 0;
           num++;

           if (num > MAX_PAL) {
               break;
           }

       } else if (c == '#') {
           continue;

       } else {
           printf("invalid input found on stdin\n");
           exit(1);
       }
    }

    pal_morph(num);
}

static void seq_add(struct sequences *seq, const char *fmt,
             const int off, const char *col) {
    int ret;

    ret = snprintf(NULL, 0, fmt, off, col);

    if (!seq->size || seq->size + ret >= seq->cap) {
        seq->cap *= 2;
        seq->str  = realloc(seq->str, seq->cap);

        if (!seq->str) {
            printf("failed to allocate memory\n");
            exit(1);
        }
    }

    ret = snprintf(seq->str + seq->size, ret, fmt, off, col);

    if (ret < 0) {
        printf("failed to construct sequences\n");
        exit(1);
    }

    seq->size += ret - 1;
}

static void pal_morph(const int max_cols) {
    struct sequences seq = {
        .cap = 18, /* most frequent size */
    };

    seq_add(&seq, FMT_708, 708, pal[0]);

    for (int i = 0; i < max_cols; i++) {
        char *fmt = i < 3 ? FMT_SPE : FMT_NUM;
        int   off = i < 3 ? i + 10  : i - 3;

        seq_add(&seq, fmt, off, pal[i]);
    }

    pal_write(&seq);
    free(seq.str);
}

static void pal_write(struct sequences *seq) {
    glob_t buf;

    glob(PTS_GLOB, GLOB_NOSORT, NULL, &buf);

    for (size_t i = 0; i < buf.gl_pathc; i++) {
        FILE *file = fopen(buf.gl_pathv[i], "w");

        if (file) {
            fprintf(file, "%s", seq->str);
            fclose(file);

            printf("sent output to %s\n", buf.gl_pathv[i]);
        }
    }

    FILE *f = fopen("/tmp/paleta", "w");

    if (f) {
        fprintf(f, "%s", seq->str);
        fclose(f);
    }

    globfree(&buf);
}

int main(int argc, char **argv) {
    int ret = 0;

    if (argc > 1 && *++argv[1]) {
        ret = argv[1][0];
    }

    switch (ret) {
        case 'v':
            printf("paleta 0.1.0\n");
            break;

        case 0: {
            pal_read();
            break;
        }

        default:
            printf("usage: paleta -[hv] <stdin>\n\n");
            printf("send [#]ffffff\\n over <stdin>.\n");
            printf("- lines 1-3   = bg, fg, and cursor.\n");
            printf("- lines 4-%d = palette (0-%d).\n", MAX_PAL, MAX_PAL - 3);
            break;
    }

    return 0;
}
