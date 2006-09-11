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
#include "icondialog.h"

#include <efltk/Fl_Item.h>
#include <efltk/Fl_Divider.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Tooltip.h>
#include <efltk/fl_ask.h>
#include <efltk/fl_draw.h>

#include <assert.h>

#ifdef SHAPE
	#include <X11/extensions/shape.h>
#endif


#define ICONSIZE 48

void rename_cb(Fl_Widget*, void* ic)
{
	Icon* icon = (Icon*)ic;
	assert(icon != NULL);
	const char* val = fl_input(_("Rename icon:"), icon->label().c_str());

	if(val)
		icon->update_label(val);
}

void delete_cb(Fl_Widget*, void* ic)
{
	Icon* icon = (Icon*)ic;
	assert(icon != NULL);

	fl_ask(_("Delete %s ?"), icon->label().c_str());
}

void prop_cb(Fl_Widget*, void* ic)
{
	Icon* icon = (Icon*)ic;
	assert(icon != NULL);

	IconDialog id(icon->icon_settings(), icon->icon_image());
	printf("%s\n", icon->icon_settings()->name.c_str());
	id.exec();
}

Icon::Icon(const GlobalIconSettings* gs, IconSettings* s) : Fl_Widget(s->x, s->y, ICONSIZE, ICONSIZE, "")
{
	assert(gs != NULL);
	assert(s != NULL);

	/* GlobalIconSettings and IconSettings are shared
	 * via single object who is changed from eiconman.
	 * Since IconSettings are used more, we allocate for it.
	 */
	globals = gs;

	settings = new IconSettings;
	settings->name = s->name;
	settings->cmd  = s->cmd;
	settings->icon_path = s->icon_path;

	/* settings->x and settings->y are not needed
	 * since x() and y() have it.
	 */

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

	Fl_Item* renameit = new Fl_Item(_("&Rename"));
	renameit->x_offset(off);
	renameit->callback(rename_cb, this);

	Fl_Item* deleteit = new Fl_Item(_("&Delete"));
	deleteit->x_offset(off);
	deleteit->callback(delete_cb, this);

	new Fl_Divider();

	Fl_Item* propit = new Fl_Item(_("&Properties"));
	propit->x_offset(off);
	propit->callback(prop_cb, this);

	popup->end();

	box(FL_NO_BOX);

	//tooltip(tt);

	copy_label(settings->name);
	label_color(globals->label_foreground);
	label_size(globals->label_fontsize);
	align(FL_ALIGN_CENTER|FL_ALIGN_WRAP);
	update_label_size();

	icon_img = Fl_Image::read(settings->icon_path, 0);
}

Icon::~Icon()
{
	puts("Icon::~Icon()");
	if(micon != NULL)
		delete micon;

	if(settings)
		delete settings;
}

IconSettings* Icon::icon_settings(void)
{
	assert(settings != NULL);
	return settings;
}

void Icon::update_label_size(void)
{
    lwidth = globals->label_maxwidth;
    lheight= 0;
    fl_font(label_font(), label_size());
    fl_measure(label(), lwidth, lheight, FL_ALIGN_WRAP);

    lwidth += 8;
	lheight += 4;
}

void Icon::update_label(const char* ll)
{
	assert(ll != NULL);

	copy_label(ll);
	update_label_size();
	redraw();
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
			fl_rectf(X,Y,lwidth,lheight);
		}

    	if(is_focused())
			focus_box()->draw(X, Y, lwidth, lheight, color(), 0);

    	fl_font(label_font(), label_size());
		fl_color(label_color());
		fl_draw(label(), X, Y, lwidth, lheight, flags());
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

int Icon::handle(int event)
{
	switch(event)
	{
		case FL_ENTER:
			//if(!Fl::event_state())
			//	icon_tooltip->show_tooltip(Fl::event_x_root(), Fl::event_y_root(), EXTENDED);
			puts("ENTER");
			return 1;
		case FL_LEAVE:
			puts("LEAVE");
			//if(!(Fl::event_state() & FL_BUTTONS))
			//	icon_tooltip->hide_tooltip();
			return 1;
		/* We have to handle FL_MOVE too, if
		 * want to get only once FL_ENTER when
		 * entered or FL_LEAVE when leaved.
		 */
		case FL_MOVE:
			return 1;
		case FL_PUSH:
			printf("FL_PUSH on icon %s\n", label().c_str());
			//icon_tooltip->hide_tooltip();
			if(Fl::event_button() == 3)
				popup->popup();
			return 1;
	}

	return Fl_Widget::handle(event);
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
