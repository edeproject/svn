// Window manager settings for EDE
// Copyright (C) 2001-2003 Martin Pekar
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

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

