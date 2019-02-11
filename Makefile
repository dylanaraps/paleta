PREFIX ?= /usr
MANDIR ?= $(PREFIX)/share/man
DOCDIR ?= $(PREFIX)/share/doc/pal

all:
	@echo Run \'make install\' to install pal.

install:
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@mkdir -p $(DESTDIR)$(MANDIR)/man1
	@mkdir -p $(DESTDIR)$(DOCDIR)
	@cp -p pal $(DESTDIR)$(PREFIX)/bin/pal
	@cp -p pal.1 $(DESTDIR)$(MANDIR)/man1
	@cp -p README.md $(DESTDIR)$(DOCDIR)
	@chmod 755 $(DESTDIR)$(PREFIX)/bin/pal

uninstall:
	@rm -rf $(DESTDIR)$(PREFIX)/bin/pal
	@rm -rf $(DESTDIR)$(MANDIR)/man1/pal.1
	@rm -rf $(DESTDIR)$(DOCDIR)
