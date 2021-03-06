// generated by Extended Fast Light User Interface Designer (eFluid) version 2.0003

#include "aboutdialog.h"
#include <efltk/Fl_Locale.h>
#include <edeconf.h>
static Fl_Window* mAboutDialogWindow;

static void cb_OK(Fl_Button*, void*) {
  mAboutDialogWindow->hide();
}

void AboutDialog(Fl_Widget*, void*) {
  Fl_Window* w;
   {Fl_Window* o = mAboutDialogWindow = new Fl_Window(355, 305, _("About Equinox Desktop Environment"));
    w = o;
    o->shortcut(0xff1b);
     {Fl_Box* o = new Fl_Box(5, 10, 345, 35, _("Equinox Desktop Environment "PACKAGE_VERSION));
      o->box(FL_BORDER_FRAME);
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
     {Fl_Box* o = new Fl_Box(5, 70, 345, 15, _("(C)Copyright 2000-2004 EDE Authors"));
      o->label_size(10);
      o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
    }
//    o->set_modal();
    o->end();
    o->resizable(o);
  }
  mAboutDialogWindow->end();
  mAboutDialogWindow->show();
}
