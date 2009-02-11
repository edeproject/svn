/*
 * $Id$
 *
 * Epanelconf, panel configuration tool
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef epanelconf_h
#define epanelconf_h
#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
extern Fl_Window* panelWindow;
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Input.h>
extern Fl_Input* vcProgram;
extern Fl_Input* tdProgram;
#include <efltk/Fl_Check_Button.h>
extern Fl_Check_Button* autohide_check;
#include <efltk/Fl_Value_Slider.h>
extern Fl_Value_Slider* ws_slider;
extern Fl_Input* browserProgram;
extern Fl_Input* terminalProgram;
#endif
