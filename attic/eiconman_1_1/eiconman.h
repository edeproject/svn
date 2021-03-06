/*
 * $Id: eiconman.h 1653 2006-06-09 13:08:58Z karijes $
 *
 * Eiconman, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef __EICONMAN_H__
#define __EICONMAN_H__

#include "wallpaper.h"

#include <efltk/Fl_Double_Window.h>
#include <efltk/Fl_Menu_Button.h>
#include <efltk/Fl_String.h>
#include <efltk/Fl_Config.h>

#ifdef LOCAL_VECTOR
	#include "Vector.h"
#else
	#include <vector>
	using std::vector;
#endif

struct GlobalIconSettings
{
	Fl_Color label_background;
	Fl_Color label_foreground;
	int label_fontsize;
	int label_maxwidth;
	int label_gridspacing;
	bool label_transparent;
	bool label_draw;
	bool one_click_exec;
	bool auto_arr;
	bool same_size;
};

// settings related to .desktop files
struct IconSettings
{
	Fl_String name;
	Fl_String cmd;
	Fl_String icon_path;
	int x, y;
};

struct IconSelector
{
	int x, y, w, h;
	bool visible;
};


class Icon;

class Desktop : public Fl_Double_Window
{
	private:
		IconSelector isel;

		// TODO: delay loading
		Wallpaper wallp;

		Fl_Menu_Button *popup;
		IconSettings isett;
		GlobalIconSettings gisett;
		Fl_Color bg_color;
		int bg_opacity;
		int bg_mode;
		bool bg_use;

		bool moving;

		int selection_x;
		int selection_y;

		vector<Icon*> selectionbuff;
		vector<Icon*> icons;

		void read_icon_file(const char* path, IconSettings& isett);
		void read_icons_conf(Fl_Config& conf);
		void read_bg_conf(Fl_Config& conf);
		void sort_internals(void);

		void move_selection(int x, int y, bool apply);
		void select(Icon* i);
		void select_only(Icon* i);

		void set_xbackground(int dx, int dy, int dw, int dh);
		//Icon* icon_clicked(void);

	public:
		Desktop();
		~Desktop();
		void draw(void);
		int handle(int event);
		void hide(void) { }
		void read_config(void);
		void update_workarea(void);
		void auto_arrange(void);
		void update_bg(void);
		void load_icons(void);
		void add_icon(Icon* ic);
		void unfocus_all(void);
		bool in_selection(const Icon* ic);
};

#endif
