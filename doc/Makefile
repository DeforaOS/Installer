TARGETS	= $(OBJDIR)installer.1 $(OBJDIR)installer.html
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

$(OBJDIR)installer.1: installer.xml
	./docbook.sh -P "$(PREFIX)" -- "$(OBJDIR)installer.1"

$(OBJDIR)installer.html: installer.css.xml installer.xml manual.css.xml
	./docbook.sh -P "$(PREFIX)" -- "$(OBJDIR)installer.html"

clean:
	$(RM) -- $(installer.1_OBJS) $(installer.html_OBJS)
	./docbook.sh -c -P "$(PREFIX)" -- "installer.1"
	./docbook.sh -c -P "$(PREFIX)" -- "installer.html"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -i -- "$(OBJDIR)installer.1"
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -i -- "$(OBJDIR)installer.html"

uninstall:
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -u -- "installer.1"
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -u -- "installer.html"

.PHONY: all clean distclean install uninstall
