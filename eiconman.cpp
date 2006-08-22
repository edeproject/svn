/*
 * $Id: eiconman.h 1653 2006-06-09 13:08:58Z karijes $
 *
 * Eiconman, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include "eiconman.h"
#include "edeskicon.h"

#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Images.h>
#include <efltk/Fl_Item.h>
#include <efltk/Fl_Divider.h>
#include <efltk/Fl_Directory_DS.h>
#include <efltk/Fl_String.h>
#include <efltk/Fl_WM.h>
#include <efltk/fl_draw.h>

#include <edeconf.h>

#include <signal.h>
#include <stdio.h>
#include <unistd.h> // access
#include <locale.h> // setlocale
#include <assert.h>

//#include <vector>
#include <algorithm> // sort
using std::sort;
using std::vector;

#define CONFIG_FILE         "ede.conf"
#define ICONS_DIR           "/.ede/desktop/"
#define DEFAULT_ICONS_DIR   PREFIX"/share/ede/icons/48x48/"
#define DEFAULT_ICON        PREFIX"/share/ede/icons/48x48/folder.png"

#define IS_READABLE(path)   (access(path, R_OK) == 0)
#define IS_EXSIST(path)     fl_file_exists(path)

bool running = true;

void exit_signal(int signum)
{
    printf("Exiting (got signal %d)\n", signum);
    running = false;
}

bool icons_sorter(Fl_Widget* w1, Fl_Widget* w2)
{
	if(w1->x() == w2->x() || w1->x() < (w2->x() + w2->w()))
		return w1->y() < w2->y();
	else 
		return w1->y() > w2->y();
}

Fl_String localized_string(void)
{
	Fl_String locale = setlocale(LC_MESSAGES, NULL);
	if(locale=="C" || locale=="POSIX") locale.clear();

	Fl_String localName;
	if(locale.empty()) localName = "Name";
	else localName.printf("Name[%s]", locale.c_str());

	return localName;
}

int icon_change_handler(int e)
{
	//TODO:
	return 0;
}

void cb_update_workarea(Fl_Widget *, void *arg)
{
	assert(arg != NULL);
	Desktop* d = (Desktop*)arg;
	d->update_workarea();
}

Desktop::Desktop() : Fl_Double_Window(0, 0, Fl::w(), Fl::h(), "")
{
	Fl_WM::callback(cb_update_workarea, this, Fl_WM::DESKTOP_WORKAREA);

	isel.x = isel.y = isel.w = isel.h = 0;
	isel.visible = false;

	window_type(Fl_WM::DESKTOP);

	gisett.label_background = 46848;
	gisett.label_foreground = FL_WHITE;
	gisett.label_fontsize = 12;
	gisett.label_maxwidth = 75;
	gisett.label_gridspacing = 16;
	gisett.label_transparent = false;
	gisett.label_draw = true;
	gisett.one_click_exec = false;
	gisett.auto_arr = false;
	gisett.same_size = false;

	bg_color = FL_BLUE;
	bg_opacity = 255;
	bg_mode = 0;
	bg_use = false;

	popup = new Fl_Menu_Button(0, 0, 0, 0);
	popup->type(Fl_Menu_Button::POPUP3);
	popup->begin();

    Fl_Item *menuit;
	const int off = 18;

    menuit = new Fl_Item(_("&New desktop item"));
    menuit->x_offset(off);

    menuit = new Fl_Item(_("&Refresh"));
    menuit->x_offset(off);

	new Fl_Menu_Divider();

	menuit = new Fl_Item(_("Line up vertical"));
    menuit->x_offset(off);
	menuit = new Fl_Item(_("Line up horizontal"));
    menuit->x_offset(off);

	new Fl_Menu_Divider();

	menuit = new Fl_Item(_("Arrange"));
    menuit->x_offset(off);
	menuit = new Fl_Item(_("Reverse arrange"));
    menuit->x_offset(off);

	new Fl_Menu_Divider();

    menuit = new Fl_Item(_("&Icons Settings "));
    menuit->x_offset(off);

    menuit = new Fl_Item(_("&Background Settings"));
    menuit->x_offset(off);

    popup->end();
	end();

	Fl_Config conf(fl_find_config_file(CONFIG_FILE, 0), true, false);
	read_bg_conf(conf);
	read_icons_conf(conf);

	update_bg();
	load_icons();
}

Desktop::~Desktop()
{
	/* icons member deleting is not needed, since add_icon will
	 * append to icons and to Fl_Group array. Desktop at the end
	 * will cleanup Fl_Group array.
	 */
	icons.clear();
}

void Desktop::update_workarea(void)
{
    int X = 0,Y = 0, W = w(),H = h();
    Fl_WM::get_workarea(X,Y,W,H);
    resize(X,Y,W,H);
    layout();
	if(gisett.auto_arr) 
		auto_arrange();
}

void Desktop::auto_arrange(void)
{
}

void Desktop::read_icons_conf(Fl_Config& conf)
{
	conf.set_section("IconManager");
    conf.read("Label Background", gisett.label_background, 46848);
    conf.read("Label Transparent", gisett.label_transparent, false);
    conf.read("Label Foreground", gisett.label_foreground, FL_WHITE);
    conf.read("Label Fontsize", gisett.label_fontsize, 12);
    conf.read("Label Maxwidth", gisett.label_maxwidth, 75);
    conf.read("Gridspacing", gisett.label_gridspacing, 16);
    conf.read("OneClickExec", gisett.one_click_exec, 0);
    conf.read("AutoArrange", gisett.auto_arr, false);

	// TODO: add this to default config file
	conf.read("AllEqualSize", gisett.same_size, false);
	conf.read("Label Visible", gisett.label_draw, true);
}

void Desktop::read_bg_conf(Fl_Config& conf)
{
	conf.set_section("Desktop");
    conf.read("Color", bg_color, (Fl_Color)fl_darker(FL_BLUE));
    conf.read("Opacity", bg_opacity, 255);
    conf.read("Mode", bg_mode, 0);
    conf.read("Use", bg_use, 1);
}

void Desktop::update_bg(void)
{
	color(bg_color);
	redraw();
}

bool Desktop::load_icon_file(const char* path, IconSettings& isett)
{
	assert(path != NULL);
	Fl_Config conf(path, true, false);

	Fl_String local_name;
	Fl_String exec;
	Fl_String icon_path;
	int x, y;

	// TODO: add section check
	conf.set_section("Desktop Entry");
	conf.get("Desktop Entry", localized_string(), local_name, "None");
	conf.read("X", x, 100);
	conf.read("Y", y, 100);
	conf.read("Exec", exec, "");
	conf.read("Icon", icon_path, "");

	// TODO: add IS_READABLE for paths

	isett.name = local_name;
	isett.cmd = exec;
	isett.x = x;
	isett.y = y;

	// check if path exists, and if not, try default
	if(IS_EXSIST(icon_path))
		isett.icon_path = icon_path;
	else
	{
		Fl_String tmp = DEFAULT_ICONS_DIR + icon_path;
		if(IS_EXSIST(tmp))
			isett.icon_path = tmp;
		else if(IS_EXSIST(DEFAULT_ICON))
			isett.icon_path = DEFAULT_ICON;
		else
			isett.icon_path = "";
	}

#if 0
	printf("Name: %s\n", local_name.c_str());
	printf("Exec: %s\n", exec.c_str());
	printf("Icon: %s\n", isett.icon_path.c_str());
	printf("X: %i Y: %i\n", x,y);
#endif
}

void Desktop::load_icons(void)
{
    Fl_String path(fl_homedir() + ICONS_DIR);
    if(!fl_is_dir(path))
		return;

	Fl_Directory_DS dds;
	dds.directory(path);
	dds.open();

	while(!dds.eof())
	{
		Fl_String name(dds["Name"].get_string());
		Fl_String filename(dds.directory() + name);

		dds.next();

		if(IS_READABLE(path))
		{
			if(fl_file_match(name, "*.desktop")) 
			{
				printf("Loading %s\n", filename.c_str());

				IconSettings s;
				load_icon_file(filename.c_str(), s);

				// now create icon and add it to our Fl_Group
				Icon* ic = new Icon(&gisett, &s);
				add_icon(ic);
			}
		}
	}

	sort_internals();
}

void Desktop::add_icon(Icon* ic)
{
	assert(ic != NULL);
	icons.push_back(ic);
	add(ic);
}

/* Sort Fl_Group's array, according to x,y.
 * Used for easier navigation in FL_Up/FL_Down keys.
 */
void Desktop::sort_internals(void)
{
/*
	vector<Fl_Widget*> vs;
	vs.reserve(children());
	for(int i = children(); i--;)
	{
		vs.push_back(child(i));
		remove(child(i));
	}

	sort(vs.begin(), vs.end(), icons_sorter);
	for(int i = 0; i < vs.size(); i++)
	{
		add(vs[i]);
		printf("x:%i y: %i l: %s\n", vs[i]->x(), vs[i]->y(), vs[i]->label().c_str());
	}
*/
}

void Desktop::move_selection(int x, int y)
{
	if(selectionbuff.empty())
		return;
	int prev_x, prev_y;

	for(uint i = 0; i < selectionbuff.size(); i++)
	{
		prev_x = selectionbuff[i]->x();
		prev_y = selectionbuff[i]->y();
		selectionbuff[i]->position(prev_x + x, prev_y + y);
	}
}

void Desktop::unfocus_all(void)
{
	for(uint i = 0; i < icons.size(); i++)
	{
		icons[i]->do_unfocus();
		icons[i]->redraw();
	}
}

void Desktop::draw(void)
{
	fl_color(bg_color);
	fl_rectf(0,0,w(),h());

	for(uint n = icons.size(); n--;) 
	{
		Icon* ic = icons[n];
		if(fl_not_clipped(ic->x(), ic->y(), ic->w(), ic->h()))
		{
			fl_push_matrix();
			fl_translate(ic->x(), ic->y());
			ic->set_damage(FL_DAMAGE_ALL|FL_DAMAGE_EXPOSE);
			ic->draw();
			ic->set_damage(0);
			fl_pop_matrix();
		}
    }
}

int Desktop::handle(int event)
{
	// up/down keys are not sent to children
	if(event == FL_KEY)
	{
		return 1;
	}
	
	// handle other events or send them to children
	int ret = Fl_Double_Window::handle(event);

	switch(event)
	{
		case FL_PUSH:
			if(Fl::event_button() == 1 && (Fl::get_key_state(FL_Shift_L) || Fl::get_key_state(FL_Shift_R)))
			{
				/* Check is event inside one of children.
				 * Means, on desktop we can click either on
				 * icon or desktop. If we do not click
				 * on desktop, then we certainly clicked on icon.
				 */
				Fl_Widget* clicked = Fl::belowmouse();
				if(clicked != this)
				{
					for(uint i = 0; i < icons.size(); i++)
						if(icons[i] == clicked)
						{
							icons[i]->do_focus();
							icons[i]->redraw();
							selectionbuff.push_back(icons[i]);
							puts("ADDED");
						}
				}
				return 1;
			}

			unfocus_all();

			// send events to children
			if(Fl::belowmouse() != this)
				return Fl_Double_Window::handle(event);

			if(Fl::event_button() == 3)
				popup->Fl_Menu_::popup(Fl::event_x_root(), Fl::event_y_root());

			return 1;
		case FL_RELEASE:
			return 1;
		case FL_DRAG:
			puts("DRAGGGG");
			if(!selectionbuff.empty())
				move_selection(Fl::event_x(), Fl::event_y());
			return 1;
		case FL_FOCUS:
		case FL_UNFOCUS:
			return 1;
		default:
			break;
	}
	return ret;
	//return Fl_Double_Window::handle(event);
}

int main(int argc, char** argv)
{
    signal(SIGTERM, exit_signal);
    signal(SIGKILL, exit_signal);
    signal(SIGINT, exit_signal);

	fl_init_locale_support("eiconman", PREFIX"/share/locale");
	fl_init_images_lib();
	
	Desktop *desktop = new Desktop();
	desktop->show();
	Fl::add_handler(icon_change_handler);

	while(running) 
		Fl::wait();

	delete desktop;
	return 0;
}
