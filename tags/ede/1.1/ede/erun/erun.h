/*
 * $Id$
 *
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef erun_h
#define erun_h
#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
extern Fl_Window* windowRunDialog;
#include <efltk/Fl_Input.h>
extern Fl_Input* inputRunDialog;
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Button.h>
void run_resource(char *cmd);
#endif
