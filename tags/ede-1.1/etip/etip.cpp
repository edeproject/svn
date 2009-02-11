/*
 * $Id$
 *
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include <efltk/Fl.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Check_Button.h>
#include <efltk/Fl_Image.h>
#include <efltk/Fl_Config.h>
#include <efltk/Fl_Locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <efltk/Fl_Images.h>
#include "icons/hint.xpm"

#include <edeconf.h>

unsigned int curr_tip = 0;
Fl_Check_Button* show_check = NULL;
Fl_Config* conf_global = NULL;

#define TIPS_NUM 7
#define TITLE_TIPS_NUM 9

const char* tiplist[TIPS_NUM] = 
{
_("To start any application is simple. Press on the button with your user name, go\
 to the Programs menu, select category and click on the wished program."),

_("To exit the EDE, press button with your user name and then Logout."),

_("To lock the computer, press button with your user name and then choose Lock."),

_("To setup things on the computer, press button with your user name, Panel menu and then the Control panel."),

_("To add a program that is not in the Programs menu, click on the button with your user,\
 Panel menu, and then Menu editor."),

_("Notice that this is still development version, so please send your bug reports or\
 comments on EDE forum, EDE bug reporting system (on project's page), or check mails of current\
 maintainers located in AUTHORS file."),

_("You can download latest release on http://sourceforge.net/projects/ede.")
};

const char* title_tips[TITLE_TIPS_NUM] =
{
_("Boring \"Did you know...\""),
_("How about this..."),
_("Smart idea..."),
_("Really smart idea..."),
_("Really really smart idea..."),
_("Uf..."),
_("Something new..."),
_("Or maybe this..."),
_("...")
};

const char* random_txt(const char** lst, unsigned int max)
{
	unsigned int val = rand() % max;
	curr_tip = val;
	return lst[val];
}

void close_cb(Fl_Widget*, void* w)
{
	Fl_Window* win = (Fl_Window*)w;
	conf_global->set_section("Tips");
	conf_global->write("Show", !show_check->value()); 
	conf_global->flush(); 

	win->hide();
}

void next_cb(Fl_Widget*, void* tb)
{
	Fl_Box* tipbox = (Fl_Box*)tb;
	curr_tip++;
	if(curr_tip >= TIPS_NUM)
		curr_tip = 0;
	tipbox->label(tiplist[curr_tip]);	
	tipbox->redraw_label();
}

void prev_cb(Fl_Widget*, void* tb)
{
	Fl_Box* tipbox = (Fl_Box*)tb;
	if(curr_tip == 0)
		curr_tip = TIPS_NUM - 1;
	else
		curr_tip--;
	tipbox->label(tiplist[curr_tip]);	
	tipbox->redraw_label();
}

int main(int argc, char** argv)
{
	// Check config option, if showing is disabled, exit
	Fl_Config conf("EDE Team", "etip");
	if(argc == 2 && (!strcmp(argv[1], "-f") || !strcmp(argv[1], "--force")))
	{
		// nothing, to simplify omiting those '!'
	}
	else
	{
		bool show = true;
		conf.set_section("Tips");
		conf.read("Show", show, true);
		if (!show)
			return 0;
	}
	
	conf_global = &conf;
	srand(time(NULL));
	fl_init_locale_support("etip", PREFIX"/share/locale");

	Fl_Window* win = new Fl_Window(420, 169, _("Tips..."));
	win->shortcut(0xff1b);
	win->begin();

	Fl_Group* gr = new Fl_Group(5, 5, 410, 130);
	gr->box(FL_DOWN_BOX);
	Fl_Box* img = new Fl_Box(5, 5, 70, 65);
	Fl_Image pix(hint_xpm);
	img->image(pix);

	Fl_Box* title = new Fl_Box(80, 10, 320, 25, random_txt(title_tips, TITLE_TIPS_NUM));
	title->label_font(fl_fonts+1);
	title->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	title->box(FL_FLAT_BOX);

	Fl_Box* tiptxt = new Fl_Box(80, 45, 320, 75, random_txt(tiplist, TIPS_NUM));
	tiptxt->align(FL_ALIGN_LEFT|FL_ALIGN_TOP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
	tiptxt->box(FL_FLAT_BOX);
	gr->end();

	Fl_Check_Button* ch = new Fl_Check_Button(5, 140, 150, 25, _(" Do not bother me"));
	show_check = ch;

	Fl_Button* prev = new Fl_Button(160, 140, 80, 25, "<-");
	prev->label_font(fl_fonts+1);
	prev->callback(prev_cb, tiptxt);

	Fl_Button* next = new Fl_Button(245, 140, 80, 25, "->");
	next->label_font(fl_fonts+1);
	next->callback(next_cb, tiptxt);
	
	Fl_Button* close = new Fl_Button(335, 140, 80, 25, _("&Close"));
	close->callback(close_cb, win);

	win->end();

	win->set_modal();
	win->show();
	return Fl::run();
}
