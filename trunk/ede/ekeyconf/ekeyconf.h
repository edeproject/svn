/*
 * $Id$
 *
 * EControl applet for keyboard shortcuts
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef _EKEYCONF_H_
#define _EKEYCONF_H_

#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Input_Browser.h>
#include <efltk/Fl_Input.h>
#include "Shortcut_Button.h"

extern Fl_Window* iconsConfWindow;
extern Shortcut_Button* shortcut;
extern Fl_Input_Browser* action;
extern Fl_Window* newShortcutWindow;
extern Fl_Input* shortcutName;
extern Fl_Input* shortcutCommand;


#endif
