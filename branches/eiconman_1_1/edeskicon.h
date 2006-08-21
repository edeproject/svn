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

#ifndef __EDESKICON_H__
#define __EDESKICON_H__

#include <efltk/Fl_Widget.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Menu_Button.h>
#include <efltk/Fl_Image.h>
#include <efltk/Fl_String.h>

struct IconSettings;
struct GlobalIconSettings;

class Icon : public Fl_Widget
{
	private:
		Fl_Menu_Button* popup;
		Fl_Image* icon_img;
		const IconSettings* settings;
		const GlobalIconSettings* globals;
		int lwidth;
		int lheight;

		void update_label_size(const char* ll, int maxwidth);

	public:
		Icon(const GlobalIconSettings* gs, const IconSettings* s);
		~Icon();
		Fl_Image* icon_image(void) { return icon_img; }
		void draw(void);
		void layout(void);
		int handle(int event);
		void delete_icon(void);
		void update_all(void);
};

class MovableIcon : public Fl_Window
{
	private:
		Icon* icon;
	public:
		MovableIcon(Icon* i);
		~MovableIcon();
};

#endif
