/*
 * $Id$
 *
 * Edewm, window manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef _WINDOWMANAGER_H_
#define _WINDOWMANAGER_H_

#include <efltk/Fl.h>
#include <efltk/Fl_Window.h>
#include <efltk/x.h>
#include <efltk/fl_draw.h>
#include <efltk/filename.h>

#include <efltk/Fl_Ptr_List.h>
#include <efltk/Fl_Config.h>
#include <efltk/Fl_Locale.h>
#include "../exset/exset.h"

class Frame;

class Frame_List : public Fl_Ptr_List {
public:
    Frame_List() : Fl_Ptr_List() { }

    void append(Frame *item) { Fl_Ptr_List::append((void *)item); }
    void prepend(Frame *item) { Fl_Ptr_List::prepend((void *)item); }
    void insert(uint pos, Frame *item) { Fl_Ptr_List::insert(pos, (void *)item); }
    void replace(uint pos, Frame *item) { Fl_Ptr_List::replace(pos, (void *)item); }
    void remove(uint pos) { Fl_Ptr_List::remove(pos); }
    bool remove(Frame *item) { return Fl_Ptr_List::remove((void *)item); }
    int index_of(const Frame *w) const { return Fl_Ptr_List::index_of((void*)w); }
    Frame *item(uint index) const { return (Frame*)Fl_Ptr_List::item(index); }

    Frame **data() { return (Frame**)items; }

    Frame *operator [](uint ind) const { return (Frame *)items[ind]; }
};

////////////////////////////////////////////////////////////////
// The WindowManager class looks like a window to efltk but is actually the
// screen's root window.  This is done by setting xid to "show" it
// rather than have efltk create the window. Class handles all root
// windows X events

class WindowManager : public Fl_Window
{
    Fl_Rect wm_area;
    Fl_Cursor cursor;
    Exset *xset;
	Window root_win;
	static WindowManager* pinstance;
	bool is_init;

	WindowManager();
	~WindowManager();
	WindowManager(const WindowManager&);
	WindowManager& operator=(WindowManager&);
    void init_internals(int argc, char *argv[]);


public:
	static void init(int argc, char* argv[]);
	static WindowManager* instance();
	static void shutdown();

	bool initialized() { return is_init; }
    void set_default_cursor();
    void set_cursor(Fl_Cursor c, Fl_Color fg, Fl_Color bg);
    void read_dispconf();
    Fl_Cursor get_cursor() { return cursor; }

	Window root_window() { return root_win; }

    void idle();
    int handle(int e);
    int handle(XEvent *e);

    void show();
    void hide() { } //Prevent efltk to hide root window
    void draw();

    void update_workarea(bool send=true);
	void read_configuration();

    Frame *find_by_wid(Window wid);
    void restack_windows();

    //Updates GNOME and NET client list atoms
    static void update_client_list();
    static bool handle_desktop_msgs(const XClientMessageEvent *e);

    int x() { return wm_area.x(); }
    int y() { return wm_area.y(); }
    int w() { return wm_area.w(); }
    int h() { return wm_area.h(); }

    int XShapeEventBase, XShapeErrorBase;
};

extern Frame_List stack_order;
extern Frame_List map_order;

extern Frame_List remove_list;
#endif
