// Copyright (c) 2000. - 2005. EDE Authors
// This program is licenced under terms of the
// GNU General Public Licence version 2 or newer.
// See COPYING for details.

#ifndef prefs_h
#define prefs_h
#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
void choice_items(char *path);
#include <efltk/Fl_Window.h>
extern Fl_Window* preferenciesWindow;
#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Input_Browser.h>
extern Fl_Input_Browser* deviceNameInput;
#include <efltk/Fl_Button.h>
#include <fcntl.h>
extern void update_info();
void PreferencesDialog(Fl_Widget *, void *);
#endif
