#
# $Id$
#
# Part of Equinox Desktop Environment (EDE).
# Copyright (c) 2000-2006 EDE Authors.
#
# This program is licenced under terms of the 
# GNU General Public Licence version 2 or newer.
# See COPYING for details.

CPPFILES = \
		   debug.cpp         \
		   main.cpp          \
		   Windowmanager.cpp \
		   Hints.cpp         \
		   Atoms.cpp         \
		   Titlebar.cpp      \
		   Frame.cpp         \
		   Cursor.cpp        \
		   Events.cpp        \
		   Utils.cpp

TARGET   = edewm

include ../makeinclude

install:
	$(INSTALL_PROGRAM) $(TARGET) $(bindir)
	$(INSTALL_LOCALE)

uninstall:
	$(RM) $(bindir)/$(TARGET)

clean:
	$(RM) $(TARGET)
	$(RM) *.o

archive:
	DATE=`date +%d%m%Y`; \
	cd ..; \
	tar -cjpvf edewm-unfinished-$$DATE.tar.bz2 new
# DO NOT DELETE

Atoms.o: Atoms.h /usr/include/X11/Xlib.h /usr/include/sys/types.h
Atoms.o: /usr/include/features.h /usr/include/sys/cdefs.h
Atoms.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
Atoms.o: /usr/include/bits/wordsize.h
Atoms.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stddef.h
Atoms.o: /usr/include/bits/typesizes.h /usr/include/time.h
Atoms.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
Atoms.o: /usr/include/X11/Xosdefs.h
Cursor.o: Cursor.h Frame.h /usr/include/X11/Xlib.h /usr/include/sys/types.h
Cursor.o: /usr/include/features.h /usr/include/sys/cdefs.h
Cursor.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
Cursor.o: /usr/include/bits/wordsize.h
Cursor.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stddef.h
Cursor.o: /usr/include/bits/typesizes.h /usr/include/time.h
Cursor.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
Cursor.o: /usr/include/X11/Xosdefs.h /usr/include/X11/cursorfont.h
Cursor.o: /usr/include/assert.h
Events.o: Events.h /usr/include/X11/Xlib.h /usr/include/sys/types.h
Events.o: /usr/include/features.h /usr/include/sys/cdefs.h
Events.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
Events.o: /usr/include/bits/wordsize.h
Events.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stddef.h
Events.o: /usr/include/bits/typesizes.h /usr/include/time.h
Events.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
Events.o: /usr/include/X11/Xosdefs.h Frame.h Titlebar.h Windowmanager.h
Events.o: Tracers.h debug.h Atoms.h Hints.h /usr/include/assert.h
Frame.o: Frame.h /usr/include/X11/Xlib.h /usr/include/sys/types.h
Frame.o: /usr/include/features.h /usr/include/sys/cdefs.h
Frame.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
Frame.o: /usr/include/bits/wordsize.h
Frame.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stddef.h
Frame.o: /usr/include/bits/typesizes.h /usr/include/time.h
Frame.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
Frame.o: /usr/include/X11/Xosdefs.h Events.h Hints.h Windowmanager.h Atoms.h
Frame.o: Utils.h debug.h Titlebar.h Tracers.h /usr/include/assert.h mask.xpm
Hints.o: Hints.h /usr/include/X11/Xlib.h /usr/include/sys/types.h
Hints.o: /usr/include/features.h /usr/include/sys/cdefs.h
Hints.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
Hints.o: /usr/include/bits/wordsize.h
Hints.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stddef.h
Hints.o: /usr/include/bits/typesizes.h /usr/include/time.h
Hints.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
Hints.o: /usr/include/X11/Xosdefs.h Tracers.h debug.h Frame.h Atoms.h
Hints.o: Windowmanager.h /usr/include/X11/Xproto.h /usr/include/X11/Xmd.h
Hints.o: /usr/include/X11/Xprotostr.h /usr/include/assert.h
Titlebar.o: Titlebar.h Frame.h /usr/include/X11/Xlib.h
Titlebar.o: /usr/include/sys/types.h /usr/include/features.h
Titlebar.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Titlebar.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
Titlebar.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stddef.h
Titlebar.o: /usr/include/bits/typesizes.h /usr/include/time.h
Titlebar.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
Titlebar.o: /usr/include/X11/Xosdefs.h debug.h /usr/include/assert.h
Utils.o: Utils.h debug.h /usr/include/X11/Xlib.h /usr/include/sys/types.h
Utils.o: /usr/include/features.h /usr/include/sys/cdefs.h
Utils.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
Utils.o: /usr/include/bits/wordsize.h
Utils.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stddef.h
Utils.o: /usr/include/bits/typesizes.h /usr/include/time.h
Utils.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
Utils.o: /usr/include/X11/Xosdefs.h
Windowmanager.o: Windowmanager.h Atoms.h /usr/include/X11/Xlib.h
Windowmanager.o: /usr/include/sys/types.h /usr/include/features.h
Windowmanager.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Windowmanager.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
Windowmanager.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stddef.h
Windowmanager.o: /usr/include/bits/typesizes.h /usr/include/time.h
Windowmanager.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
Windowmanager.o: /usr/include/X11/Xosdefs.h Hints.h Frame.h ../exset/exset.h
Windowmanager.o: /usr/include/stdio.h /usr/include/libio.h
Windowmanager.o: /usr/include/_G_config.h /usr/include/wchar.h
Windowmanager.o: /usr/include/bits/wchar.h /usr/include/gconv.h
Windowmanager.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stdarg.h
Windowmanager.o: /usr/include/bits/stdio_lim.h
Windowmanager.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
Windowmanager.o: Tracers.h debug.h /usr/include/assert.h
Windowmanager.o: /usr/include/X11/Xproto.h /usr/include/X11/Xmd.h
Windowmanager.o: /usr/include/X11/Xprotostr.h /usr/include/X11/cursorfont.h
debug.o: debug.h /usr/include/stdio.h /usr/include/features.h
debug.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
debug.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stddef.h
debug.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
debug.o: /usr/include/bits/typesizes.h /usr/include/libio.h
debug.o: /usr/include/_G_config.h /usr/include/wchar.h
debug.o: /usr/include/bits/wchar.h /usr/include/gconv.h
debug.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stdarg.h
debug.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
main.o: Windowmanager.h debug.h /usr/include/stdio.h /usr/include/features.h
main.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
main.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stddef.h
main.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
main.o: /usr/include/bits/typesizes.h /usr/include/libio.h
main.o: /usr/include/_G_config.h /usr/include/wchar.h
main.o: /usr/include/bits/wchar.h /usr/include/gconv.h
main.o: /usr/lib/gcc-lib/i486-slackware-linux/3.3.6/include/stdarg.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
main.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/signal.h
main.o: /usr/include/bits/sigset.h /usr/include/bits/signum.h
