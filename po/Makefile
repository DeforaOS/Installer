TARGETS	= $(OBJDIR)Installer.pot $(OBJDIR)fr.mo
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

$(OBJDIR)Installer.pot: POTFILES
	./gettext.sh -P "$(PREFIX)" -- "$(OBJDIR)Installer.pot"

$(OBJDIR)fr.mo: $(OBJDIR)Installer.pot fr.po
	./gettext.sh -P "$(PREFIX)" -- "$(OBJDIR)fr.mo"

clean:
	$(RM) -- $(Installer.pot_OBJS) $(fr.mo_OBJS)
	./gettext.sh -c -P "$(PREFIX)" -- "Installer.pot"
	./gettext.sh -c -P "$(PREFIX)" -- "fr.mo"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	./gettext.sh -P "$(DESTDIR)$(PREFIX)" -i -- "$(OBJDIR)fr.mo"

uninstall:
	./gettext.sh -P "$(DESTDIR)$(PREFIX)" -u -- "fr.mo"

.PHONY: all clean distclean install uninstall
