#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <glob.h>
#include <unistd.h>

#include "config.h"
#include "log.h"
#include "paleta.h"

void pal_read() {
    char c;
    int color = 0;
    int num = 0;

    while ((c = fgetc(stdin)) != EOF) {
        if ((c >= 'A' && c <= 'F') ||
            (c >= 'a' && c <= 'f') ||
            (c >= '0' && c <= '9')) {

            if (color > MAX_COL) {
                die("invalid input found on stdin");
            }

            memset(&pal[num][color], c, 1);
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

    if (!seq->size || (seq->size + ret) >= seq->cap) {
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

    char *fmt_spe = "\033]%d;#%s\033\\\\";
    char *fmt_pal = "\033]4;%d;#%s\033\\\\";

    /* see: 1a651cf990e67c4046fbff7674249259bcaa89a8 */
    seq_add(&seq, "\033]%d;#%s\033\\\\", 708, pal[1]);

    for (int i = 3; i < max_cols; i++) {
        seq_add(&seq, fmt_pal, i - 3, pal[i]);

        /* some terminals require that these sequences go
         * after colors 0-16. other terminals flicker if
         * these sequences are sent too late */
        switch (i - 3) {
            case 0:
                /* background */
                seq_add(&seq, fmt_spe, 11, pal[1]);
                break;

            case 15:
                /* foreground, cursor */
                seq_add(&seq, fmt_spe, 10, pal[0]);
                seq_add(&seq, fmt_spe, 12, pal[2]);
                break;
        }
    }

    pal_write(&seq);
    free(seq.str);
}

void pal_write(struct sequences *seq) {
    glob_t buf;

    glob(PTS_GLOB, GLOB_NOSORT, NULL, &buf);

    for (size_t i = 0; i < buf.gl_pathc; i++) {
        int f = open(buf.gl_pathv[i], O_WRONLY | O_NONBLOCK);

        if (f) {
            write(f, seq->str, seq->size);
            close(f);

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
            msg("paleta 0.2.0");
            break;

        case 0: {
            pal_read();
            break;
        }

        default:
            msg("usage: paleta -[hv] <stdin>\n");
            msg("send [#]ffffff\\n over <stdin>.");
            msg("- lines 1-3   = fg, bg, and cursor.");
            msg("- lines 4-%d = palette (0-%d).", MAX_PAL, MAX_PAL - 3);
            break;
    }

    return 0;
}
