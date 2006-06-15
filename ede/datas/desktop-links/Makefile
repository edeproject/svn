#
# $Id$
#
# Part of Equinox Desktop Environment (EDE).
# Copyright (c) 2000-2006 EDE Authors.
#
# This program is licenced under terms of the 
# GNU General Public Licence version 2 or newer.
# See COPYING for details.

include ../../makeinclude

desktopdir = $(datadir)/ede/.ede/desktop

DESKTOPLINKS     = \
	Terminal.desktop

install:
	$(MKINSTALLDIRS) $(desktopdir)
	$(INSTALL_DATA) $(DESKTOPLINKS) $(desktopdir)

uninstall:
	$(RM) -r $(desktopdir)

clean:
