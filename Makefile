.POSIX:

PREFIX       = /usr/local
ALL_WARNINGS = -Wall -Wextra -pedantic -Wmissing-prototypes -Wstrict-prototypes
ALL_CFLAGS   = $(CFLAGS) $(CPPFLAGS) -O0 -g -static -std=c99 $(ALL_WARNINGS)
ALL_LDFLAGS  = $(LDFLAGS) $(LIBS)

OBJ = src/log.o src/paleta.o
HDR = src/log.h

.c.o:
	$(CC) $(ALL_CFLAGS) -c -o $@ $<

paleta: $(OBJ)
	$(CC) $(ALL_CFLAGS) -o $@ $(OBJ) $(ALL_LDFLAGS)

$(OBJ): $(HDR)

install: paleta
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp paleta  $(DESTDIR)$(PREFIX)/bin/

clean:
	rm -f paleta $(OBJ)

.PHONY: install clean
