#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include <X11/Xproto.h>
#include <edelib/Debug.h>
#include <edelib/List.h>

#include "Panel.h"

/* empty space from left and right panel border */
#define INITIAL_SPACING 5

/* space between each applet */
#define DEFAULT_SPACING 5

#undef MIN
#define MIN(x,y)  ((x) < (y) ? (x) : (y))
#undef MAX
#define MAX(x,y)  ((x) > (y) ? (x) : (y))

EDELIB_NS_USING(list)

typedef list<Fl_Widget*> WidgetList;
typedef list<Fl_Widget*>::iterator WidgetListIt;

inline bool intersects(Fl_Widget *o1, Fl_Widget *o2) {
	    return (MAX(o1->x(), o2->x()) <= MIN(o1->x() + o1->w(), o2->x() + o2->w()) &&
	            MAX(o1->y(), o2->y()) <= MIN(o1->y() + o1->h(), o2->y() + o2->h()));
}

static bool net_get_workarea(int& x, int& y, int& w, int &h) {
	static Atom _net_workarea = XInternAtom(fl_display, "_NET_WORKAREA", False);
	Atom real;

	int format;
	unsigned long n, extra;
	unsigned char* prop;
	x = y = w = h = 0;

	int status = XGetWindowProperty(fl_display, RootWindow(fl_display, fl_screen), 
			_net_workarea, 0L, 0x7fffffff, False, XA_CARDINAL, &real, &format, &n, &extra, (unsigned char**)&prop);

	if(status != Success)
		return false;

	CARD32* val = (CARD32*)prop;
	if(val) {
		x = val[0];
		y = val[1];
		w = val[2];
		h = val[3];

		XFree((char*)val);
		return true;
	}

	return false;
}

static void net_make_me_dock(Fl_Window *win) {
	static Atom _net_wm_window_type      = XInternAtom(fl_display, "_NET_WM_WINDOW_TYPE", False);
	static Atom _net_wm_window_type_dock = XInternAtom(fl_display, "_NET_WM_WINDOW_TYPE_DOCK", False);

	XChangeProperty(fl_display, fl_xid(win), _net_wm_window_type, XA_ATOM, 32, PropModeReplace, 
			(unsigned char*)&_net_wm_window_type_dock, sizeof(Atom));
}

/* horizontaly centers widget in the panel */
static void center_widget_h(Fl_Widget *o, Panel *self) {
	int ph, wy;

	ph = self->panel_h() / 2;
	wy = ph - (o->h() / 2);
	o->position(o->x(), wy);
}

static void add_from_list(WidgetList &lst, Panel *self, int &X, bool inc) {
	WidgetListIt it = lst.begin(), it_end = lst.end();
	Fl_Widget    *o;

	while(it != it_end) {
		o = *it;

		/* fix y position */
		center_widget_h(o, self);

		/* 'inc == false' means we are going from right to left */
		if(!inc)
			X -= o->w();

		/* fix x position */
		o->position(X, o->y());

		/* add it to the group */
		self->add(o);

		if(inc) {
			X += DEFAULT_SPACING;
			X += o->w();
		} else {
			X -= DEFAULT_SPACING;
		}

		it = lst.erase(it);
	}
}

static void move_widget(Panel *self, Fl_Widget *o, int &sx, int &sy) {
	int tx, ty, px, py;
	Fl_Widget *const *a;

	tx = Fl::event_x() - sx;
	ty = Fl::event_y() - sy;

	px = o->x() + tx;
	py = o->y() + ty;

	/* check the bounds */
	if(px < 0)
		px = 0;
	if(px + o->w() > self->panel_w())
		px = self->panel_w() - o->w();
	if(py < 0)
		py = 0;
	if(py + o->h() > self->y() + self->panel_h())
		py = self->y() + self->panel_h() - o->h();
	if(py + o->h() > self->panel_h())
		py = self->panel_h() - o->h();

	/* o->position(px, py); */
	o->position(px, o->y());

	/* find possible collision and move others */
	a = self->array();
	for(int i = self->children(); i--; ) {
		if(o == a[i])
			continue;

		if(intersects(a[i], o)) {
			px = a[i]->x() + tx;
			py = a[i]->y() + ty;

			/* check the bounds */
			if(px < 0)
				px = 0;
			if(px + o->w() > self->panel_w())
				px = self->panel_w() - o->w();
			if(py < 0)
				py = 0;
			if(py + o->h() > self->y() + self->panel_h())
				py = self->y() + self->panel_h() - o->h();
			if(py + o->h() > self->panel_h())
				py = self->panel_h() - o->h();

			/* a[i]->position(px, py); */
			a[i]->position(px, a[i]->y());
		}
	}

	/* update current position */
	sx = Fl::event_x();
	sy = Fl::event_y();

	o->parent()->redraw();
}

void Panel::set_window_strut(int left, int right, int top, int bottom) {
	static Atom _net_wm_strut = XInternAtom(fl_display, "_NET_WM_STRUT", False);
	CARD32 strut[4];

	strut[0] = left;
	strut[1] = right;
	strut[2] = top;
	strut[3] = bottom;

	XChangeProperty(fl_display, fl_xid(this), _net_wm_strut, XA_CARDINAL, 32, 
			PropModeReplace, (unsigned char *)&strut, sizeof(CARD32) * 4);
}

void Panel::do_layout(void) {
	E_RETURN_IF_FAIL(mgr.napplets() > 0);

	Fl_Widget     *o;
	unsigned long  opts;
	unsigned int   lsz;
	int            X;

	WidgetList left, right, center, unmanaged;

	for(int i = 0; i < children(); i++) {
		o = child(i);

		/* could be slow, but I'm relaying how number of loaded applets will not be huge */
		if(!mgr.get_applet_options(o, opts)) {
			/* here are put widgets not loaded as applets */
			unmanaged.push_back(o);
			continue;
		}

		if(opts & EDE_PANEL_APPLET_OPTION_ALIGN_LEFT) {
			/* first item will be most leftest */
			left.push_back(o);
			continue;
		}

		if(opts & EDE_PANEL_APPLET_OPTION_ALIGN_RIGHT) {
			/* first item will be most rightest */
			right.push_back(o);
			continue;
		}

		/* rest of them */
		center.push_back(o);
	}

	/* make sure we at the end have all widgets, so we can overwrite group array */
	lsz = left.size() + center.size() + right.size() + unmanaged.size();
	E_ASSERT(lsz == (unsigned int)children() && "Size of layout lists size not equal to group size");

	X = INITIAL_SPACING;

	/* 
	 * Call add() on each element, processing lists in order. add() will remove element
	 * in group array and put it at the end of array. At the end, we should have array ordered by
	 * layout flags.
	 */
	add_from_list(left, this, X, true);
	add_from_list(center, this, X, true);
	add_from_list(unmanaged, this, X, true);

	/* elements right will be put from starting from the right panel border */
	X = w();
	X -= INITIAL_SPACING;
	add_from_list(right, this, X, false);
}

void Panel::show(void) {
	if(shown()) {
		Fl_Window::show();
		return;
	}

	int X, Y, W, H;

	fl_open_display();

	/* position it */
	net_get_workarea(X, Y, W, H);
	resize(X, Y + H - 35, W, 35);

	do_layout();

	for(int i = 0; i < children(); i++)
		center_widget_h(child(i), this);

	Fl_X::make_xid(this);
	net_make_me_dock(this);
}

int Panel::handle(int e) {
	switch(e) {
		case FL_PUSH:
			sx = Fl::event_x();
			sy = Fl::event_y();

			clicked = Fl::belowmouse();
			if(clicked == this)
				clicked = 0;
			else
				clicked->handle(e);

			sx = Fl::event_x();
			sy = Fl::event_y();
			return 1;

		case FL_DRAG:
			if(clicked) {
				fl_cursor(FL_CURSOR_MOVE);
				move_widget(this, clicked, sx, sy);
				clicked->handle(e);
			}
			return 1;

		case FL_RELEASE:
			fl_cursor(FL_CURSOR_DEFAULT);

			if(clicked) {
				clicked->handle(e);
				clicked = 0;
			}
			return 1;
	}

	return Fl_Window::handle(e);
}

void Panel::load_applets(void) {
	mgr.load("./applets/start-menu/edepanel_start_menu.so");
	mgr.load("./applets/quick-launch/edepanel_quick_launch.so");
	mgr.load("./applets/clock/edepanel_clock.so");
	mgr.load("./applets/pager/edepanel_pager.so");
	mgr.fill_group(this);
}
