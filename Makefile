.POSIX:

PREFIX       = /usr/local
ALL_WARNINGS = -Wall -Wextra -pedantic -Wmissing-prototypes -Wstrict-prototypes
ALL_CFLAGS   = $(CFLAGS) $(CPPFLAGS) -std=c99 -O3 -static $(ALL_WARNINGS)
ALL_LDFLAGS  = $(LDFLAGS) $(LIBS)

all:
	$(CC) $(ALL_CFLAGS) -o paleta src/paleta.c $(ALL_LDFLAGS)

install:
	mkdir -p  $(DESTDIR)$(PREFIX)/bin
	cp paleta $(DESTDIR)$(PREFIX)/bin/

clean:
	rm -f paleta $(OBJ)

.PHONY: install clean
