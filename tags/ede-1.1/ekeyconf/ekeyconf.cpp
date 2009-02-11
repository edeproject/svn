/*
 * $Id$
 *
 * EControl applet for keyboard shortcuts
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include "ekeyconf.h"
#include <efltk/Fl_Locale.h>
#include <edeconf.h>
#include "ekeys.h"
Fl_Window* iconsConfWindow;

static void cb_OK(Fl_Button*, void*) {
	writeKeysConfiguration();
	sendUpdateInfo();
	exit(0);
}

static void cb_Cancel(Fl_Button*, void*) {
	exit(0);
}

static void cb_Apply(Fl_Button*, void*) {
	writeKeysConfiguration();
	sendUpdateInfo();
}
Shortcut_Button* shortcut;

static void cb_shortcut(Shortcut_Button*, void*) {
	setshortcutfor(action->value(),shortcut->svalue);
}
Fl_Input_Browser* action;

static void cb_action(Fl_Input_Browser*, void*) {
	shortcut->svalue = getshortcutfor(action->value());
	shortcut->redraw();
}

static void cb_New(Fl_Button*, void*) {
	newShortcutWindow->show();
}

static void cb_Remove(Fl_Button*, void*) {
	removeShortcut(action->value());
	populatelist(action);
	action->value(action->child(0)->label());
	action->do_callback();
}
Fl_Window* newShortcutWindow;
Fl_Input* shortcutName;
Fl_Input* shortcutCommand;

static void cb_Ok(Fl_Button*, void*) {
	addShortcut(shortcutName->value(),shortcutCommand->value());
	populatelist(action);
	action->value(shortcutName->value());
	action->do_callback();
	newShortcutWindow->hide();
}

static void cb_Cancel1(Fl_Button*, void*) {
	newShortcutWindow->hide();
}

int main (int argc, char **argv) {

	Fl_Window* w;
	fl_init_locale_support("ekeyconf", PREFIX"/share/locale");
	readKeysConfiguration();
	 {Fl_Window* o = iconsConfWindow = new Fl_Window(275, 269, _("Keyboard settings"));
		w = o;
		o->shortcut(0xff1b);
		 {Fl_Button* o = new Fl_Button(20, 240, 80, 25, _("&OK"));
			o->callback((Fl_Callback*)cb_OK);
		}
		 {Fl_Button* o = new Fl_Button(190, 240, 80, 24, _("&Cancel"));
			o->callback((Fl_Callback*)cb_Cancel);
		}
		 {Fl_Button* o = new Fl_Button(105, 240, 80, 24, _("&Apply"));
			o->callback((Fl_Callback*)cb_Apply);
		}
		 {Fl_Tabs* o = new Fl_Tabs(3, 5, 265, 229);
			o->color((Fl_Color)0xfffffffe);
			 {Fl_Group* o = new Fl_Group(1, 28, 263, 200, _("Shortcuts"));
				o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
				 {Shortcut_Button* o = shortcut = new Shortcut_Button(21, 86, 140, 20, _("Keyboard shortcut:"));
					o->box(FL_DOWN_BOX);
					o->color((Fl_Color)7);
					o->callback((Fl_Callback*)cb_shortcut);
					o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
					o->tooltip(_("To set a shortcut:\n   Click here, then type the key combination you want.\nT\
o delete the shortcut:\n   Click here, then type backspace.\nTo get out of sho\
rtcut-setting mode:\n   Click the mouse on this again, or on some other field."));
				}
				 {Fl_Input_Browser* o = action = new Fl_Input_Browser(21, 35, 165, 21, _("Action:")); o->begin();
//					o->button_box(FL_(null));
					o->callback((Fl_Callback*)cb_action);
					o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
					action->value("Next window");
					o->end();
				}
				 {Fl_Button* o = new Fl_Button(21, 132, 110, 25, _("&New shortcut..."));
					o->callback((Fl_Callback*)cb_New);
				}
				 {Fl_Button* o = new Fl_Button(141, 132, 110, 25, _("&Remove shortcut"));
					o->callback((Fl_Callback*)cb_Remove);
				}
				o->end();
			}
			o->end();
		}
		o->end();
		o->resizable(o);
	}
	 {Fl_Window* o = newShortcutWindow = new Fl_Window(285, 114, _("Define new shortcut"));
		o->shortcut(0xff1b);
		shortcutName = new Fl_Input(104, 15, 170, 24, _("&Shortcut name:"));
		shortcutCommand = new Fl_Input(104, 49, 170, 24, _("C&ommand"));
		 {Fl_Button* o = new Fl_Button(49, 83, 88, 24, _("&Ok"));
			o->callback((Fl_Callback*)cb_Ok);
		}
		 {Fl_Button* o = new Fl_Button(158, 83, 88, 24, _("&Cancel"));
			o->callback((Fl_Callback*)cb_Cancel1);
		}
		o->set_modal();
		o->end();
		o->resizable(o);
	}
	populatelist(action);
	shortcut->svalue = getshortcutfor("Next window");
	shortcut->redraw();
	w->show(argc, argv);
	
	// Grab all keyboard events from window manager
	XGrabKey(fl_display, AnyKey, AnyModifier, fl_xid(w), true, GrabModeAsync, GrabModeAsync);
	return  Fl::run();
}
