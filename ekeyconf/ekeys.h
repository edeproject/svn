// EControl applet for keyboard shortcuts
// Copyright (c) 2005. EDE Authors
// This program is licenced under terms of the
// GNU General Public Licence version 2 or newer.
// See COPYING for details

#ifndef EICON_H
#define EICON_H

#include <stdlib.h>
#include <efltk/Fl.h>
#include <efltk/fl_ask.h>
#include <efltk/Fl_Color_Chooser.h>
#include <efltk/x.h>
#include <efltk/Fl_Config.h>
#include <efltk/Fl_Locale.h>

int getshortcutfor(Fl_String);
void setshortcutfor(Fl_String, int);
void readKeysConfiguration();
void writeKeysConfiguration();
void sendUpdateInfo();
void populatelist(Fl_Menu_ *);

#endif

