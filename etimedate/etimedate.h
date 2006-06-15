/*
 * $Id$
 *
 * Etimedate, time-date configuration tool
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef _ETIMEDATE_H_
#define _ETIMEDATE_H_

#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Calendar.h>
#include <efltk/Fl_Clock.h>
#include "fl_time.h"
#include <efltk/Fl_Choice.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Input_Browser.h>

extern Fl_Time* timeBox;
extern Fl_Choice* formatChoice;
extern Fl_Input_Browser* timeZonesList;


#endif
