// Icons settings for EDE
// Copyright (C) 2000-2002 Martin Pekar
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

#ifndef EICON_H
#define EICON_H

#include <stdlib.h>
#include <efltk/Fl.h>
#include <efltk/fl_ask.h>
#include <efltk/Fl_Color_Chooser.h>
#include <efltk/x.h>
#include <efltk/Fl_Config.h>
#include <efltk/Fl_Locale.h>

extern int label_background;
extern int label_foreground;
extern int label_fontsize;
extern int label_maxwidth;
extern int label_gridspacing;
extern bool label_trans;
extern bool label_engage_1click;
extern bool auto_arr;
	    
void
 readIconsConfiguration()
;
void
 writeIconsConfiguration()
;
void sendUpdateInfo();

#endif

