/*
 * $Id$
 *
 * Eiconman, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2007 EDE Authors.
 *
 * This program is licensed under terms of the 
 * GNU General Public License version 2 or newer.
 * See COPYING for details.
 */

#include "DesktopIcon.h"
#include "eiconman.h"
#include "Utils.h"

#include <FL/Fl.h>
#include <FL/fl_draw.h>
#include <FL/Fl_Shared_Image.h>
#include <FL/Fl_Menu_Button.h>
#include <FL/x.h>
#include <FL/fl_ask.h>

#include <edelib/Debug.h>
#include <edelib/IconTheme.h>
#include <edelib/Nls.h>

#define USE_SHAPE 1

#ifdef USE_SHAPE
	#include <X11/extensions/shape.h>
#endif

// minimal icon size
#define ICONSIZE 48

// spaces around box in case of large/small icons
#define OFFSET_W 16
#define OFFSET_H 16

// label offset from icon y()+h(), so selection box can be drawn nicely
#define LABEL_OFFSET 2


void rename_cb(Fl_Widget*, void* d);

Fl_Menu_Item icon_menu[] = {
	{_("    &Open    "),   0, 0},
	{_("    &Rename    "), 0, rename_cb, 0},
	{_("    &Delete    "), 0, 0, 0, FL_MENU_DIVIDER},
	{_("    &Properties    "), 0, 0},
	{0}
};

Fl_Menu_Item icon_trash_menu[] = {
	{_("    &Open    "),       0, 0},
	{_("    &Properties    "), 0, 0, 0, FL_MENU_DIVIDER},
	{_("    &Empty    "),      0, 0},
	{0}
};

void rename_cb(Fl_Widget*, void* d) {
	DesktopIcon* di = (DesktopIcon*)d;
	EASSERT(di != NULL);

	const char* new_name = fl_input(_("New name"), di->label());
	if(!new_name)
		return;
	if(new_name[0] == '\0')
		return;
	di->rename(new_name);
}

DesktopIcon::DesktopIcon(GlobalIconSettings* gs, IconSettings* is, int bg) : 
	Fl_Widget(is->x, is->y, ICONSIZE, ICONSIZE) {

	EASSERT(gs != NULL);

	lwidth = lheight = 0;
	focus = false;
	micon = NULL;

	/*
	 * GlobalIconSettings is shared from desktop so we only
	 * reference it. On other hand IconSettings is not shared
	 * and we must construct a copy from given parameter
	 */
	globals = gs;

	settings = new IconSettings;
	settings->name = is->name;
	settings->cmd  = is->cmd;
	settings->icon = is->icon;
	settings->type = is->type;
	settings->key_name= is->key_name;
	settings->full_path = is->full_path;

	// x,y are not needed since x(), y() are filled with it
	
	// setting fonts is TODO :P
#if 0	
	Fl::set_font((Fl_Font)20, "-windows-*-medium-r-*-*-14-*-*-*-*-*-*-*");
	labelfont((Fl_Font)20);
#endif

	label(settings->name.c_str());

	imenu = new Fl_Menu_Button(0, 0, 0, 0);
	if(settings->type == ICON_TRASH)
		imenu->menu(icon_trash_menu);
	else
		imenu->menu(icon_menu);

	if(!settings->icon.empty()) {
		const char* nn = settings->icon.c_str();

		//edelib::String ipath = edelib::IconTheme::get(nn, edelib::ICON_SIZE_MEDIUM);
		edelib::String ipath = edelib::IconTheme::get(nn, edelib::ICON_SIZE_HUGE);
		if(!ipath.empty()) {
			Fl_Image* img = Fl_Shared_Image::get(ipath.c_str());
			if(img) {
				int img_w = img->w();
				int img_h = img->h();

				// resize box if icon is larger
				if(img_w > ICONSIZE || img_h > ICONSIZE)
					size(img_w + OFFSET_W, img_h + OFFSET_H);

				image(img);
			} else 
				EDEBUG(ESTRLOC ": Unable to load %s\n", ipath.c_str());
		} else
			EDEBUG(ESTRLOC ": Got empty icon name ?!?\n");
	}

	fix_position(x(), y());

	//Use desktop color as color for icon background
	color(bg);

	align(FL_ALIGN_WRAP);
	update_label_size();
}

DesktopIcon::~DesktopIcon() { 
	EDEBUG("DesktopIcon::~DesktopIcon()\n");

	if(settings)
		delete settings;
	if(micon)
		delete micon;

	delete imenu;
}

void DesktopIcon::update_label_size(void) {
    lwidth = globals->label_maxwidth;
    lheight= 0;

	/*
	 * make sure current font size/type is set (internaly to fltk)
	 * so fl_measure() can correctly calculate label width and height
	 */
	int old = fl_font();
	int old_sz = fl_size();
	fl_font(labelfont(), labelsize());

	fl_measure(label(), lwidth, lheight, align());

	fl_font(old, old_sz);

    lwidth += 12;
	lheight += 5;
}

void DesktopIcon::fix_position(int X, int Y) {
	int dx, dy, dw, dh;
	Desktop::instance()->area(dx, dy, dw, dh);

	if(X < dx)
		X = dx;
	if(Y < dy)
		Y = dy;
	if(X + w() > dw)
		X = (dx + dw) - w();
	if(Y + h() > dh)
		Y = (dy + dh) - h();

	position(X, Y);
}

void DesktopIcon::drag(int x, int y, bool apply) {
	if(!micon) {
		micon = new MovableIcon(this);
#if USE_SHAPE
		/* 
		 * This is used to calculate correct window startup/ending
		 * position since icon is placed in the middle of the box.
		 *
		 * Opposite, window (shaped) will have small but noticeable 'jump off' and
		 * dropped icon position will not be at the exact place where was dropped.
		 */
		int ix, iy;
		ix = iy = 0;
		if(image()) {
			ix = (w()/2) - (image()->w()/2);
			iy = (h()/2) - (image()->h()/2);
		}

		micon->position(micon->x() + ix, micon->y() + iy);
#endif
		micon->show();
	} else {
		EASSERT(micon != NULL);

		micon->position(x, y);
	}

	if(apply) {
#if USE_SHAPE
		int ix, iy;
		ix = iy = 0;
		if(image()) {
			ix = (w()/2) - (image()->w()/2);
			iy = (h()/2) - (image()->h()/2);
		}
		fix_position(micon->x() - ix, micon->y() - iy);
#else
		fix_position(micon->x(), micon->y());
#endif
		delete micon;
		micon = NULL;
	}
}

// Used only in Desktop::move_selection
int DesktopIcon::drag_icon_x(void) {
	if(!micon)
		return x();
	else 
		return micon->x();
}

// Used only in Desktop::move_selection
int DesktopIcon::drag_icon_y(void) {
	if(!micon)
		return y();
	else
		return micon->y();
}

void DesktopIcon::rename(const char* str) {
	if(!str)
		return;
	if(settings->name == str)
		return;

	settings->name = str;
	label(settings->name.c_str());
	update_label_size();
	redraw();
}

void DesktopIcon::fast_redraw(void) {
	EASSERT(parent() != NULL && "Impossible !");

	int wsz = w();
	int xpos = x();

	if(lwidth > w()) {
		wsz = lwidth + 4;
		xpos = x() - 4;
	}

	// LABEL_OFFSET + 2 include selection box line height too; same for xpos
	parent()->damage(FL_DAMAGE_ALL, xpos, y(), wsz, h() + lheight + LABEL_OFFSET + 2);
}

void DesktopIcon::draw(void) { 
	//draw_box(FL_UP_BOX, FL_BLACK);

	if(image() && (damage() & FL_DAMAGE_ALL)) {
		Fl_Image* im = image();

		// center image in the box
		int ix = (w()/2) - (im->w()/2);
		int iy = (h()/2) - (im->h()/2);
		ix += x();
		iy += y();

		im->draw(ix, iy);

		EDEBUG(ESTRLOC ": DesktopIcon icon redraw\n");
	}

	if(globals->label_draw && (damage() & (FL_DAMAGE_ALL | EDAMAGE_CHILD_LABEL))) {
		int X = x() + w()-(w()/2)-(lwidth/2);
		int Y = y() + h() + LABEL_OFFSET;

		Fl_Color old = fl_color();

		if(!globals->label_transparent) {
			fl_color(globals->label_background);
			fl_rectf(X, Y, lwidth, lheight);
		}

		int old_font = fl_font();
		int old_font_sz = fl_size();

		// draw with icon's font
		fl_font(labelfont(), labelsize());

		// pseudo-shadow
		fl_color(FL_BLACK);
		fl_draw(label(), X+1, Y+1, lwidth, lheight, align(), 0, 0);

		fl_color(globals->label_foreground);
		fl_draw(label(), X, Y, lwidth, lheight, align(), 0, 0);

		// restore old font
		fl_font(old_font, old_font_sz);

		if(is_focused()) {
			/* 
			 * draw focused box on our way so later
			 * this can be used to draw customised boxes
			 */
			fl_color(globals->label_foreground);
			fl_line_style(FL_DOT);

			fl_push_matrix();
			fl_begin_loop();
				fl_vertex(X,Y);
				fl_vertex(X+lwidth,Y);
				fl_vertex(X+lwidth,Y+lheight);
				fl_vertex(X,Y+lheight);
				fl_vertex(X,Y);
			fl_end_loop();
			fl_pop_matrix();

			// revert to default line style
			fl_line_style(0);
		}

		// revert to old color whatever that be
		fl_color(old);

		EDEBUG(ESTRLOC ": DesktopIcon label redraw\n");
	}
}

int DesktopIcon::handle(int event) {
	switch(event) {
		case FL_FOCUS:
		case FL_UNFOCUS:
		case FL_ENTER:
		case FL_LEAVE:
			return 1;
		/* 
		 * We have to handle FL_MOVE too, if want to get only once 
		 * FL_ENTER when entered or FL_LEAVE when leaved.
		 */
		case FL_MOVE:
			return 1;
		case FL_PUSH:
			if(Fl::event_button() == 3) {
				// Fl_Menu_Item::popup() by default does not call callbacks
				const Fl_Menu_Item* m = imenu->menu()->popup(Fl::event_x(), Fl::event_y());
				if(m && m->callback())
					m->do_callback(0, this);
			}
			return 1;
		case FL_RELEASE:
			if(Fl::event_clicks() > 0)
				EDEBUG(ESTRLOC ": EXECUTE: %s\n", settings->cmd.c_str());
			return 1;

		case FL_DND_ENTER:
		case FL_DND_DRAG:
		case FL_DND_LEAVE:
			return 1;

		case FL_DND_RELEASE:
			EDEBUG(ESTRLOC ": FL_DND_RELEASE on icon\n");
			return 1;
		case FL_PASTE:
			EDEBUG(ESTRLOC ": FL_PASTE on icon with %s\n", Fl::event_text());
			return 1;
		default:
			break;
	}

	return 0;
}

MovableIcon::MovableIcon(DesktopIcon* ic) : Fl_Window(ic->x(), ic->y(), ic->w(), ic->h()), icon(ic), mask(0) {
	EASSERT(icon != NULL);

	set_override();
	color(ic->color());

	begin();
		/*
		 * Force box be same width/height as icon so it
		 * can fit inside masked window.
		 */
#ifdef USE_SHAPE 
		Fl_Image* img = ic->icon_image();
		if(img)
			icon_box = new Fl_Box(0, 0, img->w(), img->h());
		else
			icon_box = new Fl_Box(0, 0, w(), h());
#else
		icon_box = new Fl_Box(0, 0, w(), h());
#endif
		icon_box->image(ic->icon_image());
	end();
}

MovableIcon::~MovableIcon() {
	if(mask)
		XFreePixmap(fl_display, mask);
}

void MovableIcon::show(void) {
	if(!shown())
		Fl_X::make_xid(this);

#ifdef USE_SHAPE
	if(icon->icon_image()) {
		mask = create_mask(icon->icon_image());
		if(mask)
			XShapeCombineMask(fl_display, fl_xid(this), ShapeBounding, 0, 0, mask, ShapeSet);
	}
#endif
}
