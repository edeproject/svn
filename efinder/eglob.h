/*
 * $Id$
 *
 * Efinder, file search tool
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef EGLOB_H_
#define EGLOB_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Util.h>

#define FILE_TYPES_NR 9

static char *ftypes[FILE_TYPES_NR] =
{
    _("Any kind"),
    _("Regular file"),
    _("Directory"),
    _("Symbolic link"),
    _("Socket"),
    _("Block device"),
    _("Character device"),
    _("FIFO"),
    NULL
};


int  process_find_messages();
void jam(char *file, Fl_Menu_ *);
void findCB();
void stopSearch();
void pauseSearch();
void toggle_permission(long);

#endif

