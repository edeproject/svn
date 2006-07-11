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

define make_test
ALL += $1
$1 : $1.cpp
	echo Compiling $$<...
	$(CXX) $(CXXFLAGS) $$< -o $$@ $(LIBS)
endef

# Note: $(eval...) must be used instead plain $(call...)
# since variables will not be expanded correctly

$(eval $(call make_test, basic_moveresize))

all: $(ALL)

clean:
	$(RM) $(ALL)
	$(RM) *.o
