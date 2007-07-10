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

#ifndef _EPANELCONF_H_
#define _EPANELCONF_H_

#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Check_Button.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Value_Slider.h>
#include <efltk/Fl_Input.h>

extern Fl_Window* panelWindow;
extern Fl_Check_Button* showdesktop_check;
extern Fl_Check_Button* workspace_check;
extern Fl_Check_Button* qlb_check;
extern Fl_Check_Button* runbrowser_check;
extern Fl_Check_Button* soundmixer_check;
extern Fl_Check_Button* cpumonitor_check;
extern Fl_Check_Button* autohide_check;
extern Fl_Value_Slider* ws_slider;
extern Fl_Input* browserProgram;
extern Fl_Input* terminalProgram;
extern Fl_Input* vcProgram;
extern Fl_Input* tdProgram;


#endif
