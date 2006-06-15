/*
 * $Id$
 *
 * Colors settings
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef ecolorutils_h
#define ecolorutils_h

#include <efltk/Fl.h>
#include <efltk/Fl_Window.h>
#include <efltk/x.h>
#include <efltk/Fl_Menu_Button.h>
#include <efltk/Fl_Item_Group.h>
#include <efltk/Fl_Item.h>
#include <efltk/filename.h>
#include <efltk/Fl_Value_Output.h>
#include <efltk/Fl_Pack.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Divider.h>
#include <efltk/Fl_Image.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Radio_Button.h>
#include <efltk/Fl_Color_Chooser.h>
#include <efltk/Fl_Menu_Bar.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Input.h>
#include <efltk/Fl_Output.h>
#include <efltk/fl_ask.h>
#include <efltk/Fl_Value_Input.h>
#include <efltk/Fl_Font.h>
#include <efltk/Fl_Input_Browser.h>
#include <efltk/Fl_Config.h>
#include <efltk/Fl_Locale.h>

// this struct can hold slightly more information than Fl_Font
typedef struct {
	Fl_Font font;
	Fl_String encoding;
	int size;
	bool defined;
} EDEFont;


extern void updateFontAll();
extern void getSchemeColors();
extern void saveActiveScheme();
extern void saveSchemeAs();
extern void applyColors();
extern void fillItems();
extern void loadEfltkConfig();
extern void saveEfltkConfig();

// font stuff
Fl_String font_nice_name(EDEFont);
extern void labelfont_cb();
extern void textfont_cb();
extern EDEFont fl_font_dialog(EDEFont);

#endif
