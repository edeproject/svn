/*
 * $Id$
 *
 * ede-bug-report, a tool to report bugs on EDE bugzilla instance
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2009 EDE Authors.
 *
 * This program is licensed under terms of the 
 * GNU General Public License version 2 or newer.
 * See COPYING for details.
 */

#ifndef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include "Bugzilla.h"

#define EDE_BUGZILLA_URL "http://bugs.equinox-project.org/xmlrpc.cgi"

EDELIB_NS_USING(String)

int main(int argc, char** argv) {
	BugzillaData *data = bugzilla_new(EDE_BUGZILLA_URL);
	if(!data)
		return 1;

	String v = bugzilla_get_version(data);
	printf("EDE Bugzilla version: %s\n", v.c_str());

	bugzilla_free(data);
	printf("%p\n", data);

	return 0;
}

