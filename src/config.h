/* numerical terminal palette (0-256 by default) */
/* 3 reserved for bg, fg and cursor */
#define MAX_PAL 3 + 256

/* maximum length of color */
#define MAX_COL 6

/* file glob to match for directory entries */
/* macOS: /dev/ttys00[0-9] */
#define PTS_GLOB "/dev/pts/[0-9]*"
