/*
 * $Id$
 *
 * Edewm, window manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include "Theme.h"
#include "debug.h"
#include <efltk/Fl.h>

Theme* Theme::pinstance = NULL;

Theme::Theme() : use_theme(false), tbheight(20), framecol(FL_NO_COLOR),
				 framecol_unfocus(FL_NO_COLOR)
{
	DBG("Initializing theme support");
}

Theme::~Theme()
{
	unload();
	DBG("Shutdown theme support");
}

Theme* Theme::instance()
{
	if(Theme::pinstance == NULL)
		Theme::pinstance = new Theme();
	return Theme::pinstance;
}

void Theme::shutdown()
{
	if(Theme::pinstance != NULL)
	{
		delete Theme::pinstance;
		Theme::pinstance = NULL;
	}
}

void Theme::load_image(const char *name, int type, const Fl_String &path, Fl_Config &cfg)
{
    Fl_String filename;
    cfg.read(name, filename, 0);
    Fl_String img_path(path + filename);

    Fl_Image* img = Fl_Image::read(img_path);
	DBG("Theme, loading image: %s (type: %i, name: %s)", img_path.c_str(), type, name);
	if(img)
	{
		img->state_effect(false);
		images[type] = img;
		imglist.append(img);
		DBG("Ok \n");
	}
	else
	{
		DBG("Failed, deleting...\n");
		delete img;
	}
}

Fl_Color fc;
bool Theme::load(const Fl_String &file)
{
	if(!fl_file_exists(file))
		return false;

	int pos = file.rpos('/');
    if(pos==-1) return false;

	// unload current theme
	// TODO: add checking if some theme is loaded
    unload();

	Fl_String path(file.sub_str(0, pos+1));
    Fl_String filename;

    Fl_Config themeconf(file);
    themeconf.set_section("Theme");

	printf("%i\n", framecol);
    //themeconf.read("Frame color", framecol, FL_NO_COLOR);
    themeconf.read("Frame color", fc, FL_NO_COLOR);
    themeconf.read("Frame color unfocused", framecol_unfocus, FL_NO_COLOR);

	// default is -1, which means no "Title height" information in
	// theme file
	themeconf.read("Title height", tbheight, -1);

	load_image("Title image", TITLEBAR_BG, path, themeconf);
	load_image("Title image unfocused", TITLEBAR_BG_UNFOCUSED, path, themeconf);
	load_image("Close image up", TITLEBAR_CLOSE_UP, path, themeconf);
	load_image("Close image down", TITLEBAR_CLOSE_DOWN, path, themeconf);
	load_image("Close image unfocused", TITLEBAR_CLOSE_UNFOCUSED, path, themeconf);
    load_image("Maximize image up", TITLEBAR_MAX_UP, path, themeconf);
    load_image("Maximize image down", TITLEBAR_MAX_DOWN, path, themeconf);
    load_image("Maximize image unfocused", TITLEBAR_MAX_UNFOCUSED, path, themeconf);
    load_image("Restore image up", TITLEBAR_RESTORE_UP, path, themeconf);
    load_image("Restore image down", TITLEBAR_RESTORE_DOWN, path, themeconf);
    load_image("Restore image unfocused", TITLEBAR_RESTORE_UNFOCUSED, path, themeconf);
	load_image("Minimize image up", TITLEBAR_MIN_UP, path, themeconf);
	load_image("Minimize image down", TITLEBAR_MIN_DOWN, path, themeconf);
	load_image("Minimize image unfocused", TITLEBAR_MIN_UNFOCUSED, path, themeconf);

	return true;
}

void Theme::unload()
{
    framecol = FL_NO_COLOR;
    framecol_unfocus = FL_NO_COLOR;

    for(int i=0; i < IMAGES_LAST; i++)
        images[i] = 0;

	if(imglist.size() <= 0)
		return;

    for(uint i=0; i < imglist.size(); i++)
        delete imglist[i];

    imglist.clear();
}

Fl_Image* Theme::image(int which)
{
	DBG("Theme, getting type %i\n", which);
    if(which<0 || which>=IMAGES_LAST) {
        Fl::warning("Invalid theme image index: %d", which);
        return 0;
    }

    return images[which];
}
