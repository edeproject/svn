/*
 * $Id$
 *
 * Efinder, file search tool
 * Part of Equinox Desktop Environment (EDE).
 * glob for xfce Copyright 2000-2001 Edscott Wilson Garcia.
 * Copyright (C) 2001-2002 Martin Pekar.
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

int  process_find_messages();
void jam(char *file, Fl_Menu_ *);
void findCB();
void stopSearch();
void toggle_permission(long);

#endif

