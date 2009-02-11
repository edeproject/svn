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

Data = ede-small.gif\
	   index.html\
	   authors.html\
	   changelog.html\
	   copying.html\
	   help.html\
	   logoinvert100.png\
	   getede.png\
	   ede.png\
	   style.css

DIR = $(datadir)/ede/doc

install:
	echo "Installing docs";
	$(MKINSTALLDIRS) $(DIR)
	for f in $(Data); do\
		$(INSTALL_DATA) $$f $(DIR);\
	done

uninstall:
	$(RM) -r $(DIR)

clean:
