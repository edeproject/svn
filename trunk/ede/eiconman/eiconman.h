/*
 * $Id$
 *
 * Eiconman, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef EICONMAN_H
#define EICONMAN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stddef.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>

#include <efltk/Fl.h>
#include <efltk/Fl_Double_Window.h>
#include <efltk/x.h>
#include <efltk/Fl_Menu_Button.h>
#include <efltk/Fl_Item_Group.h>
#include <efltk/Fl_Item.h>
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
#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Scroll.h>
#include <efltk/Fl_Font.h>
#include <efltk/Fl_Images.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Util.h>

class WPaper;

class Desktop : public Fl_Double_Window
{
public:
    Desktop();
    ~Desktop();

    virtual int handle(int);
    virtual void hide() {}

    virtual void draw();

    void update_bg();
    void update_icons();
    void update_workarea();

    void auto_arrange();

    WPaper *wpaper; //Generated image
    Fl_Color bg_color;
    bool bg_use;
    int bg_opacity, bg_mode;
    Fl_String bg_filename;

    Fl_Menu_Button *popup;
};

class WPaper : public Fl_Image
{
public:
    WPaper(int W, int H, Fl_PixelFormat *fmt);
    ~WPaper();

    void _draw(int dx, int dy, int dw, int dh,
              int sx, int sy, int sw, int sh,
              Fl_Flags f);

};

extern Desktop *desktop;

extern int label_background;
extern int label_foreground;
extern int label_fontsize;
extern bool label_trans;
extern int label_maxwidth;
extern int label_gridspacing;
extern bool one_click_exec;

#endif
