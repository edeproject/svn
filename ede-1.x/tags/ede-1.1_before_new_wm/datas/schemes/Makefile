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

schemesdir = $(datadir)/ede/.ede/schemes

SCHEMES     = \
	Active.scheme \
	Blueworld.scheme \
	Brown1.scheme \
	Brown2.scheme \
	Ede1.scheme \
	Ede2.scheme \
	Ede3.scheme \
	Futuristic.scheme \
	GrayBrown.scheme \
	Gray1.scheme \
	LightBrown.scheme \
	Lilola.scheme \
	None.scheme \
	Tubus.scheme \
	TypicalBrown.scheme 

install:
	$(MKINSTALLDIRS) $(schemesdir);\
	for f in $(SCHEMES); do\
		echo "Installing $(schemesdir)/$$f";\
		$(INSTALL_DATA) $$f $(schemesdir);\
	done

uninstall:
	$(RM) -r $(schemesdir)

clean:
