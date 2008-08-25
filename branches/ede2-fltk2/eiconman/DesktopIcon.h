/*
 * $Id$
 *
 * Eiconman, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2007 EDE Authors.
 *
 * This program is licensed under terms of the 
 * GNU General Public License version 2 or newer.
 * See COPYING for details.
 */

#ifndef __DESKTOPICON_H__
#define __DESKTOPICON_H__

#include <FL/Fl_Widget.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Box.h>
#include <FL/Fl_Button.h>
#include <FL/Fl_Image.h>

#include <X11/Xlib.h> // Pixmap

class GlobalIconSettings;
class IconSettings;
class MovableIcon;

class Fl_Menu_Button;

class DesktopIcon : public Fl_Widget {
	private:
		IconSettings* settings;
		const GlobalIconSettings* globals;

		int  lwidth;
		int  lheight;
		bool focus;

		MovableIcon* micon;

		Fl_Menu_Button* imenu;

		void update_label_size(void);
		void fix_position(int X, int Y);

	public:
		DesktopIcon(GlobalIconSettings* gisett, IconSettings* isett, int bg);
		~DesktopIcon();

		virtual void draw(void);
		virtual int  handle(int event);

		void drag(int x, int y, bool apply);
		int  drag_icon_x(void);
		int  drag_icon_y(void);

		/*
		 * This is 'enhanced' (in some sense) redraw(). Redrawing
		 * icon will not fully redraw label nor focus box, which laid outside 
		 * icon box. It will use damage() on given region, but called from
		 * parent, so parent can redraw that region on itself (since label does
		 * not laid on any box)
		 *
		 * Alternative way would be to redraw whole parent, but it is pretty unneeded
		 * and slow.
		 */
		void fast_redraw(void);

		/*
		 * Here is implemented localy focus schema avoiding
		 * messy fltk one. Focus/unfocus is handled from Desktop.
		 */
		void do_focus(void)   { focus = true;  }
		void do_unfocus(void) { focus = false; }
		bool is_focused(void) { return focus;  }

		Fl_Image* icon_image(void) { return image(); }

		void rename(const char* str);
};

class MovableIcon : public Fl_Window {
	private:
		DesktopIcon* icon;
		Fl_Box* icon_box;
		Pixmap mask;

	public:
		MovableIcon(DesktopIcon* i);
		~MovableIcon();
		virtual void show(void);
};

#endif