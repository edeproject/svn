/*
 * $Id$
 *
 * Eiconman, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include "edeskicon.h"
#include "eiconman.h"

#include <efltk/Fl_Item.h>
#include <efltk/Fl_Divider.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Tooltip.h>
#include <efltk/fl_draw.h>

#ifdef SHAPE
	#include <X11/extensions/shape.h>
#endif

#include <assert.h>

#define ICONSIZE 48

Icon::Icon(const GlobalIconSettings* gs, const IconSettings* s) : Fl_Widget(s->x, s->y, ICONSIZE, ICONSIZE, "")
{
	assert(gs != NULL);
	assert(s != NULL);
	globals = gs;
	settings = s;
	lwidth = lheight = 0;
	infocus = false;
	micon = NULL;

	popup = new Fl_Menu_Button(0, 0, 0, 0);
	// ?@?
	if(popup->parent())
		popup->parent()->remove(popup);
	popup->parent(0);
	popup->type(Fl_Menu_Button::POPUP3);
	popup->begin();

	const int off = 12;
	Fl_Item* openit = new Fl_Item(_("&Open"));
	openit->x_offset(off);

	Fl_Item* deleteit = new Fl_Item(_("&Delete"));
	deleteit->x_offset(off);

	new Fl_Divider();

	Fl_Item* propit = new Fl_Item(_("&Properties"));
	propit->x_offset(off);

	popup->end();

	box(FL_NO_BOX);

	tooltip(settings->name);
	label(settings->name);
	label_color(globals->label_foreground);
	label_size(globals->label_fontsize);
	align(FL_ALIGN_RIGHT|FL_ALIGN_WRAP);
	update_label_size(settings->name.c_str(), globals->label_maxwidth);

	icon_img = Fl_Image::read(settings->icon_path, 0);
}

Icon::~Icon()
{
	puts("Icon::~Icon()");
	if(micon != NULL)
		delete micon;
}

void Icon::update_label_size(const char* ll, int maxwidth)
{
	assert(ll != NULL);

    lwidth = maxwidth;
    lheight= 0;
    fl_font(label_font(), label_size());
    fl_measure(ll, lwidth, lheight, FL_ALIGN_WRAP);
    lwidth += 4;
}

void Icon::draw(void)
{
	Fl_Flags f = 0;
    if(is_focused()) 
		f = FL_SELECTED;

	if(icon_img)
		icon_img->draw(0, 0, w(), h(),f);
	else
	{
		fl_color(FL_RED);
		fl_rect(0,0,w(),h());
		fl_color(FL_BLACK);
		fl_rectf(1,1,w()-2,h()-2);
		fl_color(FL_WHITE);
		fl_font(label_font()->bold(), 10);
		fl_draw(_("NO ICON FOUND!"), 1, 1, w()-2, h()-2, FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_WRAP);
		return;
	}

	if(globals->label_draw)
	{
    	int X = w()-(w()/2)-(lwidth/2);
		int Y = h()+2;
		if(!globals->label_transparent) 
		{
        	fl_color(globals->label_background);
			fl_rectf(X,Y,lwidth+4,lheight+2);
		}

    	if(is_focused())
			focus_box()->draw(X, Y, lwidth+4, lheight+2, color(), 0);

    	fl_font(label_font(), label_size());
		fl_color(label_color());
		fl_draw(label(), X, Y+1, lwidth, lheight, flags());
    }
}

void Icon::layout(void)
{
	if(layout_damage()&FL_LAYOUT_XYWH && icon_img)
	{
		if(icon_img) 
		{
			icon_img->mask_type(MASK_ALPHA);
			icon_img->threshold(128);
		}
	}

	Fl_Widget::layout();
}

void Icon::delete_icon(void)
{
}

void Icon::update_all(void)
{
}

void Icon::drag(int x, int y, bool apply)
{
	if(!micon)
	{
		micon = new MovableIcon(this);
		micon->show();
	}

	assert(micon != NULL);
	micon->position(x, y);

	if(apply)
	{
		position(micon->x(), micon->y());
		delete micon;
		micon = 0;
	}
}

// Used only in Desktop::move_selection
int Icon::drag_icon_x(void)
{
	if(!micon)
		return x();
	else
		return micon->x();
}

// Used only in Desktop::move_selection
int Icon::drag_icon_y(void)
{
	if(!micon)
		return y();
	else
		return micon->y();
}

void Icon::do_focus(void)
{
	if(is_focused())
		return;
	if(!accept_focus())
		return;
	if(!takesevents()) 
		return;

	printf("%s focused\n", label().c_str());
	infocus = true;
}
void Icon::do_unfocus(void)
{
	printf("%s ufocused\n", label().c_str());
	infocus = false;
}

// one big TODO: move this to apropriate place
bool button1 = false;
bool moving = false;
int tx, ty;

int Icon::handle(int event)
{
	if(event == FL_PUSH)
		button1 = (Fl::event_button() == 1);

	if(button1)
	{
		switch(event)
		{
			case FL_PUSH:
				do_focus();
				redraw();
				return 1;

			case FL_DRAG:
				moving = true;
				printf("DRAGG fron Icon\n");
				drag(Fl::event_x_root()-w()/2, Fl::event_y_root()-h()/2, false);
				return 1;

			case FL_RELEASE:
				moving = false;
				drag(Fl::event_x_root()-w()/2, Fl::event_y_root()-h()/2, true);
				redraw();
				return 1;
		}
	}

	switch(event)
	{
		case FL_PUSH:
			// prevent click on any other mouse buttons during move
			if(!moving && Fl::event_button() == 3)
			{
				do_focus();
				redraw();
				popup->popup();
			}
			return 1;
		case FL_ENTER:
			return 1;
		case FL_LEAVE:
			Fl_Tooltip::exit();
			return 1;
/*
		case FL_FOCUS:
			printf("%s focused\n", label().c_str());
			return 1;
		case FL_UNFOCUS:
			printf("%s unfocused\n", label().c_str());
			return 1;
*/
		default:
			return Fl_Widget::handle(event);
	}
	return 0;
}


MovableIcon::MovableIcon(Icon* i) : Fl_Window(i->x(), i->y(), i->w(), i->h()), icon(i)
{
	assert(icon != NULL);
	set_override();
	create();

	Fl_Image* img = icon->icon_image();
	if(img)
	{
#ifdef SHAPE
		Pixmap mask = img->create_mask(img->width(), img->height());
		XShapeCombineMask(fl_display, fl_xid(this), ShapeBounding, 0, 0, mask, ShapeSet);
#endif
		align(FL_ALIGN_INSIDE);
		image(img);
	}
}

MovableIcon::~MovableIcon()
{
}
