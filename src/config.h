/* numerical terminal palette (0-256 by default) */
/* 3 reserved for bg, fg and cursor */
#define MAX_PAL 3 + 256

/* maximum length of color */
#define MAX_COL 6

/* file glob to match for directory entries */
#define PTS_GLOB "/dev/pts/[0-9]*"

/* there should be no reason to change these */
#define FMT_SPE "\033]%d;#%s\033\\\\"
#define FMT_NUM "\033]4;%d;#%s\033\\\\"
#define FMT_708 "\033]%d;#%s\033\\\\"
