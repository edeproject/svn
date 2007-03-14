/*
 * $Id$
 *
 * Eiconman, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2007 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef __ICONDIALOG_H__
#define __ICONDIALOG_H__

#include <efltk/Fl_Window.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Check_Button.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_String.h>
#include <efltk/Fl_Input.h>

class IconSettings;

class IconDialog : public Fl_Window
{
	private:
		IconSettings* settings;

		Fl_Button* cancel;
		Fl_Button* ok;

		Fl_Button* img_path_button;
		Fl_Button* cmd_path_button;
		Fl_Button* link_path_button;
		Fl_Button* term_path_button;

		Fl_Box* img_box;

		Fl_Input* name;
		Fl_Input* img_path;
		Fl_Input* cmd_path;
		Fl_Input* link_path;
		Fl_Input* term_path;

	public:
		IconDialog(IconSettings* is, Fl_Image* img);
		~IconDialog();
		void apply_cb_internal(void);
};
#endif
