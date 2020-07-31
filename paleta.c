#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <fnmatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PAL 259   /* 3 + 256 */
#define MAX_COL 7
#define MAX_SEQ 20

#define PTS_DIR  "/dev/pts"
#define PTS_GLOB "[0-9]*"

static char palette[MAX_PAL][MAX_COL];

static int   pal_read(void);
static char *pal_morph(const int max_cols);
static void  pal_write(char *seq);

static int pal_read() {
    char *line = 0;
    int i = 0;
    size_t size;
    int ret;

    while ((getline(&line, &(size_t){0}, stdin) != -1)) {
        if (line[0] == '#') {
            ++line;
        }

        size = strcspn(line, "\n");
        line[size] = 0;

        if (size > MAX_COL) {
            printf("invalid input found on stdin (line: %d)\n", i + 1);
            exit(1);
        }

        ret = snprintf(palette[i], MAX_COL, "%s", line);

        if (ret < 0) {
            printf("failed to read from stdin\n");
            exit(1);
        }

        if (ret + 1 < MAX_COL) {
            printf("invalid input found on stdin (line: %d)\n", i + 1);
            exit(1);
        }

        if (++i == MAX_PAL) {
            break;
        }

        line = 0;
    }

    free(line);

    if (i == 0) {
        printf("error: Failed to read from stdin\n");
        exit(1);
    }

    printf("bg = %s, fg = %s, \033[7m \033[m = %s\n",
        palette[0], palette[1], palette[2]);
    printf("colors %d/%d\n", i - 3, MAX_PAL - 3);

    return i;
}

static char *pal_morph(const int max_cols) {
    char *seq;
    int ret;

    seq = calloc(MAX_SEQ * (max_cols + 1), 1);

    if (!seq) {
        printf("failed to allocate memory\n");
        exit(1);
    }

    for (int i = 0; i < max_cols; i++) {
        if (i < 3) {
            ret = snprintf(seq + strlen(seq), MAX_SEQ,
                    "\033]%d;#%s\033\\", i +10, palette[i]);
        } else {
            ret = snprintf(seq + strlen(seq), MAX_SEQ,
                    "\033]4;%d;#%s\033\\", i - 3, palette[i]);
        }

        if (ret < 0) {
            printf("failed to construct sequences\n");
            exit(1);
        }
    }

    return seq;
}

static void pal_write(char *seq) {
    struct dirent *dp;
    DIR *dir;
    FILE *file;

    dir = opendir(PTS_DIR);

    if (!dir) {
        printf("failed to open %s.\n", PTS_DIR);
        exit(1);
    }

    if (chdir(PTS_DIR) == -1) {
        closedir(dir);
        printf("failed to open %s.\n", PTS_DIR);
        exit(1);
    }

    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_name[0] == '.') {
            continue;
        }

        if (fnmatch("[0-9]*", dp->d_name, 0) != 0) {
            continue;
        }

        file = fopen(dp->d_name, "w");

        if (!file) {
            continue;
        }

        fprintf(file, "%s", seq);
        fclose(file);
    }

    free(seq);
    closedir(dir);
}

int main(int argc, char **argv) {
    int ret = 0;
    char *seq;

    if (argc > 1 && *++argv[1]) {
        ret = argv[1][0];
    }

    switch (ret) {
        case 'v':
            printf("paleta 0.1.0\n");
            break;

        case 'h':
            printf("usage: paleta -[hv] <stdin>\n\n");
            printf("send [#]ffffff\\n over <stdin>.\n");
            printf("lines 1-3 are bg, fg, and cursor.\n");
            printf("lines 4-%d are the numerical palette.\n", MAX_PAL);
            break;

        default:
            ret = pal_read();
            seq = pal_morph(ret);
            pal_write(seq);
    }

    return 0;
}
