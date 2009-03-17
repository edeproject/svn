/*
 * $Id$
 *
 * Icon loader via IconTheme
 * Copyright (c) 2009 edelib authors
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Fl.H>
#include <FL/Fl_Button.H>

#include <edelib/List.h>
#include <edelib/String.h>
#include <edelib/Debug.h>
#include <edelib/IconLoader.h>

EDELIB_NS_BEGIN

static const char* fallback_icon  = "empty";
IconLoader* IconLoader::pinstance = NULL;

struct IconLoaderItem {
	String      name;
	String      path;
	IconContext context;
	IconSizes   size;

	Fl_Widget*  widget;
};

typedef list<IconLoaderItem*>           Items;
typedef list<IconLoaderItem*>::iterator ItemsIter;

static IconLoaderItem* find_item(Items& items, const char* icon_name) {
	ItemsIter it = items.begin(), it_end = items.end();

	for(; it != it_end; ++it) {
		if((*it)->name == icon_name)
			return (*it);
	}

	return NULL;
}

static IconLoaderItem* get_or_create_item(Items& items, 
		const char* name, IconSizes sz, IconContext ctx, 
		IconTheme* theme, Fl_Widget* widg) 
{
	IconLoaderItem* item = find_item(items, name);
	if(!item) {
		item = new IconLoaderItem;
		item->name = name;
		item->path = theme->find_icon(name, sz, ctx);
		item->size = sz;
		item->context = ctx;
		item->widget = widg;

		/* register it */
		items.push_back(item);
	}

	return item;
}

IconLoader::IconLoader() { 
	curr_theme = new IconTheme;
}

IconLoader::~IconLoader() { 
	clear_items();
	delete curr_theme;
}

void IconLoader::clear_items(void) {
	ItemsIter it = items.begin(), it_end = items.end();

	for(; it != it_end; ++it)
		delete *it;
	items.clear();
}

void IconLoader::load_theme(const char* name) {
	curr_theme->load(name);
}

const char* IconLoader::get_icon_path(const char* name, IconSizes sz, IconContext ctx) {
	IconLoaderItem* item;
	
	item = get_or_create_item(items, name, sz, ctx, curr_theme, NULL);
	return item->path.c_str();
}

Fl_Shared_Image* IconLoader::get_icon(const char* name, IconSizes sz, IconContext ctx) {
	Fl_Shared_Image* img;
	const char* path;
	
	path = get_icon_path(name, sz, ctx);
	img = Fl_Shared_Image::get(path);
	return img;
}

bool IconLoader::set_icon(const char* name, Fl_Widget* widget, IconSizes sz, IconContext ctx) {
	Fl_Shared_Image* img;
	IconLoaderItem* item;
	
	item = get_or_create_item(items, name, sz, ctx, curr_theme, widget);
	img = Fl_Shared_Image::get(item->path.c_str());

	if(!img) {
		/* no image, try fallback then */
		item = get_or_create_item(items, fallback_icon, sz, ctx, curr_theme, widget);
		img = Fl_Shared_Image::get(item->path.c_str());
	}

	/* nothing, bail out */
	if(!img)
		return false;

	widget->image(img);
	widget->redraw();
	return true;
}

void IconLoader::reload_icons(void) {
	ItemsIter it = items.begin(), it_end = items.end();
	IconLoaderItem* item;

	/* iterate over the list and update each icon name with the new path */
	for(; it != it_end; ++it) {
		item = *it;
		item->path = curr_theme->find_icon(item->name.c_str(), item->size, item->context);

		/* also update widget icon */
		if(item->widget) {
			Fl_Shared_Image* img = Fl_Shared_Image::get(item->path.c_str());
			if(img) {
				Fl_Widget* wi = item->widget;
				wi->image(img);
				wi->redraw();
			}
		}
	}		
}

void IconLoader::repoll_icons(void) {
	Fl_Shared_Image* img;
	ItemsIter it = items.begin(), it_end = items.end();

	while(it != it_end) {
		/* 
		 * when widget get's destroyed, it's image goes too; this is good since we can
		 * check that with Fl_Shared_Image::find() function since it will not exists in Fl_Shared_Image cache
		 */
		img = Fl_Shared_Image::find((*it)->path.c_str());
		if(!img) {
			/* if not exists, then it was deleted; remove it from our list then */
			delete *it;
			it = items.erase(it);
		}

		++it;
	}
}

void IconLoader::init(const char* theme) {
	if(!IconLoader::pinstance)
		IconLoader::pinstance = new IconLoader();

	IconLoader::pinstance->load_theme(theme);
}

void IconLoader::shutdown(void) {
	delete IconLoader::pinstance;
	IconLoader::pinstance = NULL;
}

bool IconLoader::inited(void) {
	return (IconLoader::pinstance != NULL);
}

void IconLoader::reload(const char* theme) {
	IconLoader::pinstance->load_theme(theme);
	IconLoader::pinstance->reload_icons();
}

IconLoader* IconLoader::instance() {
	E_ASSERT(IconLoader::pinstance != NULL && "Did you run IconLoader::init() first?");
	return IconLoader::pinstance;
}

Fl_Shared_Image* IconLoader::get(const char* name, IconSizes sz, IconContext ctx) {
	return IconLoader::instance()->get_icon(name, sz, ctx);
}

String IconLoader::get_path(const char* name, IconSizes sz, IconContext ctx) {
	return IconLoader::instance()->get_icon_path(name, sz, ctx);
}

bool IconLoader::set(Fl_Widget* widget, const char* name, IconSizes sz, IconContext ctx) {
	return IconLoader::instance()->set_icon(name, widget, sz, ctx);
}

const IconTheme* IconLoader::theme(void) {
	return IconLoader::instance()->current_theme();
}

void IconLoader::set_fallback_icon(const char* name) {
	fallback_icon = name;
}

const char* IconLoader::get_fallback_icon(void) {
	return fallback_icon;
}

EDELIB_NS_END

using namespace edelib;

void change_cb(Fl_Widget*, void* win_) {
	IconLoader::reload("edeneu");
	Fl_Window* win = (Fl_Window*)win_;
	win->redraw();
}

int main() {
#if 0
	IconLoader il;

	printf(">>> %s\n", il.get_icon_path("empty", ICON_SIZE_MEDIUM));
	printf(">>> %s\n", il.get_icon_path("empty", ICON_SIZE_MEDIUM));
	printf(">>> %s\n", il.get_icon_path("autopackage-installer", ICON_SIZE_LARGE));
	//IconTheme::shutdown();
	//IconTheme::init("gnome");
	il.reload_icons();
	printf(">>> %s\n", il.get_icon_path("autopackage-installer", ICON_SIZE_LARGE));
	printf(">>> %s\n", il.get_icon_path("empty", ICON_SIZE_LARGE));
#endif

#if 0
	IconTheme it;
	it.load("edeneu");
	printf(">> %s\n", it.find_icon("serviceconf", ICON_SIZE_MEDIUM).c_str());
	it.load("gnome");
	printf(">> %s\n", it.find_icon("serviceconf", ICON_SIZE_MEDIUM).c_str());
	printf("|%s| |%s| |%s|\n", it.stylized_theme_name(), it.description(), it.example_icon());

	list<String> lst;
	it.query_icons(lst, ICON_SIZE_MEDIUM);
	list<String>::iterator iter = lst.begin(), it_end = lst.end();

	while(iter != it_end) {
		printf("=> %s\n", (*iter).c_str());
		++iter;
	}
#endif

	fl_register_images();
	IconLoader::init("edeneu");
	Fl_Window* win = new Fl_Window(400, 400, "Some window");	
	win->begin();
		Fl_Box* bb = new Fl_Box(0, 0, 400, 100);
		IconLoader::set(bb, "utilities-terminal", ICON_SIZE_LARGE);
		Fl_Button* button = new Fl_Button(50, 200, 90, 25, "Click me");
		button->callback(change_cb, win);
	win->end();
	win->show();
	Fl::run();
	IconLoader::shutdown();

	return 0;
}


