/*
 * $Id$
 *
 * Eiconman, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef propdialog_h
#define propdialog_h
#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include "edeskicon.h"
extern Fl_Window* i_propertieswindow;
#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Group.h>
extern Fl_Group* i_icontab;
#include <efltk/Fl_Box.h>
extern Fl_Box* pr_icon;
extern Fl_Box* pr_name;
extern Fl_Box* pr_size;
extern Fl_Box* pr_exec;
extern Fl_Group* i_settingstab;
#include <efltk/Fl_Output.h>
extern Fl_Output* i_link;
#include <efltk/Fl_Input.h>
extern Fl_Input* i_name;
extern Fl_Input* i_location;
#include <efltk/Fl_Button.h>
extern Fl_Input* i_filename;
void property_dialog(Fl_Widget *, Icon *icon, bool show_settings);
#endif
