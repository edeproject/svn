// generated by Extended Fast Light User Interface Designer (eFluid) version 2,0003

#include "aboutdialog.h"
// The EDE control center.
// Copyright (c) 2000. - 2005. EDE Authors
// This program is licenced under terms of the
// GNU General Public Licence version 2 or newer.
// See COPYING for details
//
#include <efltk/Fl_Util.h>
#include <edeconf.h>
static Fl_Window *aboutWindow;

inline void AboutDialog::cb_Click_i(Fl_Button*, void*) {
	showCopyingInfo();
}
void AboutDialog::cb_Click(Fl_Button* o, void* v) {
	((AboutDialog*)(o->parent()->user_data()))->cb_Click_i(o,v);
}

inline void AboutDialog::cb_Close_i(Fl_Button*, void*) {
	aboutWindow->hide();
}
void AboutDialog::cb_Close(Fl_Button* o, void* v) {
	((AboutDialog*)(o->parent()->user_data()))->cb_Close_i(o,v);
}

AboutDialog::AboutDialog(const char *progname, const char *progversion, const char *addcomment ) {
	Fl_Window* w;
	 {Fl_Window* o = aboutWindow = new Fl_Window(275, 190);
		w = o;
		o->shortcut(0xff1b);
		o->user_data((void*)(this));
		 {Fl_Box* o = new Fl_Box(5, 5, 265, 44);
			o->label_size(18);
			o->align(FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
			o->label(Fl_String(progname)+Fl_String(" ")+Fl_String(progversion));
		}
		 {Fl_Box* o = new Fl_Box(5, 49, 265, 20, _("Part of Equinox Desktop Environment"));
			o->align(FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
			o->label(o->label() + Fl_String(" "PACKAGE_VERSION));
		}
		 {Fl_Box* o = new Fl_Box(5, 74, 265, 20, _("Copyright (c) 2000-2005 EDE Authors"));
			o->align(FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
		}
		 {Fl_Box* o = new Fl_Box(5, 98, 265, 30, _("This program is licenced under terms of the GNU General Public License versio\
n 2 or newer."));
			o->label_size(10);
			o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
		}
		 {Fl_Button* o = new Fl_Button(65, 124, 145, 20, _("Click here for details."));
			o->box(FL_NO_BOX);
			o->button_box(FL_NO_BOX);
			o->label_color((Fl_Color)4);
			o->highlight_label_color((Fl_Color)1);
			o->label_size(10);
			o->callback((Fl_Callback*)cb_Click);
			((Fl_Window*)(o->parent()))->hotspot(o);
		}
		 {Fl_Button* o = new Fl_Button(95, 152, 80, 25, _("&Close"));
			o->callback((Fl_Callback*)cb_Close);
		}
		aboutWindow->label(Fl_String(_("About"))+Fl_String(" ")+Fl_String(progname));
		o->set_modal();
		o->end();
		o->resizable(o);
	}
	aboutWindow->end();
	aboutWindow->show();
}

void AboutDialog::showCopyingInfo() {
	fl_start_child_process("elauncher file:/usr/share/ede/doc/copying.html",false);
}