PREFIX ?= /usr
MANDIR ?= $(PREFIX)/share/man

all:
	@echo Run \'make install\' to install paleta.

install:
	@install -Dm755 paleta   $(DESTDIR)$(PREFIX)/bin/paleta
	@install -Dm644 paleta.1 $(DESTDIR)$(MANDIR)/man1/paleta.1

uninstall:
	@rm -f $(DESTDIR)$(PREFIX)/bin/paleta
	@rm -f $(DESTDIR)$(MANDIR)/man1/paleta.1
