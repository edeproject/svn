/*
 * $Id$
 *
 * Eiconsconf, icon settings tool
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef _EICONSCONF_H_
#define _EICONSCONF_H_

#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Value_Slider.h>
#include <efltk/Fl_Check_Button.h>

extern Fl_Window* iconsConfWindow;
extern Fl_Button* colorButton;
extern Fl_Button* colorButton1;
extern Fl_Value_Slider* maxWidthSlider;
extern Fl_Value_Slider* fontsizeSlider;
extern Fl_Value_Slider* gridspaceSlider;
extern Fl_Check_Button* autoArrButton;
extern Fl_Check_Button* engageButton;
extern Fl_Check_Button* bg_color_check;

void changeBoxColor(Fl_Button *box);

#endif
