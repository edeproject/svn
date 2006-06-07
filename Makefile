#
# $Id$
#
# Part of Equinox Desktop Environment (EDE).
# Copyright (c) 2000-2006 EDE Authors.
#
# This program is licenced under terms of the 
# GNU General Public Licence version 2 or newer.
# See COPYING for details.

# main file for making

include makeinclude

DIRS =	common\
		ecolorconf\
		econtrol\
		efinder\
		eiconman\
		eiconsconf\
		einstaller\
		ekeyconf\
		elauncher\
		emenueditor\
		epanelconf\
		erun\
		esvrconf\
		etimedate\
		etip\
		ewmconf\
		eworkpanel\
		edisplayconf\
		edewm\
		datas\
		datas/programs-links\
		datas/desktop-links\
		datas/icons-16\
		datas/icons-48\
		datas/schemes\
		docs/ede $(EVOLUME)

all: makeinclude
	for dir in $(DIRS); do\
		echo "Going to $$dir...";\
		(cd $$dir; $(MAKE) $(MFLAGS)) || exit;\
	done
	@@echo "Now please run 'make install' as root"

# each submakefile should implement install and clean

.PHONY: clean 

install: makeinclude
	for dir in $(DIRS); do\
		echo "Installing from $$dir...";\
		(cd $$dir; $(MAKE) $(MFLAGS) install) || exit;\
	done

uninstall: makeinclude
	for dir in $(DIRS); do\
		echo "Uninstalling $$dir...";\
		(cd $$dir; $(MAKE) $(MFLAGS) uninstall) || exit;\
	done

clean: makeinclude
	for dir in $(DIRS); do\
		echo "Cleaning $$dir...";\
		(cd $$dir; $(MAKE) $(MFLAGS) clean) || exit;\
	done

depend: makeinclude
	for dir in $(DIRS); do\
		echo "Creating dependencies in $$dir...";\
		(cd $$dir; $(MAKEDEPEND) -- $(CXXFLAGS) *.cpp) || exit;\
	done
