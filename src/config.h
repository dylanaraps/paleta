/* numerical terminal palette (0-256 by default) */
/* 3 reserved for bg, fg and cursor */
#define MAX_PAL 3 + 256

/* maximum length of color */
#define MAX_COL 6

/* file glob to match for directory entries */
/* See: https://man7.org/linux/man-pages/man7/pty.7.html */
/* macOS: /dev/ttys00[0-9] */
#define PTS_GLOB "/dev/pts/[0-9]*"
