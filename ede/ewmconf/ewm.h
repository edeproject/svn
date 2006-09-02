/*
 * $Id$
 *
 * Ewmconf, window manager configuration tool
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef ewm_h
#define ewm_h

#include <stdlib.h>
#include <efltk/Fl.h>
#include <efltk/fl_ask.h>
#include <efltk/Fl_Color_Chooser.h>
#include <efltk/Fl.h>
#include <efltk/x.h>
#include <efltk/Fl_Config.h>

extern Fl_Color title_active_color, title_active_color_text;
extern Fl_Color title_normal_color, title_normal_color_text;
extern bool opaque_resize;
extern int title_draw_grad;
extern bool animate;
extern int animate_speed;
extern bool use_frame;
extern Fl_Color theme_frame_color;
extern bool use_theme;
extern Fl_String theme_path;
extern int title_height;
extern int title_align;

void readConfiguration();
void applyConfiguration();
void writeConfiguration();

#endif

