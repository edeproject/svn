#
# $Id: Makefile 1652 2006-06-07 19:37:51Z karijes $
#
# Part of Equinox Desktop Environment (EDE).
# Copyright (c) 2000-2006 EDE Authors.
#
# This program is licenced under terms of the 
# GNU General Public Licence version 2 or newer.
# See COPYING for details.

CPPFILES = icontooltip.cpp eiconman.cpp edeskicon.cpp
TARGET   = eiconman

#POFILES = locale/ru.po\
#			locale/sr.po\
#			locale/sk.po\
#			locale/hu.po\

include ../makeinclude

install:
	$(INSTALL_PROGRAM) $(TARGET) $(bindir)
	$(INSTALL_LOCALE)

uninstall:
	$(RM) $(bindir)/$(TARGET)

clean:
	$(RM) $(TARGET)
	$(RM) *.o

# DO NOT DELETE

edeskicon.o: edeskicon.h icontooltip.h eiconman.h /usr/include/assert.h
edeskicon.o: /usr/include/features.h /usr/include/sys/cdefs.h
edeskicon.o: /usr/include/gnu/stubs.h
eiconman.o: eiconman.h edeskicon.h icontooltip.h /usr/include/signal.h
eiconman.o: /usr/include/features.h /usr/include/sys/cdefs.h
eiconman.o: /usr/include/gnu/stubs.h /usr/include/bits/sigset.h
eiconman.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
eiconman.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stddef.h
eiconman.o: /usr/include/bits/typesizes.h /usr/include/bits/signum.h
eiconman.o: /usr/include/stdio.h /usr/include/libio.h
eiconman.o: /usr/include/_G_config.h /usr/include/wchar.h
eiconman.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eiconman.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stdarg.h
eiconman.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
eiconman.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
eiconman.o: /usr/include/bits/confname.h /usr/include/locale.h
eiconman.o: /usr/include/bits/locale.h /usr/include/assert.h
icontooltip.o: icontooltip.h /usr/include/assert.h /usr/include/features.h
icontooltip.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
icontooltip.o: /usr/include/stdio.h
icontooltip.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stddef.h
icontooltip.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
icontooltip.o: /usr/include/bits/typesizes.h /usr/include/libio.h
icontooltip.o: /usr/include/_G_config.h /usr/include/wchar.h
icontooltip.o: /usr/include/bits/wchar.h /usr/include/gconv.h
icontooltip.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stdarg.h
icontooltip.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
