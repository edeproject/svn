// Copyright (c) 2000. - 2005. EDE Authors
// This program is licenced under terms of the
// GNU General Public Licence version 2 or newer.
// See COPYING for details.

#include "aboutdialog.h"
#include <efltk/Fl_Util.h>
#include <edeconf.h>

void showCopyingInfo() {
	fl_start_child_process("elauncher file:/usr/share/ede/doc/copyleft.html",false);
}

static void cb_Click(Fl_Button*, void*) {
	showCopyingInfo();
}

static void cb_Close(Fl_Button*, Fl_Window* window) 
{
	window->hide();
}

AboutDialog::AboutDialog(const char *progname, const char *progversion, const char *addcomment)
{
  Fl_Window* w;
  {
  	Fl_Window* o = aboutWindow = new Fl_Window(275, 190, Fl_String(_("About "))+Fl_String(progname));
	w = o;
	o->shortcut(0xff1b);
	{
		Fl_Box* o = new Fl_Box(5, 5, 265, 44, Fl_String(progname)+Fl_String(" ")+Fl_String(progversion));
		o->label_size(18);
		o->align(FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
	}
	{
		Fl_Box* o = new Fl_Box(5, 49, 265, 20, _("Part of Equinox Desktop Environment "PACKAGE_VERSION));
		o->align(FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
	}
	{
		Fl_Box* o = new Fl_Box(5, 74, 265, 20, _("(C) Copyright 2000-2005 EDE Authors"));
		o->align(FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
	}
	{
		Fl_Box* o = new Fl_Box(5, 98, 265, 30, _("This program is licenced under terms of the GNU General Public License version 2 or newer."));
		o->label_size(10);
		o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
	}
	{
		Fl_Button* o = new Fl_Button(65, 120, 145, 20, _("Click here for details."));
		o->box(FL_NO_BOX);
		o->button_box(FL_NO_BOX);
		o->label_color((Fl_Color)4);
		o->highlight_label_color((Fl_Color)1);
		o->label_size(10);
		o->callback((Fl_Callback*)cb_Click);
	}
	{
		Fl_Button* o = new Fl_Button(95, 152, 80, 25, _("&Close"));
		o->callback((Fl_Callback*)cb_Close,aboutWindow);
	}
	o->end();
  }
  aboutWindow->end();
  aboutWindow->set_modal();
  aboutWindow->show();
}


