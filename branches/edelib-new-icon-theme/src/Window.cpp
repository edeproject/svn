/*
 * $Id$
 *
 * Window class
 * Copyright (c) 2005-2007 edelib authors
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Tooltip.H>
#include <FL/Fl_Shared_Image.H>

#include <edelib/Window.h>
#include <edelib/Color.h>
#include <edelib/MessageBox.h>
#include <edelib/IconLoader.h>
#include <edelib/Debug.h>
#include <edelib/XSettingsClient.h>
#include <edelib/WindowXid.h>

#ifdef HAVE_LIBXPM
#include <X11/xpm.h>
#endif

#define EWINDOW 0xF3

#define DEFAULT_ICON_THEME "edeneu"
#define DEFAULT_FONT_SIZE  12

extern int FL_NORMAL_SIZE;

EDELIB_NS_BEGIN

static XSettingsClient* xcli = NULL;
static Window*          local_window  = NULL;
static char*            seen_theme = NULL;

static bool icon_theme_load_once(const char* theme) {
	if(seen_theme && strcmp(seen_theme, theme) == 0)
		return false;

	seen_theme = strdup(theme);
	E_DEBUG(E_STRLOC ": loading '%s' theme\n", theme);

	if(IconLoader::inited())
		IconLoader::reload(theme);
	else
		IconLoader::init(theme);
	return true;
}

static void send_client_message(::Window w, Atom a, unsigned int uid) {
	XEvent xev;
	long mask;

	memset(&xev, 0, sizeof(xev));
	xev.xclient.type = ClientMessage;
	xev.xclient.window = w;
	xev.xclient.message_type = a;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = uid;
	xev.xclient.data.l[1] = CurrentTime;
	mask = 0L;

	if(w == RootWindow(fl_display, fl_screen))
		mask = SubstructureRedirectMask;
	XSendEvent(fl_display, w, False, mask, &xev);
}

/*
 * FIXME: make sure this function route events to window; e.g. ClientMessage
 * so window can install handler and re-read ClientMessage again
 */
static int xevent_handler(int id) {
	if(fl_xevent->type == ClientMessage) {
		Fl_Window* win = 0;
		Atom message = fl_xevent->xclient.message_type;

		/*
		 * All windows known to FLTK must be scanned and global variable must NOT be used
		 * (sadly add_handler() don't have additional parameter that can be passed) due
		 * multiple Window objects in the same app. If global variable is used, callbacks
		 * will be messed up and only last Window object created will accept them.
		 *
		 * There are two ways to resolve this:
		 * - (used here) is to relay on FLTK's RTTI and scan window list for matched uid;
		 *   cons: every inherited window _must not_ decrease type() variable, only can increase it
		 *
		 * - relay on xclient.window member, but all X tree must be scanned for _CHANGE_SETTINGS property
		 *   and send it to those
		 *   cons: if there are large number of windows, this can be slow
		 *   pros: much more robust
		 */
		for(win = Fl::first_window(); win; win = Fl::next_window(win)) {
			if(win->type() >= EWINDOW) {
				edelib::Window* ewin = (edelib::Window*)win;
				if(message == ewin->preferences_atom() && 
						(unsigned int)fl_xevent->xclient.data.l[0] == ewin->settings_uid()) {
					(*ewin->settings_callback())(ewin->settings_callback_data());
				}
			}
		}
	}

	E_RETURN_VAL_IF_FAIL(xcli != NULL, 0);
	return xcli->process_xevent(fl_xevent);
}

static void xsettings_cb(const char* name, XSettingsAction action, XSettingsSetting* setting, void* data) {
	bool changed = false;
	Window* win = (Window*)data;

	if(strcmp(name, "Fltk/Background") == 0) {
		unsigned char r, g, b;

		if(action == XSETTINGS_ACTION_DELETED || setting->type != XSETTINGS_TYPE_COLOR) {
			// FL_GRAY
			r = g = b = 192;
		} else {
			r = setting->data.v_color.red;
			g = setting->data.v_color.green;
			b = setting->data.v_color.blue;
		}

		Fl::background(r, g, b);
		changed = true;
	} else if(strcmp(name, "Fltk/Background2") == 0) {
		unsigned char r, g, b;

		if(action == XSETTINGS_ACTION_DELETED || setting->type != XSETTINGS_TYPE_COLOR) {
			// FL_WHITE
			r = g = b = 255;
		} else {
			r = setting->data.v_color.red;
			g = setting->data.v_color.green;
			b = setting->data.v_color.blue;
		}

		Fl::background2(r, g, b);
		changed = true;
	} else if(strcmp(name, "Fltk/Foreground") == 0) {
		unsigned char r, g, b;

		if(action == XSETTINGS_ACTION_DELETED || setting->type != XSETTINGS_TYPE_COLOR) {
			// FL_BLACK
			r = g = b = 0;
		} else {
			r = setting->data.v_color.red;
			g = setting->data.v_color.green;
			b = setting->data.v_color.blue;
		}

		Fl::foreground(r, g, b);
		changed = true;
	} else if(strcmp(name, "Fltk/FontSize") == 0) {
		int normal_size;
		if(action == XSETTINGS_ACTION_DELETED || setting->type != XSETTINGS_TYPE_INT)
			normal_size = DEFAULT_FONT_SIZE;
		else
			normal_size = setting->data.v_int;
		/*
		 * A temporal hack until I figure something better.
		 *
		 * Ideal way would be to use got font size and call labelsize() 
		 * on each child inside window, but some of those childs can be 
		 * Fl_Group which does not labelsize()
		 */
		FL_NORMAL_SIZE = normal_size;
		changed = true;
	} else if(strcmp(name, "Net/IconThemeName") == 0) {
		const char* th = NULL;

		if(action == XSETTINGS_ACTION_DELETED || setting->type != XSETTINGS_TYPE_STRING)
			th = DEFAULT_ICON_THEME;
		else
			th = setting->data.v_string;

		changed = icon_theme_load_once(th);
	} else if(win->xsettings_callback()) {
		// finally pass data to the user
		changed = (*win->xsettings_callback())(name, action, setting, win->xsettings_callback_data());
	}

	if(changed) {
		win->redraw();
	}
}

Window::Window(int X, int Y, int W, int H, const char* l) : Fl_Double_Window(X, Y, W, H, l), 
	inited(false), sbuffer(false), loaded_components(0), pref_atom(0), pref_uid(0), 
	xs_cb(NULL), xs_cb_old(NULL), xs_cb_data(NULL),
	s_cb(NULL), s_cb_data(NULL), icon_pixmap(NULL) { 

	type(EWINDOW);
}

Window::Window(int W, int H, const char* l) : Fl_Double_Window(W, H, l),
	inited(false), sbuffer(false), loaded_components(0), pref_atom(0), pref_uid(0), 
	xs_cb(NULL), xs_cb_old(NULL), xs_cb_data(NULL), 
	s_cb(NULL), s_cb_data(NULL), icon_pixmap(NULL) { 

	type(EWINDOW);
}

Window::~Window() {
	IconLoader::shutdown();

	if(seen_theme)
		free(seen_theme);

	hide();
}

void Window::init(int component) {
	if(inited)
		return;

	if(component & WIN_INIT_IMAGES)
		fl_register_images();

	fl_open_display();

	if(component & WIN_INIT_ICON_THEME)
		icon_theme_load_once(DEFAULT_ICON_THEME);

	/* setup icons for dialogs */
	MessageBox::set_themed_icons(MSGBOX_ICON_INFO, 
								 MSGBOX_ICON_ERROR, 
								 MSGBOX_ICON_QUESTION, 
								 MSGBOX_ICON_QUESTION, 
								 MSGBOX_ICON_PASSWORD);

	// FIXME: uh, uh ???
	local_window = this;

	pref_atom = XInternAtom(fl_display, "_CHANGE_SETTINGS", False);

	// FIXME: what if failed ?
	if(xs.init(fl_display, fl_screen, xsettings_cb, this))
		xcli = &xs;

	/*
	 * FLTK keeps internaly list of event handlers that are shared between
	 * windows in one application. If in one app exists two or more Window objects,
	 * handler will be added again without checking if already exists. With that 
	 * behaviour callback can be called multiple times even if only one window matches it.
	 *
	 * Removing (if exists) and re-adding again will assure we have only one callback
	 * per Window object.
	 */
	Fl::remove_handler(xevent_handler);
	Fl::add_handler(xevent_handler);

	inited = true;
}

/* static */
void Window::update_settings(unsigned int uid) {
	/* 
	 * Fetch _CHANGE_SETTINGS atom (note 'True'). If noone app set
	 * this, it will fail and we will do nothing.
	 */
	Atom pref = XInternAtom(fl_display, "_CHANGE_SETTINGS", True);
	if(pref == None)
		return;

	/*
	 * Alternative version:
	 * instead recursively children scan, we will relay on server; let it
	 * pass message to the each child at the first level and send it uid. 
	 * If that uid matches child's one, callback will be done
	 */
	::Window dummy, root, *children = 0;
	unsigned int nchildren;

	root = RootWindow(fl_display, fl_screen);
	XQueryTree(fl_display, root, &dummy, &dummy, &children, &nchildren);
	if(!nchildren)
		return;

	for(unsigned int i = 0; i < nchildren; i++)
		if(children[i] != root)
			send_client_message(children[i], pref, uid);

	XFree(children);
	XFlush(fl_display);
}

XSettingsClient* Window::xsettings(void) {
	EASSERT(inited == true && "init() must be called before this function");
	return &xs;
}

static void set_titlebar_icon(Fl_Window* ww) {
#ifdef HAVE_LIBXPM
	Window* win = (Window*)ww;

	if(!win->window_icon())
		return;

	Pixmap pix, mask;
	XpmCreatePixmapFromData(fl_display, DefaultRootWindow(fl_display), (char**)win->window_icon(), &pix, &mask, NULL);

	XWMHints* hints = XGetWMHints(fl_display, fl_xid(win));
	if(!hints)
		return;

	hints->icon_pixmap = pix;
	hints->icon_mask = mask;
	hints->flags |= IconPixmapHint | IconMaskHint;

	XSetWMHints(fl_display, fl_xid(win), hints);
	XFree(hints);
#endif
}

void Window::show(void) {
	image(Fl::scheme_bg_);

	if(Fl::scheme_bg_) {
		labeltype(FL_NORMAL_LABEL);
		align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
	} else {
		labeltype(FL_NO_LABEL);
	}

	Fl_Tooltip::exit(this);

	if(!shown()) {
		// Don't set background pixel for double-buffered windows...
		int background_pixel = -1;

		if(type() == FL_WINDOW && ((box() == 1) || (box() & 2) && (box() <= 15))) {
			background_pixel = int(fl_xpixel(color()));
		}

		window_xid_create(this, set_titlebar_icon, background_pixel);
	} else
		XMapRaised(fl_display, fl_xid(this));

	if(pref_atom) {
		XChangeProperty(fl_display, fl_xid(this), 
				pref_atom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&pref_uid, 1);
	}
}

EDELIB_NS_END
