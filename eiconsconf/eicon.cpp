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

#include "eicon.h"

int label_background =  46848
;
int label_foreground = FL_WHITE;
int label_fontsize = 12;
int label_maxwidth = 75;
int label_gridspacing = 16
;
bool label_trans = true;
bool label_engage_1click = false;
bool auto_arr = false;

static void sendClientMessage(Window w, Atom a, long x)
{
  XEvent ev;
  long mask;

  memset(&ev, 0, sizeof(ev));
  ev.xclient.type = ClientMessage;
  ev.xclient.window = w;
  ev.xclient.message_type = a;
  ev.xclient.format = 32;
  ev.xclient.data.l[0] = x;
  ev.xclient.data.l[1] = CurrentTime;
  mask = 0L;
  if (w == RootWindow(fl_display, fl_screen))
    mask = SubstructureRedirectMask;	   
  XSendEvent(fl_display, w, False, mask, &ev);
}

void sendUpdateInfo() 
{
    unsigned int i, nrootwins;
    Window dw1, dw2, *rootwins = 0;
    int screen_count = ScreenCount(fl_display);
    extern Atom FLTKChangeSettings;
    for (int s = 0; s < screen_count; s++) {
        Window root = RootWindow(fl_display, s);
        XQueryTree(fl_display, root, &dw1, &dw2, &rootwins, &nrootwins);
        for (i = 0; i < nrootwins; i++) {
            if (rootwins[i]!=RootWindow(fl_display, fl_screen)) {
                sendClientMessage(rootwins[i], FLTKChangeSettings, 0);
            }
        }
    }
    XFlush(fl_display);
}

void
 readIconsConfiguration()
{
    Fl_Config globalConfig(fl_find_config_file("ede.conf", 0), true, false);
    globalConfig.set_section("IconManager");

    globalConfig.read("Label Background", label_background, 46848);
    globalConfig.read("Label Transparent", label_trans, false);
    globalConfig.read("Label Foreground", label_foreground, FL_WHITE);
    globalConfig.read("Label Fontsize", label_fontsize, 12);
    globalConfig.read("Label Maxwidth", label_maxwidth, 75);
    globalConfig.read("Gridspacing", label_gridspacing, 16);
    globalConfig.read("OneClickExec", label_engage_1click, false);
    globalConfig.read("AutoArrange", auto_arr, false);
}

void writeIconsConfiguration()
{
    Fl_Config globalConfig(fl_find_config_file("ede.conf", 1));
    globalConfig.set_section("IconManager");

    globalConfig.write("Label Background", label_background);
    globalConfig.write("Label Transparent", label_trans);
    globalConfig.write("Label Foreground", label_foreground);
    globalConfig.write("Label Fontsize", label_fontsize);
    globalConfig.write("Label Maxwidth", label_maxwidth);
    globalConfig.write("Gridspacing", label_gridspacing);
    globalConfig.write("OneClickExec", label_engage_1click);
    globalConfig.write("AutoArrange", auto_arr);
}


