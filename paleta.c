#define _POSIX_C_SOURCE 200809L
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

static int  pal_read(void);
static void pal_morph(const int max_cols);
static void pal_write(void);

static char pal[MAX_PAL][MAX_COL + 1];
static char seq[MAX_SEQ + 1]; /* todo lol */

static int pal_read() {
    char *line = 0;
    int i = 0;

    while ((getline(&line, &(size_t){0}, stdin) != -1)) {
        size_t size;
        int ret;

        if (line[0] == '#') {
            ++line;
        }

        size = strcspn(line, "\n");
        line[size] = 0;

        if (size > MAX_COL || size == 0) {
            printf("invalid input found on stdin (line: %d)\n", i + 1);
            exit(1);
        }

        ret = snprintf(pal[i], MAX_COL + 1, "%s", line);

        if (ret < 0) {
            printf("failed to read from stdin\n");
            exit(1);
        }

        if (ret < MAX_COL) {
            printf("invalid input found on stdin (line: %d)\n", i + 1);
            exit(1);
        }

        if (++i == MAX_PAL) {
            break;
        }

        line = 0;
    }

    free(line);

    printf("bg = %s, fg = %s, \033[7m \033[m = %s\n",
        pal[0], pal[1], pal[2]);
    printf("colors %d/%d\n", i - 3, MAX_PAL - 3);

    return i;
}

static void pal_morph(const int max_cols) {
    int ret;

    ret = snprintf(NULL, 0, FMT_708, pal[0]);

    if (ret < 0) {
        printf("failed to construct sequences\n");
        exit(1);
    }

    ret = snprintf(seq, ret, FMT_708, pal[0]);

    for (int i = 0; i < max_cols; i++) {
        size_t seq_l;
        char *fmt = i < 3 ? FMT_SPE : FMT_NUM;
        int   off = i < 3 ? i + 10  : i - 3;

        ret = snprintf(NULL, 0, fmt, off, pal[i]);

        if (ret < 0) {
            printf("failed to construct sequences\n");
            exit(1);
        }

        seq_l = strlen(seq);

        if (seq_l + ret > MAX_SEQ + 1) {
            printf("sequences exceed maximum\n");
            exit(1);
        }

        ret = snprintf(seq + seq_l, ret, fmt, off, pal[i]);
    }

    seq[MAX_SEQ] = 0;
}

static void pal_write() {
    glob_t buf;

    glob(PTS_GLOB, GLOB_NOSORT, NULL, &buf);

    for (size_t i = 0; i < buf.gl_pathc; i++) {
        FILE *file = fopen(buf.gl_pathv[i], "w");

        if (file) {
            fprintf(file, "%s", seq);
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

        case 0:
            ret = pal_read();
            pal_morph(ret);
            pal_write();
            break;

        default:
            printf("usage: paleta -[hv] <stdin>\n\n");
            printf("send [#]ffffff\\n over <stdin>.\n");
            printf("- lines 1-3   = bg, fg, and cursor.\n");
            printf("- lines 4-%d = palette (0-%d).\n", MAX_PAL, MAX_PAL - 3);
            break;
    }

    return 0;
}
