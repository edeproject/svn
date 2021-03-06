/*
 * $Id$
 *
 * Eabout, display version and authors
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2008-2014 EDE Authors.
 *
 * This program is licensed under terms of the 
 * GNU General Public License version 2 or newer.
 * See COPYING for details.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h> // memset

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Pixmap.H>

#include <edelib/Ede.h>
#include <edelib/WindowUtils.h>
#include "icons/ede.xpm"

EDELIB_NS_USING(window_center_on_screen)

static Fl_Pixmap image_ede((const char**)ede_xpm);

Fl_Window* win;
Fl_Text_Buffer* stylebuff;

Fl_Text_Display::Style_Table_Entry style_table[] = {
	{ FL_BLACK, FL_HELVETICA, 12, }, // A - plain
	{ FL_BLACK, FL_HELVETICA_BOLD, 12} // B - bold text
};

const char* content = "\n\
 Developers:\n\n\
 Sanel Zukan (karijes@users.sf.net)\n\
 Vedran Ljubovic (vljubovic@users.sf.net)\n\
\n\
 Contributors:\n\n\
 (to be added)\n\
\n\
 Past developers:\n\n\
 Martin Pekar\n\
 Mikko Lahtenaaki\n\
 Dejan Lekic\n\
 Alexey Parshin\n\
\n\
 Translators:\n\n\
 (to be added)\n\
\n\
 Patches in 1.x series by:\n\n\
 Michael Sheldon\n\
 Anthony Wesley\n\
 and others! Thank you guys! :)\n\
\n\
 Web:\n\n\
 http://equinox-project.org\n\
\n\
 Bug reports:\n\n\
 http://bugs.equinox-project.org\n\
\n\
 License: \n\n\
 This program is based in part on the work of\n\
 FLTK project (www.fltk.org).\n\
\n\
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
 Cambridge, MA 02139, USA";

const char* bold_keywords[] = {
	"Developers:",
	"Contributors:",
	"Past developers:",
	"Patches in 1.x series by:",
	"Translators:",
	"Web:",
	"Bug reports:",
	"License:"
};

#define BOLD_KEYWORDS_LEN 8

void close_cb(Fl_Widget*, void*) {
	win->hide();
}

char* prepare_style(char* txt, int len) {
	// paint all with 'A' style at startup
	char* style = new char[len + 1];
	memset(style, 'A', len - 1);
	style[len] = '\0';

	// find bold keywords and paint them
	char* p = 0;
	for(int i = 0; i < BOLD_KEYWORDS_LEN; i++) {
		p = strstr(txt, bold_keywords[i]);
		if(!p)
			continue;
		unsigned int len = strlen(bold_keywords[i]);
		memset(&style[p - txt], 'B', len);
	}

	return style;
}

int main(int argc, char **argv) {
	EDE_APPLICATION("ede-about");

	win = new Fl_Window(440, 335, _("About EDE"));
	win->begin();
		Fl_Group* title_group = new Fl_Group(0, 0, 440, 65);
		title_group->box(FL_ENGRAVED_BOX);
		title_group->color(FL_BACKGROUND2_COLOR);
		title_group->begin();
			Fl_Box* image_box = new Fl_Box(5, 5, 60, 55);
			image_box->image(image_ede);

			Fl_Box* ede_label_box = new Fl_Box(70, 10, 300, 25, "Equinox Deskop Environment " PACKAGE_VERSION);
			ede_label_box->labelfont(1);
			ede_label_box->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

			Fl_Box* ede_copy_box = new Fl_Box(70, 35, 300, 25, "Copyright (c) by EDE Authors 2000-2014");
			ede_copy_box->labelsize(11);
			ede_copy_box->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_TOP);

			// resizable box
			Fl_Box* rbox1 = new Fl_Box(385, 10, 40, 50);
        	Fl_Group::current()->resizable(rbox1);
		title_group->end();

		Fl_Text_Display* txt_display = new Fl_Text_Display(10, 78, 420, 212);
		txt_display->textsize(12);

		// remove bottom scrollbar, since text is already nicely formatted
		txt_display->scrollbar_align(FL_ALIGN_RIGHT);

		Fl_Text_Buffer* buff = new Fl_Text_Buffer();
		buff->append(content);

		// load style buff
		stylebuff = new Fl_Text_Buffer(buff->length());
		char* style = prepare_style(buff->text(), buff->length());

		stylebuff->text(style);
		delete [] style;

		txt_display->buffer(buff);
		txt_display->highlight_data(stylebuff, style_table, 
				sizeof(style_table) / sizeof(style_table[0]), 'A', 0, 0);

		// resizable box
		Fl_Box* rbox2 = new Fl_Box(270, 238, 50, 37);

		Fl_Button* close_button = new Fl_Button(340, 300, 90, 25, _("&Close"));
		close_button->callback(close_cb);

		Fl_Group::current()->resizable(rbox2);
	win->end();
	window_center_on_screen(win);
	win->show(argc, argv);
	return Fl::run();
}
