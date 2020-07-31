/* maximum colors to read */
/* + 3 special colors (bg, fg, cursor) */
/* + numerical terminal palette (256 by default) */
#define MAX_PAL 259

/* maximum length of color (with \0) */
#define MAX_COL 7

/* maximum length of sequence (with \0) */
#define MAX_SEQ 18

/* directory containing terminal devices */
#define PTS_DIR  "/dev/pts"

/* file glob to match for directory entries */
#define PTS_GLOB "[0-9]*"

/* there should be no reason to change these */
#define FMT_SPE "\033]%d;#%s\033\\"
#define FMT_NUM "\033]4;%d;#%s\033\\"
#define FMT_708 "\033]708;#%s\033\\"
