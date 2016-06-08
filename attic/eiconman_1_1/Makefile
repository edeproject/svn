#
# $Id: Makefile 1652 2006-06-07 19:37:51Z karijes $
#
# Part of Equinox Desktop Environment (EDE).
# Copyright (c) 2000-2006 EDE Authors.
#
# This program is licenced under terms of the 
# GNU General Public Licence version 2 or newer.
# See COPYING for details.

CPPFILES = icondialog.cpp eiconman.cpp edeskicon.cpp wallpaper.cpp
LOCALFLAGS = -DLOCAL_VECTOR
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
