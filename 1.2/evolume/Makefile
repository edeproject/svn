#
# $Id$
#
# Part of Equinox Desktop Environment (EDE).
# Copyright (c) 2000-2006 EDE Authors.
#
# This program is licenced under terms of the 
# GNU General Public Licence version 2 or newer.
# See COPYING for details.

CPPFILES = evolume.cpp prefs.cpp ../common/aboutdialog.cpp
TARGET   = evolume


POFILES = locale/ru.po\
		  locale/sk.po\
		  locale/hu.po\
		  locale/de.po\
		  locale/fr.po

include ../makeinclude

install:
	$(INSTALL_PROGRAM) $(TARGET) $(bindir)
	$(INSTALL_LOCALE)

uninstall:
	$(RM) $(bindir)/$(TARGET)

clean:
	$(RM) $(TARGET)
	$(RM) *.o

