#define _POSIX_C_SOURCE 200809L
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "paleta.h"

char *str_to_col(char *str) {
    size_t size;

    if (str[0] == '#') {
        ++str;
    }

    size = strcspn(str, "\n");
    str[size] = 0;

    if (size > MAX_COL || size < MAX_COL) {
        return NULL;
    }

    return str;
}

void pal_read() {
    char *line = 0;
    int i = 0;

    while ((getline(&line, &(size_t){0}, stdin) != -1)) {
        int ret;

        line = str_to_col(line);

        if (!line) {
            printf("invalid input found on stdin (line: %d)\n", i + 1);
            exit(1);
        }

        ret = snprintf(pal[i], MAX_COL + 1, "%s", line);

        if (ret < MAX_COL) {
            printf("failed to read input (line: %d\n", i + 1);
            exit(1);
        }

        if (++i == MAX_PAL) {
            break;
        }

        line = 0;
    }

    free(line);

    printf("read 3 special colors\n");
    printf("read %d/%d colors\n", i - 3, MAX_PAL -3);

    pal_morph(i);
}

void seq_add(struct sequences *seq, const char *fmt,
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

void pal_morph(const int max_cols) {
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

void pal_write(struct sequences *seq) {
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
