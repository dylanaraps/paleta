#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>

#include "config.h"
#include "log.h"
#include "paleta.h"

void pal_read() {
    char c;
    int color = 0;
    int num = 0;
    int ret;

    while ((ret = fscanf(stdin, "%c", &c)) != EOF) {
        if (ret != 1) {
            die("failed to read from stdin");
        }

        if ((c >= 'A' && c <= 'F') ||
            (c >= 'a' && c <= 'f') ||
            (c >= '0' && c <= '9')) {

            if (color > MAX_COL) {
                die("invalid input found on stdin");
            }

            strcpy(&pal[num][color], &c);
            pal[num][MAX_COL] = 0;

            color++;

        } else if (c == '\n') {
           if (color < MAX_COL) {
               die("invalid input found on stdin");
           }

           color = 0;
           num++;

           if (num > MAX_PAL) {
               break;
           }

        } else if (c == '#') {
            continue;

        } else {
            die("invalid input found on stdin");
        }
    }

    pal_morph(num);
}

void seq_add(struct sequences *seq, const char *fmt,
             const int off, const char *col) {
    int ret;

    ret = snprintf(NULL, 0, fmt, off, col);

    if (!seq->size || seq->size + ret >= seq->cap) {
        seq->cap *= 2;
        seq->str  = realloc(seq->str, seq->cap);

        if (!seq->str) {
            die("failed to allocate memory");
            exit(1);
        }
    }

    ret = snprintf(seq->str + seq->size, ret, fmt, off, col);

    if (ret < 0) {
        die("failed to construct sequences");
    }

    seq->size += ret - 1;
}

void pal_morph(const int max_cols) {
    struct sequences seq = {
        .cap = 18, /* most frequent size */
    };

    seq_add(&seq, "\033]%d;#%s\033\\\\", 708, pal[0]);

    for (int i = 0; i < max_cols; i++) {
        char *fmt = i < 3
            ? "\033]%d;#%s\033\\\\"
            : "\033]4;%d;#%s\033\\\\";

        int off = i < 3 ? i + 10 : i - 3;

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

            msg("sent output to %s", buf.gl_pathv[i]);
        }
    }

    globfree(&buf);
}

int main(int argc, char **argv) {
    int flag = 0;

    if (argc > 1 && *++argv[1]) {
        flag = argv[1][0];
    }

    switch (flag) {
        case 'v':
            msg("paleta 0.1.0");
            break;

        case 0: {
            pal_read();
            break;
        }

        default:
            msg("usage: paleta -[hv] <stdin>\n");
            msg("send [#]ffffff\\n over <stdin>.");
            msg("- lines 1-3   = bg, fg, and cursor.");
            msg("- lines 4-%d = palette (0-%d).", MAX_PAL, MAX_PAL - 3);
            break;
    }

    return 0;
}
