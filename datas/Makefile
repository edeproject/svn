#
# $Id$
#
# Part of Equinox Desktop Environment (EDE).
# Copyright (c) 2000-2006 EDE Authors.
#
# This program is licenced under terms of the 
# GNU General Public Licence version 2 or newer.
# See COPYING for details.

include ../makeinclude

Data = ede.conf wmanager.conf mime.conf efltk.conf
DIR = $(datadir)/ede/.ede

install:
	$(MKINSTALLDIRS) $(DIR)
	$(INSTALL_DATA) ede.conf $(DIR)
	$(INSTALL_DATA) wmanager.conf $(DIR)
	$(INSTALL_DATA) mime.conf $(DIR)
	$(INSTALL_DATA) efltk.conf $(DIR)
	$(MKINSTALLDIRS) $(bindir)
	$(INSTALL_PROGRAM) startede $(bindir)

uninstall:
	$(RM) -r $(DIR)
	$(RM) $(bindir)/startede

clean:
