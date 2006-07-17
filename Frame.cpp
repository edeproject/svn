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

#include "Frame.h"
#include "Events.h"
#include "Hints.h"
#include "Windowmanager.h"
#include "Atoms.h"
#include "Utils.h"
#include "Titlebar.h"
#include "debug.h"
#include "Tracers.h"

#include <assert.h>

#ifdef SHAPE
#include <X11/extensions/shape.h>
#endif // SHAPE


#define FRAME_NAME(frame) (frame->label ? frame->label : "NULL")

// TODO: just for testing
#define BORDER_UPDOWN 3
#define BORDER_LEFTRIGHT 3

#define BORDER_THIN_UPDOWN 1
#define BORDER_THIN_LEFTRIGHT 1

#define SIZER_W 15
#define SIZER_H 15

/*
#define MIN_W 300
#define MIN_H 255
*/

#define MIN_W 0
#define MIN_H 0

#define TITLEBAR_H 20

#define EDGE_SNAP 10

void FrameBorders::border_color(Fl_Color c, FrameBordersState s)
{
	switch(s)
	{
		case FOCUSED:
			focused = c;
			break;
		case UNFOCUSED:
			unfocused = c;
			break;
		case CLICKED:
			clicked = c;
			break;
	}
}

Fl_Color FrameBorders::border_color(FrameBordersState s)
{
	switch(s)
	{
		case FOCUSED:
			return focused;
		case UNFOCUSED:
			return unfocused;
		case CLICKED:
			return clicked;
	}
	return FL_CYAN;	// XXX
}


void FrameBorders::sizers_color(Fl_Color c, FrameBordersState s)
{
	switch(s)
	{
		case FOCUSED:
			sizers_focused = c;
			break;
		case UNFOCUSED:
			sizers_unfocused = c;
			break;
		case CLICKED:
			sizers_clicked = c;
			break;
	}
}

Fl_Color FrameBorders::sizers_color(FrameBordersState s)
{
	switch(s)
	{
		case FOCUSED:
			return sizers_focused;
		case UNFOCUSED:
			return sizers_unfocused;
		case CLICKED:
			return sizers_clicked;
	}
	return FL_CYAN; // XXX
}


void FrameBorders::updown(int s)
{
	updown_w = s;
	updown2x_w = s * 2;
}

void FrameBorders::leftright(int s)
{
	leftright_h = s;
	leftright2x_h = s * 2;
}

void FrameBorders::shaped(bool s)
{
	is_shaped = s;
}

Frame::Frame(Window win, XWindowAttributes* attrs) : 
	Fl_Window(0, 0), 
	fdata(new FrameData),
	screen_x(0),
	screen_y(0),
	screen_w(0),
	screen_h(0),
	restore_x(0),
	restore_y(0),
	restore_w(0),
	restore_h(0),
	opaque_move_resize(false),
	show_titlebar(true),
	is_moving(false),
	is_resizing(false),
	cursor_grabbed(false),
	snap_move(false)
{
	// register our events
	events = new FrameEventHandler(this);

	fdata->window        = win;
	fdata->transient_win = None;
	fdata->colormap      = DefaultColormap(fl_display, fl_screen);
	fdata->option        = 0;
	fdata->type          = FrameTypeNormal;
	fdata->state         = 0;
	fdata->win_gravity   = 0;
	fdata->autoplace     = true;
	fdata->icon_pixmap   = 0;
	fdata->icon_mask     = 0;
	fdata->label         = 0;
	fdata->label_alocated = false;

	fdata->plain.x  = fdata->plain.y = 0;
	fdata->plain.w  = fdata->plain.h = 0;
	fdata->plain.inc_w = fdata->plain.inc_h = 0;
	fdata->plain.max_w = fdata->plain.max_h = 0;
	fdata->plain.min_w = fdata->plain.min_h = 0;

	borders.border_color(fl_darker(FL_GRAY), FOCUSED);
	borders.border_color(FL_WHITE, UNFOCUSED);
	borders.border_color(FL_BLUE, CLICKED);

	borders.sizers_color(FL_GRAY, FOCUSED);
	borders.sizers_color(FL_RED, UNFOCUSED);
	borders.sizers_color(FL_RED, CLICKED);
	// recalculated in setup_borders
	borders.updown(0);
	borders.leftright(0);
	borders.shaped(false);

	// we does not use window specific borders
	fdata->plain.border = 0;

	// small check if we are in X session
	if(attrs)
		set_option(FrameOptIgnoreUnmap);

	// collect needed data
	feed_data(attrs);
		
	// do this asap so we don't miss any events...
	CHECK_RET1(XSelectInput(fl_display, fdata->window, 
			VisibilityChangeMask | ColormapChangeMask | 
			PropertyChangeMask | FocusChangeMask /*| StructureNotifyMask*/), BadWindow);

	CHECK_RET2(XGetTransientForHint(fl_display, fdata->window, &fdata->transient_win), BadWindow, BadAlloc);
	if(fdata->transient_win != None)
	{
		fdata->type = FrameTypeDialog;
		ELOG("Got transient_win");
	}

	init_sizes();
	setup_borders();

	CHECK_RET3(XMoveResizeWindow(fl_display, fdata->window, fdata->plain.x, fdata->plain.y,
			fdata->plain.w, fdata->plain.h), BadWindow, BadValue, BadMatch);

	begin();
	Fl_Color szc = borders.sizers_color(FOCUSED);
	sizer_top_left = new Fl_Box(1,1,SIZER_W,SIZER_H);
	sizer_top_left->box(FL_FLAT_BOX);
	sizer_top_left->color(szc);

	sizer_top_right = new Fl_Box(1,1,SIZER_W,SIZER_H);
	sizer_top_right->box(FL_FLAT_BOX);
	sizer_top_right->color(szc);

	sizer_bottom_left = new Fl_Box(1,1,SIZER_W,SIZER_H);
	sizer_bottom_left->box(FL_FLAT_BOX);
	sizer_bottom_left->color(szc);

	sizer_bottom_right = new Fl_Box(1,1,SIZER_W,SIZER_H);
	sizer_bottom_right->box(FL_FLAT_BOX);
	sizer_bottom_right->color(szc);


	if(show_titlebar)
	{
		titlebar = new Titlebar(this, borders.leftright(), borders.updown(),
				w() - borders.leftright2x(), TITLEBAR_H, "Untitled");

		if(fdata->label)
			titlebar->label(fdata->label);
	}		

	end();

	/* Reposition and resize main frame
	 * but skip it if position is already
	 * set by some hints or fdata->autoplace is false.
	 *
	 * NOTE: efltk set minimal position 2x2
	 * so this is check. If window is initialy
	 * tried to be placed on 0x0 it will be 
	 * misteriously hidden.
	 */
	int pos_x;
	int pos_y;
	if(fdata->autoplace && WindowManager::instance()->query_best_position(&pos_x, &pos_y, w(), h()))
	{
		if(pos_x <= 2)
			pos_x = x();
		if(pos_y <= 2)
			pos_y = y();

		ELOG("This window does use autoplace");
	}
	/*
	else
	{
		pos_x = x();
		pos_y = y();
	}
	*/
	else
	{
		// recorrect positions made up in setup_borders() ?@#@! including
		// titlebar if present
		if((x() - borders.leftright()) > screen_x)
			pos_x = x() - borders.leftright();
		else
			pos_x = x();

		if(show_titlebar && (y() - titlebar->h() - borders.updown()) > screen_y)
			pos_y = y() - titlebar->h() - borders.updown();
		else if((y() - borders.updown()) > screen_y)
			pos_y = y() - borders.updown();
		else
			pos_y = y();
	}


	if(show_titlebar)
		resize(pos_x, pos_y, w(), h() + titlebar->h());
	else
		resize(pos_x, pos_y, w(), h());

	// only normal windows can be resized
	if(fdata->type == FrameTypeNormal)
		place_sizers(x(), y(), w(), h());

	set_override();
	// color of main window background, aka borders
	color(borders.border_color(FOCUSED));
	create();
	reparent_window();
	configure_notify();

	// rest is in content_click()
	CHECK_RET3(XGrabButton(fl_display, AnyButton, AnyModifier, fdata->window, 
			False, ButtonPressMask, GrabModeSync, GrabModeAsync, None, None),
			BadCursor, BadValue, BadWindow);

	// show our creation
	CHECK_RET1(XMapWindow(fl_display, fdata->window), BadWindow);
	show();
	
	if(borders.shaped())
		shape_borders();

	//shape_edges();

	CHECK_RET3(XAddToSaveSet(fl_display, fdata->window), BadWindow, BadMatch, BadValue);

	// TODO: this handling should be in WindowManager
	WindowManager::instance()->window_list.push_back(this);

	if(fdata->transient_win != None)
	{
		ELOG("Added to aot_list");
		// latest mapped windows will be _before_ others iff they are transient
		WindowManager::instance()->aot_list.push_front(this);
	}
	else
	{
		ELOG("Added to stack_list");
		WindowManager::instance()->stack_list.push_front(this);
	}

	if(fdata->option & FrameOptTakeFocus)
		focus();

	XFlush(fl_display);
}

Frame::~Frame()
{
	ELOG("Frame::~Frame");
	if(fdata->label_alocated)
		free(fdata->label);

	delete fdata;
	delete events;
}

void Frame::feed_data(XWindowAttributes* existing)
{
	WindowManager* wm = WindowManager::instance();
	
	screen_x = wm->x();
	screen_y = wm->y();
	screen_w = wm->w();
	screen_h = wm->h();

	wm->hints()->icccm_size(fdata);

	// TODO: For testing only. Better solution will be.
	fdata->type = wm->hints()->netwm_window_type(fdata);
	setup_borders();
	if(fdata->type == FrameTypeSplash || fdata->type == FrameTypeMenu || fdata->type == FrameTypeDesktop ||
			fdata->type == FrameTypeDock)
		show_titlebar = false;
	// ------------------------------------------------

	wm->hints()->netwm_window_state(fdata);
	wm->hints()->mwm_load_hints(fdata);
	
	load_wm_hints();
	load_colormap();
	load_label();

	XWindowAttributes attrs;
	if(existing)
		attrs = *existing;
	else
		XGetWindowAttributes(fl_display, fdata->window, &attrs);

	// according to ICCCM standard, returned values must
	// be used for window only, excluding decorations
	fdata->plain.x = attrs.x;
	fdata->plain.y = attrs.y;
	// TODO: add checking if window out of screen
	fdata->plain.w = attrs.width;
	fdata->plain.h = attrs.height;

	ELOG("XWindowAttributes: %i %i %i %i", fdata->plain.x, fdata->plain.y, fdata->plain.w, fdata->plain.h);

	// Shorthand for: fdata->colormap = attrs->colormap;
	// ...for easier tracking
	if(fdata->colormap != attrs.colormap)
		load_colormap(attrs.colormap);	

	switch(attrs.map_state)
	{
		case IsUnmapped:
			set_state(FrameStateIconized);
			break;
		case IsViewable:
			set_state(FrameStateNormal);
			break;
		case IsUnviewable:
			ELOG("Got IsUnviewable map_state, skiping for now...");
			break;
	}
}

/* load WM_HINTS property, finding if window
 * have icons and in what state is in
 * TODO: place this as Hint::icccm_load_wm_hints()
 */
void Frame::load_wm_hints(void)
{
	XWMHints* wm_hints = XAllocWMHints();
	wm_hints = XGetWMHints(fl_display, fdata->window);
	if(!wm_hints)
	{
		ELOG("XGetWMHints failed!");
		return;
	}

	if((wm_hints->flags & IconPixmapHint) && wm_hints->icon_pixmap)
		fdata->icon_pixmap = wm_hints->icon_pixmap;
	if((wm_hints->flags & IconMaskHint) &&wm_hints->icon_mask)
		fdata->icon_mask   = wm_hints->icon_mask;

	switch(wm_hints->initial_state)
	{
		case WithdrawnState:
			CHECK_RET3(XRemoveFromSaveSet(fl_display, fdata->window), BadWindow, BadMatch, BadValue);
			break;
		case IconicState:
			fdata->state = FrameStateIconized;
			break;
		case NormalState:
		default:
			fdata->state = FrameStateNormal;
			break;
	}

	// check for focus
	if((wm_hints->flags & InputHint) && !wm_hints->input)
			fdata->option &= ~FrameOptTakeFocus; // window does not want focus
	else
			fdata->option |= FrameOptTakeFocus;  // window want focus;

	XFree(wm_hints);
}

// guess title
void Frame::load_label(void)
{
	TRACE_FUNCTION("void Frame::load_label(void)");
	WindowManager* wm = WindowManager::instance();

	// first try NETWM style
	fdata->label = wm->hints()->netwm_label(fdata->window, &fdata->label_alocated);
	if(!fdata->label)
	{
		// then ICCCM
		fdata->label = wm->hints()->icccm_label(fdata->window, &fdata->label_alocated);
	}
	ELOG("Window: %s (%p) loaded", (fdata->label ? fdata->label : "NULL"), fdata->window);
}

void Frame::destroy(void)
{
	TRACE_FUNCTION("void Frame::destroy(void)");

	ELOG("window goes down");
	if(state(FrameStateDestroyed))
		return;

	if(shown())
	{
		CHECK_RET1(XUnmapWindow(fl_display, fl_xid(this)), BadWindow);
		CHECK_RET1(XUnmapWindow(fl_display, fdata->window), BadWindow);
	}

	CHECK_RET3(XRemoveFromSaveSet(fl_display, fdata->window), BadWindow, BadMatch, BadValue);
	set_state(FrameStateDestroyed);
	WindowManager::instance()->update_client_list();
	Fl::awake();
}

// Install custom or default colormap.
// Default colormap is read only once, in Frame constructor.
void Frame::load_colormap(Colormap col)
{
	TRACE_FUNCTION("void Frame::load_colormap(Colormap col)");

	if(col != 0)
	{
		ELOG("Installing custom colormap");
		fdata->colormap = col;
	}
	else
		ELOG("Loading default colormap");

	CHECK_RET2(XInstallColormap(fl_display, fdata->colormap), BadWindow, BadColor);
}

// Recalculate framed based on plain.
// If window is outside screen, fix what needs to be fixed
void Frame::setup_borders(void)
{
	TRACE_FUNCTION("void Frame::setup_borders(void)");

	int tx, ty;
	switch(fdata->type)
	{
		case FrameTypeNormal:
			/* border = BORDER_NORMAL; */
			borders.leftright(BORDER_LEFTRIGHT);
			borders.updown(BORDER_UPDOWN);
			break;
		case FrameTypeDialog:
			/*border = BORDER_THIN; */
			borders.leftright(BORDER_THIN_LEFTRIGHT);
			borders.updown(BORDER_THIN_UPDOWN);
			break;
		case FrameTypeSplash:
		case FrameTypeDesktop:
		case FrameTypeMenu:
		case FrameTypeDock:
			borders.leftright(0);
			borders.updown(0);
			// they don't have visible borders
			break;
	}

	// TODO: setting up initial window sizes
	// should not be in setup_borders();
	tx = fdata->plain.x - borders.leftright();
	ty = fdata->plain.y - borders.updown();
	if(tx < 0)
		fdata->plain.x = borders.leftright();
	if(ty < 0)
		fdata->plain.y = borders.updown();
	x(fdata->plain.x);
	y(fdata->plain.y); 

	w(fdata->plain.w + borders.leftright2x());
	h(fdata->plain.h + borders.updown2x());

	// set fdata->window borders, althought fdata->plain.borders is
	// always 0
	CHECK_RET3(XSetWindowBorderWidth(fl_display, fdata->window, fdata->plain.border), 
			BadWindow, BadValue, BadMatch);
}

/* Setup window sizes to minimal usable (MIN_W, MIN_H)
 * but only for normal windows. Transient should set
 * their size internally.
 */
void Frame::init_sizes(void)
{
	if(fdata->type == FrameTypeDialog)
		return;

	if(fdata->plain.w < MIN_W)
		fdata->plain.w = MIN_W;
	if(fdata->plain.h < MIN_H)
		fdata->plain.h = MIN_H;
}

void Frame::reparent_window(void)
{
	TRACE_FUNCTION("void Frame::reparent_window(void)");

	const int XEventMask = 
		ExposureMask | 
		StructureNotifyMask|
		VisibilityChangeMask |
		KeyPressMask|
		KeyReleaseMask|
		KeymapStateMask|
		FocusChangeMask | 
		ButtonPressMask | 
		ButtonReleaseMask | 
		EnterWindowMask | 
		LeaveWindowMask | 
		PointerMotionMask | 
		SubstructureRedirectMask | 
		SubstructureNotifyMask;


	XSetWindowAttributes sattr;
	// TODO: some windows set bit_gravity too
	sattr.bit_gravity = NorthWestGravity;
	sattr.event_mask = XEventMask;
	sattr.colormap = fl_colormap;
	sattr.border_pixel = fl_xpixel(FL_BLACK);
	sattr.override_redirect = 0;
	sattr.background_pixel = fl_xpixel(FL_GRAY);
	CHECK_RET3(XChangeWindowAttributes(fl_display, fl_xid(this), 
			CWBitGravity | CWBorderPixel | CWColormap | CWEventMask | CWBackPixel | CWOverrideRedirect, &sattr),
			BadWindow, BadAccess, BadMatch);

	if(!show_titlebar)
	{
		CHECK_RET1(XReparentWindow(fl_display, fdata->window, fl_xid(this), 
			borders.leftright(), borders.updown()), BadWindow);
	}
	else
	{
		CHECK_RET1(XReparentWindow(fl_display, fdata->window, fl_xid(this), 
			borders.leftright(), borders.updown() + titlebar->h()), BadWindow);
	}

}

void Frame::recalc_geometry(int x_pos, int y_pos, int w_sz, int h_sz, short rtype)
{
	TRACE_FUNCTION("void Frame::recalc_geometry(int x_pos, int y_pos, int w_sz, int h_sz, short rtype");

	if(rtype == GeometryRecalcAll)
	{
		x(x_pos); y(y_pos); 
		w(w_sz); h(h_sz);

		fdata->plain.x = x() + borders.leftright();
		fdata->plain.y = y() + borders.updown();
		fdata->plain.w = w() - borders.leftright2x();
	}
	else if(rtype == GeometryRecalcAllXY)
	{
		// do not include fdata->plain.w
		// FIXME: duplication as above
		x(x_pos); y(y_pos);
		w(w_sz); h(h_sz);
		fdata->plain.x = x() + borders.leftright();
		fdata->plain.y = y() + borders.updown();
	}
	else if(rtype == GeometryRecalcPlain)
	{
		fdata->plain.x = x_pos;
		fdata->plain.y = y_pos;
		fdata->plain.w = w_sz;
		fdata->plain.h = h_sz;

		x(x_pos - borders.leftright());
		y(y_pos - borders.updown());
		w(w_sz  + borders.leftright2x());
	}
	else
	{
		// we should no be here
		assert(0);
	}


	if(show_titlebar)
	{
		// titlebar->resize(border, border, w() - border2x, titlebar->h());
		if(rtype == GeometryRecalcAll)
		{
			fdata->plain.h = h() - (titlebar->h() + borders.updown2x());
		}
		else if(rtype == GeometryRecalcPlain)
		{
			h(h_sz + titlebar->h() + borders.updown2x());
		}
			
		titlebar->size(w() - borders.leftright2x(), titlebar->h());
	}
	else
	{
		if(rtype == GeometryRecalcAll)
		{
			fdata->plain.h = h() - borders.updown2x();
		}
		else if(rtype == GeometryRecalcPlain)
		{
			h(h_sz + borders.updown2x());
		}
	}

		ELOG("p: %i %i %i %i w: %i %i %i %i",
			fdata->plain.x, fdata->plain.y, fdata->plain.w, fdata->plain.h,
			x(), y(), w(), h());
}

/* Set size of window, accepting coordinates for fdata->window + frame,
 * int which case it will be resized both.
 */
void Frame::set_size(int x_pos, int y_pos, int w_sz, int h_sz, bool apply_on_plain)
{
	TRACE_FUNCTION("void Frame::set_size(int x_pos, int y_pos, int w_sz, int h_sz, bool apply_on_plain)");

	short how;
	if(apply_on_plain)
		how = GeometryRecalcPlain;
	else
		how = GeometryRecalcAll;

	recalc_geometry(x_pos, y_pos, w_sz, h_sz, how);

	XGrabServer(fl_display);

	place_sizers(x(), y(), w(), h());
	CHECK_RET3(XMoveWindow(fl_display, fl_xid(this), x(), y()), BadWindow, BadMatch, BadValue);
	CHECK_RET3(XResizeWindow(fl_display, fl_xid(this), w(), h()), BadWindow, BadMatch, BadValue);
	CHECK_RET3(XResizeWindow(fl_display, fdata->window, fdata->plain.w, fdata->plain.h), BadWindow, BadMatch, BadValue);

	if(borders.shaped())
		shape_borders();

	XUngrabServer(fl_display);

	configure_notify();
	redraw();
}

void Frame::move_window(int x_pos, int y_pos)
{
	TRACE_FUNCTION("void Frame::move_window(int x_pos, int y_pos)");

	/*
	draw_overlay(x_pos, y_pos, w(), h());
	draw_overlay(x_pos, y_pos, w(), h());
	return;
	*/

	// very dummy snapping with screen edges
	// TODO: what about other window(s) edges ?
	if(snap_move)
	{
		int snap_x = screen_x + EDGE_SNAP;
		int snap_y = snap_x;
		int snap_w = screen_w - EDGE_SNAP;
		int snap_h = screen_h - EDGE_SNAP;
		int w_pos = x_pos + w();
		int h_pos = y_pos + h();

		if(snap_x > x_pos - EDGE_SNAP && snap_x < x_pos + EDGE_SNAP)
			x_pos = screen_x;
		if(snap_y > y_pos - EDGE_SNAP && snap_y < y_pos + EDGE_SNAP)
			y_pos = screen_y;
		
		if(snap_w > w_pos - EDGE_SNAP && snap_w < w_pos + EDGE_SNAP)
			x_pos = screen_w - w();
		if(snap_h > h_pos - EDGE_SNAP && snap_h < h_pos + EDGE_SNAP)
			y_pos = screen_h - h();
	}

	recalc_geometry(x_pos, y_pos, w(), h(), GeometryRecalcAllXY);
	CHECK_RET3(XMoveWindow(fl_display, fl_xid(this), x(), y()), BadMatch, BadWindow, BadValue);
	configure_notify();
}

void Frame::draw_overlay(int x, int y, int w, int h)
{
	TRACE_FUNCTION("void Frame::draw_overlay(int x, int y, int w, int h)");
	XGCValues v;
	v.subwindow_mode = IncludeInferiors;
	v.foreground = 0xffffffff;
	v.function = GXxor;
	v.line_width = borders.updown();
	v.graphics_exposures = False;
	int mask = GCForeground|GCSubwindowMode|GCFunction|GCLineWidth|GCGraphicsExposures;
	GC invertGc = XCreateGC(fl_display, RootWindow(fl_display, fl_screen), mask, &v);

	if (w < 0) {x += w; w = -w;}
	else if (!w) w = 1;
	if (h < 0) {y += h; h = -h;}
	else if (!h) h = 1;

	XDrawRectangle(fl_display, RootWindow(fl_display, fl_screen), invertGc, x, y, w, h); 
}

void Frame::place_sizers(int x, int y, int w, int h)
{
	TRACE_FUNCTION("void Frame::place_sizers(int x, int y, int w, int h)");

	// using position() instead resize() will be only
	// valid if sizers are initialy set to SIZER_W and SIZER_H

	sizer_top_left->position(0, 0);
	sizer_top_right->position(w - SIZER_W, 0);
	sizer_bottom_left->position(0, h - SIZER_H);
	sizer_bottom_right->position(w - SIZER_W, h - SIZER_H);
}

/* Determine type of resizing based on position
 * (NOTE: here is used relative position - Fl::event_x() and Fl::event_y(),
 * which are valid only in frame window).
 *
 * TODO: any way this can be simplified ?
 */
long Frame::resize_type(int x, int y)
{
	// TRACE_FUNCTION("long Frame::resize_type(int x, int y)");

	//ELOG("%i %i", x, y);
	// we are in top left sizer
	if((x >= sizer_top_left->x() && x <= sizer_top_left->w()) && 
			(y >= sizer_top_left->y() && y <= sizer_top_left->h()))
		return ResizeTypeUpLeft;

	// we are in top right sizer
	if((x >= sizer_top_right->x() && x <= sizer_top_right->x() + sizer_top_right->w()) && 
			(y >= sizer_top_right->y() && y <= sizer_top_right->y() + sizer_top_right->h()))
		return ResizeTypeUpRight;

	// we are in bottom left sizer
	if((x >= sizer_bottom_left->x() && x <= sizer_bottom_left->x() + sizer_bottom_left->w()) &&
			(y >= sizer_bottom_left->y() && y <= sizer_bottom_left->y() + sizer_bottom_left->h()))
		return ResizeTypeDownLeft;

	// we are in bottom right sizer
	if((x >= sizer_bottom_right->x() && x <= sizer_bottom_right->x() + sizer_bottom_right->w()) && 
			y >= sizer_bottom_right->y() && y <= sizer_bottom_right->y() + sizer_bottom_right->h())
		return ResizeTypeDownRight;

	// we are in top border
	if((x > sizer_top_left->x() + sizer_top_left->w() && x < sizer_top_right->x()) &&
			(y < borders.updown()))
		return ResizeTypeUp;

	// we are in left border
	if((x < borders.leftright()) && (y > sizer_top_left->y() + sizer_top_left->h()) && 
			y < sizer_bottom_left->y())
		return ResizeTypeLeft;

	// we are in right border
	if((x < sizer_top_right->x() + sizer_top_right->w()) && 
			(x > sizer_top_right->x() + sizer_top_right->w() - borders.leftright()) &&
			(y > sizer_top_right->y() + sizer_top_right->w()) &&
			(y < sizer_bottom_left->y()))
		return ResizeTypeRight;

	// we are in bottom border
	if((x > sizer_bottom_left->x() + sizer_bottom_left->w()) &&
			(x < sizer_bottom_right->x()) &&
			(y > sizer_bottom_left->y() + sizer_bottom_left->h() - borders.updown()) &&
			(y < sizer_bottom_left->y() + sizer_bottom_left->h()))
		return ResizeTypeDown;


	return ResizeTypeNone;
}

/* unscramble direction, and check what combination it have
 *
 * TODO: direction should not have oposite sides included
 * like ResizeTypeRight | ResizeTypeLeft
 */
void Frame::resize_window(int mouse_x, int mouse_y, long direction)
{
	TRACE_FUNCTION("void Frame::resize_window(int mouse_x, int mouse_y, long direction)");

	if(direction == 0)
		EFATAL("calling resize on possible unresizable window");

	if(direction & ResizeTypeNone)
		return;

#warning "Add increment/decrement info from icccm"

	// use collected increment/decrement info from icccm
	/*int inc_w = fdata->plain.inc_w;
	int inc_h = fdata->plain.inc_h; */

	int tw = 0, th = 0;

	if(direction & ResizeTypeLeft)
	{
		if(mouse_x > x())
			tw = w() - (mouse_x - x()); // decrease
		else 
			tw = w() + (x() - mouse_x); // increase

		set_size(mouse_x, y(), tw, h(), false);
	}

	if(direction & ResizeTypeRight)
	{
		tw = mouse_x - x();
		set_size(x(), y(), tw, h(), false);
	}

	if(direction & ResizeTypeUp)
	{
		if(mouse_y > y())
			th = h() - (mouse_y - y()); //decrease
		else
			th = h() + (y() - mouse_y); //increase

		set_size(x(), mouse_y, w(), th, false);
	}

	if(direction & ResizeTypeDown)
	{
		th = mouse_y - y();
		set_size(x(), y(), w(), th, false);
	}

}

void Frame::configure_notify(void)
{
	TRACE_FUNCTION("void Frame::configure_notify(void)");

	WindowManager::instance()->hints()->icccm_configure(fdata);
}

// apply shape on borders, based on shape mask
void Frame::shape_borders(void)
{
	TRACE_FUNCTION("void Frame::shape_borders(void)");

#ifdef SHAPE
	Window shape = XCreateSimpleWindow(fl_display, RootWindow(fl_display, fl_screen),
			x(), y(), w(), h(), 0, 0, 0);

    XRectangle rect[4];
	// top
	rect[0].x = SIZER_W;
	rect[0].y = 0;
	rect[0].width = w() - SIZER_W * 2;
	rect[0].height = borders.updown();

	// right
	rect[1].x = w() - borders.leftright();
	rect[1].y = SIZER_H;
	rect[1].width = borders.leftright();
	rect[1].height = h() - SIZER_H * 2;

	// bottom
	rect[2].x = SIZER_W;
	rect[2].y = h() - borders.updown();
	rect[2].width = w() - SIZER_W * 2;
	rect[2].height = borders.updown();

	// left
	rect[3].x = 0;
	rect[3].y = SIZER_H;
	rect[3].width = borders.leftright();
	rect[3].height = h() - SIZER_H * 2;

	// modify our shape window
	XShapeCombineRectangles(fl_display, shape, ShapeBounding, 0, 0, rect, 4, ShapeSubtract, Unsorted);
	// apply our shape window as mask
	XShapeCombineShape(fl_display, fl_xid(this), ShapeBounding, 0, 0, shape, ShapeBounding, ShapeSet);
	XDestroyWindow(fl_display, shape);
#endif
}

// apply shape on edges, based on pixmap mask
#include "mask.xpm"
#include <efltk/Fl_Image.h>
void Frame::shape_edges(void)
{
	TRACE_FUNCTION("void Frame::shape_edges(void)");
#if 0
	// TODO: finish with other three edges
	Fl_Image img((const char**)mask_xpm);
	Pixmap mask = img.create_mask(img.width(), img.height());

	Window shape = XCreateSimpleWindow(fl_display, RootWindow(fl_display, fl_screen),
			x(), y(), w(), h(), 0, 0, 0);

	XShapeCombineMask(fl_display, shape, ShapeBounding, 0, 0, mask, ShapeSubtract);
	XShapeCombineShape(fl_display, fl_xid(this), ShapeBounding, 0, 0, shape, ShapeBounding, ShapeSet);
	XDestroyWindow(fl_display, shape);
#endif
}

void Frame::maximize(void)
{
	TRACE_FUNCTION("void Frame::maximize(void)");

	if(state(FrameStateShaded))
	{
		ELOG("Maximizing shaded windows is bugy as devil himself !");
		return;
	}

	if(fdata->type == FrameTypeDialog)
	{
		ELOG("Dialogs should not be resized !");
		return;
	}

	if(state(FrameStateMaximized))
	{
		ELOG("Window is alread maximized");
		return;
	}

	restore_x = x();
	restore_y = y();
	restore_w = w();
	restore_h = h();

	set_size(screen_x, screen_y, screen_w, screen_h, false);
	set_state(FrameStateMaximized);
}

void Frame::restore(void)
{
	TRACE_FUNCTION("void Frame::restore(void)");

	if(!state(FrameStateMaximized))
	{
		ELOG("Restore unmaximized window ???");
		return;
	}

	set_size(restore_x, restore_y, restore_w, restore_h, false);
	clear_state(FrameStateMaximized);
}

/* First will inspect does window participate for WM_DELETE_WINDOW
 * message, and if not, it will be killed. That is life...
 */
void Frame::close_kill(void)
{
	TRACE_FUNCTION("void Frame::close_kill(void)");

	Atom* protocols;
	int n;
	bool have_close = false;

	if(XGetWMProtocols(fl_display, fdata->window, &protocols, &n))
	{
		for(int i = 0; i < n; i++)
		{
			if(protocols[i] == _XA_WM_DELETE_WINDOW)
				have_close = true;
		}

		XFree(protocols);
	}
	if(have_close)
		SendMessage(fdata->window, _XA_WM_PROTOCOLS, _XA_WM_DELETE_WINDOW);
	else
	{
		ELOG("Frame killed");
		XKillClient(fl_display, fdata->window);
	}
}

// window accepts input
void Frame::focus(void)
{
	TRACE_FUNCTION("void Frame::focus(void)");

	if(state(FrameStateFocused))
		return;

	WindowManager::instance()->clear_focus_windows();

	borders_color(FOCUSED);
	if(show_titlebar)
		titlebar->focus();

	CHECK_RET3(XSetInputFocus(fl_display, fdata->window, RevertToPointerRoot, fl_event_time),
			BadMatch, BadValue, BadWindow);

	XInstallColormap(fl_display, fdata->colormap);

	/* DO NOT use this !
	 * SendMessage(fdata->window, _XA_WM_PROTOCOLS, _XA_WM_TAKE_FOCUS);
	 */

	WindowManager::instance()->hints()->netwm_set_active_window(fdata->window);
	set_state(FrameStateFocused);
}

void Frame::unfocus(void)
{
	if(!state(FrameStateFocused))
		return;

	TRACE_FUNCTION("void Frame::unfocus(void)");

	borders_color(UNFOCUSED);
	if(show_titlebar)
		titlebar->unfocus();

	clear_state(FrameStateFocused);
}

void Frame::raise(void)
{
	TRACE_FUNCTION("void Frame::raise(void)");
	WindowManager* wm = WindowManager::instance();

	if(wm->stack_list.size() <= 1)
	{
		ELOG("Only one window, restacking skipped");
		focus();
		return;
	}

	FrameList::iterator it    = wm->stack_list.begin();
	FrameList::iterator last  = wm->stack_list.end();

	while(it != last)
	{
		if(*it == this)
		{
			wm->stack_list.erase(it);
			wm->stack_list.push_front(*it);
		}

		++it;
	}
	wm->restack_windows();
	focus();
}

/* Send frame one stack position below.
 * Note that as in raise(), aot_list is not checked
 * since those windows are _always_ on top. This will
 * rearange stack_list and call restack_windows(), as you guess. 
 *
 * Yes, this can be avoided using XWindowChanges with 
 * x.sibling and x.stack_mode = Below, which is much faster
 * but we will lose arangements in stack_list and posibility
 * to update _NET_CLIENT_LIST_STACKING atom. So stick with this.
 *
 * PS: if you have idea how to combine XWindowChanges with X calls
 * without needs to update stack_list, but to extract full stacking
 * list for _NET_CLIENT_LIST_STACKING, let me know.
 */
void Frame::lower(void)
{
	TRACE_FUNCTION("void Frame::lower(void)");

	WindowManager* wm = WindowManager::instance();

	if(wm->stack_list.size() <= 1)
	{
		ELOG("Only one window, restacking skipped");
		return;
	}

	FrameList::iterator top = wm->stack_list.begin();

	/* We are not at the top.
	 * This will prevent rise-lower-rise effect when
	 * function is called multiple times.
	 */
	if(this != *top)
		return;

	FrameList::iterator below = top;
	++below;

	if(*below)
	{
		std::swap(*top, *below);
	}

	wm->restack_windows();
	focus();
}

/* When we want to shade window, child (fdata->window) must
 * completely disapear (as should it be). Funny thing is 
 * if few pixels child is visible, some programs-windows
 * will crash (like mrxvt).
 */
void Frame::shade(void)
{
	TRACE_FUNCTION("void Frame::shade(void)");

	if(state(FrameStateShaded))
		return;

	restore_x = x();
	restore_y = y();
	restore_w = w();
	restore_h = h();

	CHECK_RET3(XResizeWindow(fl_display, fl_xid(this), w(), titlebar->h() + borders.updown()), 
			BadWindow, BadMatch, BadValue);

	set_state(FrameStateShaded);
	WindowManager::instance()->hints()->netwm_set_window_state(fdata);
	
	// Configure not needed, since we do nothing usefull to window
	//configure_notify();
}

void Frame::unshade(void)
{
	TRACE_FUNCTION("void Frame::unshade(void)");

	if(!state(FrameStateShaded))
	{
		ELOG("Unshade not shaded window ???");
		return;
	}

	CHECK_RET3(XResizeWindow(fl_display, fl_xid(this), w(), restore_h), BadWindow, BadMatch, BadValue);

	clear_state(FrameStateShaded);
	WindowManager::instance()->hints()->netwm_set_window_state(fdata);

	// Configure not needed, since we do nothing usefull to window
	//configure_notify();
}

void Frame::borders_color(FrameBordersState s)
{
	TRACE_FUNCTION("void Frame::borders_color(FrameBordersState s)");

	Fl_Color szc = borders.sizers_color(s);
	sizer_top_left->color(szc);
	sizer_top_right->color(szc);
	sizer_bottom_left->color(szc);
	sizer_bottom_right->color(szc);

	color(borders.border_color(s));
	redraw();
}

void Frame::show_sizers(void)
{
	TRACE_FUNCTION("void Frame::show_sizers(void)");

	sizer_top_left->show();
	sizer_top_right->show();
	sizer_bottom_left->show();
	sizer_bottom_right->show();
}

void Frame::hide_sizers(void)
{
	TRACE_FUNCTION("void Frame::hide_sizers(void)");

	sizer_top_left->hide();
	sizer_top_right->hide();
	sizer_bottom_left->hide();
	sizer_bottom_right->hide();
}

/* This function will change window type, to some of
 * netwm types. Note, this function should _not_ be used
 * anywhere except from toolbar menu, where window can
 * be modified (if allowed in options).
 *
 * TODO: If we want borderless window, netwm does not provide
 * anything like _NET_WM_WINDOW_TYPE_SPLASH, but not for splashes.
 * Maybe we should add something like _EDE_WM_WINDOW_TYPE_PLAIN ?
 */
void Frame::change_window_type(short type)
{
	TRACE_FUNCTION("void Frame::change_window_type(short type)");

	if(type == fdata->type)
		return;

	// set fdata->window borders, althought fdata->plain.borders is
	// always 0
	CHECK_RET3(XSetWindowBorderWidth(fl_display, fdata->window, fdata->plain.border), 
			BadWindow, BadValue, BadMatch);

	fdata->type = type;

	switch(fdata->type)
	{
		case FrameTypeNormal:
			borders.leftright(BORDER_LEFTRIGHT);
			borders.updown(BORDER_UPDOWN);
			break;
		case FrameTypeDialog:
			borders.leftright(BORDER_THIN_LEFTRIGHT);
			borders.updown(BORDER_THIN_UPDOWN);
			break;
		case FrameTypeSplash:
		case FrameTypeDesktop:
		case FrameTypeMenu:
		case FrameTypeDock:
			borders.leftright(0);
			borders.updown(0);
			if(show_titlebar && titlebar)
			{
				titlebar->hide();
				show_titlebar = false;
			}
			// they don't have visible borders
			break;
	}

	// TODO: setting up initial window sizes
	// should not be in setup_borders();
	if(fdata->type == FrameTypeNormal || fdata->type == FrameTypeDialog)
	{
		int tx, ty;
		tx = fdata->plain.x - borders.leftright();
		ty = fdata->plain.y - borders.updown();
		/*
		if(tx < 0)
			fdata->plain.x = borders.leftright();
		if(ty < 0)
			fdata->plain.y = borders.updown();
		*/
		x(fdata->plain.x);
		y(fdata->plain.y); 

		w(fdata->plain.w + borders.leftright2x());
		h(fdata->plain.h + borders.updown2x());
	}
	// means only borders
	else
	{
		/* First resize window to size of child (x,y will be preserved)
		 * then resize child. 
		 * XXX: child window's x,y are offsets from x,y of parent, so
		 * they are 0.
		 * We can't use Fl_Window::size(), since explicit window sizing
		 * does not work. In that case, slap it with hammer !
		 */
		XResizeWindow(fl_display, fl_xid(this), fdata->plain.w, fdata->plain.h);
		XMoveResizeWindow(fl_display, fdata->window, 0, 0, fdata->plain.w, fdata->plain.h);
	}

	WindowManager::instance()->hints()->netwm_set_window_type(fdata);
}

/* After XGrabButton, WindowManager will FL_PUSH events
 * redirect here. We must make sure after processing them
 * redirect events further to fdata->window.
 *
 * We should not worry about transients (or any window
 * it aot_list) since they will be always on top.
 *
 * TODO: Should we allow events on below windows event
 * if transients are shown ???
 */
void Frame::content_click(void)
{
	ELOG("Clicked on frame: %s", FRAME_NAME(fdata));

	if(fl_xevent.xbutton.button == 1) 
		raise();

	CHECK_RET1(XAllowEvents(fl_display, ReplayPointer, CurrentTime), BadValue);
}

// handle events that efltk understainds
int Frame::handle(int event)
{
	return events->handle_fltk(event);
}

int Frame::handle(XEvent* event)
{
	return events->handle_x(event);
}

// this is here so FrameEventHandler can access Fl_Window
int Frame::handle_parent(int event)
{
	return Fl_Window::handle(event);
}

/* Grab pointer, scheduling pointer events for future use.
 * The only thing I can say is this is the only way of correct
 * usage, althought I would like to see pointer in GrabModeAsync.
 * Later is hardly possible (think so) because efltk will not be
 * able to synchronize itself with X events.
 */
void Frame::grab_cursor(void)
{
	TRACE_FUNCTION("void Frame::grab_cursor(void)");

	if(cursor_grabbed)
		return;

	//if(XGrabPointer(fl_display, fdata->window, 
	if(XGrabPointer(fl_display, fl_xid(this), 
				true, /* owner events */
				ButtonMotionMask | ButtonPressMask |ButtonReleaseMask | PointerMotionMask,
				GrabModeAsync,  /* pointer mode */
				GrabModeAsync, /* keyboard mode */
				None,
				WindowManager::instance()->cursor(), fl_event_time) == GrabSuccess)
	{
		//XAllowEvents(fl_display, AsyncPointer, CurrentTime);
		cursor_grabbed = true;
		ELOG("Cursor grabbed");
	}
}

void Frame::ungrab_cursor(void)
{
	TRACE_FUNCTION("void Frame::ungrab_cursor(void)");

	if(!cursor_grabbed)
		return;

	ELOG("Cursor ungrabbed");
	Fl::event_is_click(0); // avoid double click
	//XAllowEvents(fl_display, Fl::event() == FL_PUSH ? ReplayPointer : AsyncPointer, CurrentTime);
	XUngrabPointer(fl_display, fl_event_time);
	XFlush(fl_display); // make sure we are out of danger before continuing...
	// because we "pushed back" the FL_PUSH, make it think no buttons are down:
	Fl::e_state &= 0xffffff;
	Fl::e_keysym = 0;

	cursor_grabbed = false;
}

void Frame::display_internals(void)
{
	EPRINTF("-----------------------------------\n");
	EPRINTF("window              : %lx\n", fdata->window);
	EPRINTF("transient           : %lx\n", fdata->transient_win);
	EPRINTF("colormap            : %lx\n", fdata->colormap);
	EPRINTF("state               : %li\n", fdata->state);
	EPRINTF("options             : %li\n", fdata->option);
	EPRINTF("type                : %i\n",  fdata->type);
	EPRINTF("min width           : %i\n", fdata->plain.min_w);
	EPRINTF("min height          : %i\n", fdata->plain.min_h);
	EPRINTF("max width           : %i\n", fdata->plain.max_w);
	EPRINTF("max height          : %i\n", fdata->plain.max_h);
	EPRINTF("x                   : %i\n", fdata->plain.x);
	EPRINTF("y                   : %i\n", fdata->plain.y);
	EPRINTF("w                   : %i\n", fdata->plain.w);
	EPRINTF("h                   : %i\n", fdata->plain.h);
	EPRINTF("x (framed)          : %i\n", x());
	EPRINTF("y (framed)          : %i\n", y());
	EPRINTF("w (framed)          : %i\n", w());
	EPRINTF("h (framed)          : %i\n", h());
	EPRINTF("gravity             : %i\n", fdata->win_gravity);
	EPRINTF("autoplace           : %i\n", fdata->autoplace);
	EPRINTF("have icon           : %s\n", (fdata->icon_pixmap ? "yes" : "no"));
	EPRINTF("have icon mask      : %s\n", (fdata->icon_mask ? "yes" : "no"));
	EPRINTF("label               : %s\n", (fdata->label ? fdata->label : "NULL"));
	EPRINTF("-----------------------------------\n");
}
