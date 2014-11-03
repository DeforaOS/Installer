TARGETS	= $(OBJDIR)installer
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF=
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags libDesktop`
CFLAGS	= -Wall -g -O2
LDFLAGSF= `pkg-config --libs libDesktop`
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

installer_OBJS = $(OBJDIR)installer.o $(OBJDIR)main.o
installer_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
installer_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)installer: $(installer_OBJS)
	$(CC) -o $(OBJDIR)installer $(installer_OBJS) $(installer_LDFLAGS)

$(OBJDIR)installer.o: installer.c installer.h ../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(installer_CFLAGS) -o $(OBJDIR)installer.o -c installer.c

$(OBJDIR)main.o: main.c installer.h ../config.h
	$(CC) $(installer_CFLAGS) -o $(OBJDIR)main.o -c main.c

clean:
	$(RM) -- $(installer_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(OBJDIR)installer $(DESTDIR)$(BINDIR)/installer

uninstall:
	$(RM) -- $(DESTDIR)$(BINDIR)/installer

.PHONY: all clean distclean install uninstall
