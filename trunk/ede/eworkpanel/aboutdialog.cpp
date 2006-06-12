/*
 * $Id$
 *
 * About dialog
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include <efltk/Fl.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Text_Display.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Image.h>
#include <efltk/Fl_Text_Buffer.h>
#include <edeconf.h>

#include "aboutdialog.h"
#include "icons/ede.xpm"


Fl_Image ede_logo_pix((const char**)ede_xpm);

const char* licence =
_("This program is based in part on the work of\n\
FLTK project (www.fltk.org).\n\n\
This program is free software, you can redistribute\n\
it and/or modify it under the terms of GNU General\n\
Public License as published by the Free Software\n\
Foundation, either version 2 of the License, or\n\
(at your option) any later version.\n\n\
This program is distributed in the hope that it will\n\
be useful, but WITHOUT ANY WARRANTY;\n\
without even the implied\n\
warranty of MERCHANTABILITY or FITNESS\n\
FOR A PARTICULAR PURPOSE.\n\n\
See the GNU General Public License for more details.\n\
You should have received a copy of the GNU General\n\
Public Licence along with this program; if not, write\n\
to the Free Software Foundation, Inc., 675 Mass Ave,\n\
Cambridge, MA 02139, USA");

void close_cb(Fl_Widget*, void* w)
{
	Fl_Window* win = (Fl_Window*)w;
	win->hide();
}

void DetailsDialog(void)
{
	Fl_Window* win = new Fl_Window(395, 294, _("Details"));
	win->shortcut(0xff1b);
	win->begin();
	Fl_Button* close = new Fl_Button(310, 265, 80, 25, _("&Close"));
	close->callback(close_cb, win);
	Fl_Text_Display* txt = new Fl_Text_Display(5, 5, 385, 255);
	txt->box(FL_DOWN_BOX);
	Fl_Text_Buffer* buff = new Fl_Text_Buffer();
	txt->buffer(buff);
	txt->insert(licence);
	win->end();
	win->set_modal();
	win->show();
}

void details_cb(Fl_Widget*, void*)
{
	DetailsDialog();
}

void AboutDialog(Fl_Widget*, void*)
{
	Fl_Window* win = new Fl_Window(370, 214, _("About Equinox Desktop Environment"));
    win->shortcut(0xff1b);
	win->begin();
	Fl_Box* imgbox = new Fl_Box(10, 15, 55, 55);
	imgbox->image(ede_logo_pix);

	Fl_Box* title = new Fl_Box(75, 15, 285, 35, "Equinox Desktop Environment");
	title->label_font(fl_fonts+1);
	title->label_size(14);
	title->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	Fl_Box* vers = new Fl_Box(75, 50, 285, 20, _("version "PACKAGE_VERSION));
	vers->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	Fl_Box* copyright = new Fl_Box(75, 85, 285, 20, "Copyright (c) EDE Authors 2000-2006");
	copyright->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	Fl_Box* lic = new Fl_Box(75, 110, 285, 50, _("This program is licenced under terms of \
the GNU General Public Licence version 2 or newer.\nSee Details for more."));
	lic->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);

	Fl_Button* details = new Fl_Button(200, 185, 80, 25, _("&Details"));
	details->callback(details_cb);

	Fl_Button* close = new Fl_Button(285, 185, 80, 25, _("&Close"));
	close->callback(close_cb, win);


	win->end();
	win->show();
	Fl::run();
}

/*
static Fl_Window* mAboutDialogWindow;

static void cb_OK(Fl_Button*, void*) {
  mAboutDialogWindow->hide();
}

void AboutDialog(Fl_Widget*, void*) {
  Fl_Window* w;
   {Fl_Window* o = mAboutDialogWindow = new Fl_Window(355, 305, _("About Equinox Desktop Environment"));
    w = o;
    o->shortcut(0xff1b);
     {Fl_Box* o = new Fl_Box(5, 25, 345, 35, _("Equinox Desktop Environment "PACKAGE_VERSION));
      o->label_font(fl_fonts+1);
      o->color((Fl_Color)56);
      o->label_color((Fl_Color)32);
      o->label_size(16);
      o->align(FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
    }
     {Fl_Box* o = new Fl_Box(5, 105, 345, 135, _(" This program is based in part on the work of FLTK project (www.fltk.org). Th\
is program is free software, you can redistribute it and/or modify it under th\
e terms of GNU General Public License as published by the Free Software Founda\
tion, either version 2 of the License, or (at your option) any later version. \
This program is distributed in the hope that it will be useful, but WITHOUT AN\
Y WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FO\
R A PARTICULAR PURPOSE. See the GNU General Public License for more details. Y\
ou should have received a copy of the GNU General Public Licence along with th\
is program; if not, write to the Free Software Foundation, Inc., 675 Mass Ave,\
 Cambridge, MA 02139, USA"));
      o->label_size(10);
      o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
    }
     {Fl_Button* o = new Fl_Button(270, 275, 80, 25, _("&OK"));
      o->callback((Fl_Callback*)cb_OK);
      o->align(FL_ALIGN_WRAP);
    }
    new Fl_Divider(5, 260, 345, 15, _("label"));
     {Fl_Box* o = new Fl_Box(5, 60, 345, 15, _("Copyright (c) 2000-2005 EDE Authors"));
      o->label_size(10);
      o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
    }
    o->set_modal();
    o->end();
    o->resizable(o);
  }
  mAboutDialogWindow->end();
  mAboutDialogWindow->show();
}
*/
