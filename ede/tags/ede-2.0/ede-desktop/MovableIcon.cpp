/*
 * $Id: DesktopIcon.cpp 3032 2011-08-03 12:20:37Z karijes $
 *
 * ede-desktop, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2006-2011 EDE Authors.
 *
 * This program is licensed under terms of the 
 * GNU General Public License version 2 or newer.
 * See COPYING for details.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define FL_LIBRARY 1
#include <FL/Fl.H>
#include <FL/x.H>
#include <edelib/Debug.h>

#ifdef HAVE_SHAPE
# include <X11/extensions/shape.h>
#endif

#include "MovableIcon.h"
#include "DesktopIcon.h"
#include "Utils.h"

MovableIcon::MovableIcon(DesktopIcon* ic) : Fl_Window(ic->x(), ic->y(), ic->w(), ic->h()), icon(ic), mask(0), opacity_atom(0) {
	E_ASSERT(icon != NULL);

	set_override();
	color(ic->color());

	begin();
		/* force box be same width/height as icon so it can fit inside masked window */
#ifdef HAVE_SHAPE 
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

	opacity_atom = XInternAtom(fl_display, "_NET_WM_WINDOW_OPACITY", False);
}

MovableIcon::~MovableIcon() {
	if(mask)
		XFreePixmap(fl_display, mask);
}

void MovableIcon::show(void) {
	if(!shown())
		Fl_X::make_xid(this);

#ifdef HAVE_SHAPE
	if(icon->icon_image()) {
		mask = create_mask(icon->icon_image());
		if(mask) {
			XShapeCombineMask(fl_display, fl_xid(this), ShapeBounding, 0, 0, mask, ShapeSet);

			/* now set transparency; composite manager should handle the rest (if running) */
			unsigned int opacity = 0xc0000000;
			XChangeProperty(fl_display, fl_xid(this), opacity_atom, XA_CARDINAL, 32, PropModeReplace,
					(unsigned char*)&opacity, 1L);
		}
	}
#endif
}
