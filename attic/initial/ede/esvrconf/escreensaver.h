// Equinox Desktop Environment - screensaver configuration
// Copyright (C) 2002 Martin Pekar
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

#ifndef escreensaver_h
#define escreensaver_h

#include <stdio.h>
#include <stdlib.h>

#include <efltk/Fl_Window.h>
#include <efltk/Fl_Item.h>
#include <efltk/x.h>
#include <efltk/Fl_Input_Browser.h>
#include <efltk/Fl_Locale.h>

void fillSaversList(Fl_Input_Browser *);
void startSaverPreview();
void readConfiguration();
void writeConfiguration();
void writeConfigurationSaver();
void getScreenhacks();
void clearOnExit();

#endif



