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

