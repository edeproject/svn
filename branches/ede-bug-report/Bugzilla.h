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

#ifndef __BUGZILLA_H__
#define __BUGZILLA_H__

#include <edelib/String.h>

struct BugzillaData;

BugzillaData  *bugzilla_new(const char *url);
void           bugzilla_free(BugzillaData *data);

edelib::String bugzilla_get_version(BugzillaData *data);

#endif
