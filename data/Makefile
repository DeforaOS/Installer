OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
MKDIR	= mkdir -m 0755 -p
INSTALL	= install
RM	= rm -f


all:

clean:

distclean: clean

install:
	$(MKDIR) $(DESTDIR)$(PREFIX)/share/applications
	$(INSTALL) -m 0644 deforaos-installer.desktop $(DESTDIR)$(PREFIX)/share/applications/deforaos-installer.desktop

uninstall:
	$(RM) -- $(DESTDIR)$(PREFIX)/share/applications/deforaos-installer.desktop

.PHONY: all clean distclean install uninstall
