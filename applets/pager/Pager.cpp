#include "Applet.h"

#include <string.h>

#include <FL/Fl_Group.H>
#include <FL/Fl.H>
#include <FL/x.H>
#include <edelib/Debug.h>
#include <edelib/List.h>

#include "PagerButton.h"

EDELIB_NS_USING(list);

#define ATOM_INIT_ONCE(var, name) if(!var) var = XInternAtom(fl_display, name, False)

class Pager;

/* Fl::add_handler() does not have additional parameter, so list is used to register multiple instances */
typedef list<Pager*> PagerList;
typedef list<Pager*>::iterator PagerListIt;

static PagerList pager_list;

static Atom _XA_NET_NUMBER_OF_DESKTOPS = 0;
static Atom _XA_NET_CURRENT_DESKTOP    = 0;
static Atom _XA_NET_DESKTOP_NAMES      = 0;

class Pager : public Fl_Group {
public:
	Pager();
	void init_workspace_boxes(void);
	void workspace_changed(void);
};

static int net_get_workspace_count(void) {
	Atom real;
	int format;
	unsigned long n, extra;
	unsigned char* prop;

	ATOM_INIT_ONCE(_XA_NET_NUMBER_OF_DESKTOPS, "_NET_NUMBER_OF_DESKTOPS");

	int status = XGetWindowProperty(fl_display, RootWindow(fl_display, fl_screen), 
			_XA_NET_NUMBER_OF_DESKTOPS, 0L, 0x7fffffff, False, XA_CARDINAL, &real, &format, &n, &extra, 
			(unsigned char**)&prop);

	if(status != Success && !prop)
		return -1;

	int ns = int(*(long*)prop);
	XFree(prop);
	return ns;
}

/* desktops are starting from 0 */
static int net_get_current_desktop(void) {
	Atom real;
	int format;
	unsigned long n, extra;
	unsigned char* prop;

	ATOM_INIT_ONCE(_XA_NET_CURRENT_DESKTOP, "_NET_CURRENT_DESKTOP");

	int status = XGetWindowProperty(fl_display, RootWindow(fl_display, fl_screen), 
			_XA_NET_CURRENT_DESKTOP, 0L, 0x7fffffff, False, XA_CARDINAL, &real, &format, &n, &extra, 
			(unsigned char**)&prop);

	if(status != Success && !prop)
		return -1;

	int ns = int(*(long*)prop);
	XFree(prop);
	return ns;
}

/* call on this XFreeStringList(names) */
static int net_get_workspace_names(char**& names) {
	ATOM_INIT_ONCE(_XA_NET_DESKTOP_NAMES, "_NET_DESKTOP_NAMES");

	/* FIXME: add _NET_SUPPORTING_WM_CHECK and _NET_SUPPORTED ??? */
	XTextProperty wnames;
	XGetTextProperty(fl_display, RootWindow(fl_display, fl_screen), &wnames, _XA_NET_DESKTOP_NAMES);

	/* if wm does not understainds _NET_DESKTOP_NAMES this is not set */
	if(!wnames.nitems || !wnames.value)
		return 0;

	int nsz;

	/*
	 * FIXME: Here should as alternative Xutf8TextPropertyToTextList since
	 * many wm's set UTF8_STRING property. Below is XA_STRING and for UTF8_STRING
	 * will fail.
	 */
	if(!XTextPropertyToStringList(&wnames, &names, &nsz)) {
		XFree(wnames.value);
		return 0;
	}

	XFree(wnames.value);
	return nsz;
}

static void net_change_workspace(int n) {
	ATOM_INIT_ONCE(_XA_NET_CURRENT_DESKTOP, "_NET_CURRENT_DESKTOP");

	XEvent xev;
	Window root_win = RootWindow(fl_display, fl_screen);

	memset(&xev, 0, sizeof(xev));
	xev.xclient.type = ClientMessage;
	xev.xclient.serial = 0;
	xev.xclient.send_event = True;
	xev.xclient.window = root_win;
	xev.xclient.display = fl_display;
	xev.xclient.message_type = _XA_NET_CURRENT_DESKTOP;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = (long)n;
    xev.xclient.data.l[1] = CurrentTime;

	XSendEvent (fl_display, root_win, False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
    XSync(fl_display, True);
}

static void box_cb(Fl_Widget*, void *b) {
	PagerButton *pb = (PagerButton*)b;

	/* because workspaces in button labels are increased */
	int s = pb->get_workspace_label() - 1;
	net_change_workspace(s);
}

static int xevent_handler(int e) {
	if(fl_xevent->type == PropertyNotify && fl_xevent->xproperty.atom == _XA_NET_CURRENT_DESKTOP) { 
		PagerListIt it = pager_list.begin(), it_end = pager_list.end();

		for(; it != it_end; ++it)
			(*it)->workspace_changed();
	}

	return 0;
}

Pager::Pager() : Fl_Group(0, 0, 25, 25, NULL) { 
	box(FL_DOWN_BOX);
	/* we will explicitly add elements */
	end();

	init_workspace_boxes();
	XSelectInput(fl_display, RootWindow(fl_display, fl_screen), PropertyChangeMask | StructureNotifyMask);

	ATOM_INIT_ONCE(_XA_NET_CURRENT_DESKTOP, "_NET_CURRENT_DESKTOP");

	/* so we do not register multiple handlers */
	Fl::remove_handler(xevent_handler);
	Fl::add_handler(xevent_handler);

	pager_list.push_back(this);
}

void Pager::init_workspace_boxes(void) {
	int X, Y, H;

	/* prepare the sizes */
	X = x() + Fl::box_dx(box());
	Y = y() + Fl::box_dy(box());
	H = h() - Fl::box_dh(box());

	int  nworkspaces, curr_workspace;
	char **names = 0;
	char nbuf[16];

	nworkspaces    = net_get_workspace_count();
	curr_workspace = net_get_current_desktop();
	net_get_workspace_names(names);

	/* 
	 * Resize group before childs, or they will be resized too, messing things up.
	 * Here, each child is 25px wide plus 1px for delimiter between the childs .
	 */
	int gw = nworkspaces * (25 + 1);
	gw += Fl::box_dw(box());
	/* last child do not ends with 1px wide delimiter */
	gw -= 1;

	size(gw, h());

	for(int i = 0; i < nworkspaces; i++) {
		/* let box width be fixed */
		PagerButton *bx = new PagerButton(X, Y, 25, H);
		bx->box(FL_FLAT_BOX);

		if(i == curr_workspace)
			bx->select_it(1);
		else
			bx->select_it(0);

		/* workspaces are started from 0 */
		bx->set_workspace_label(i + 1);

		if(names)
			bx->copy_tooltip(names[i]);

		bx->callback(box_cb, bx);

		add(bx);
		/* position for the next box */
		X = bx->x() + bx->w() + 1;
	}

	XFreeStringList(names);
}

void Pager::workspace_changed(void) {
	int c = net_get_current_desktop();
	PagerButton *pb;

	E_RETURN_IF_FAIL(c < children());

	for(int i = 0; i < children(); i++) {
		pb = (PagerButton*)child(i);
		pb->select_it(0);
	}

	pb = (PagerButton*)child(c);
	pb->select_it(1);
}

EDE_PANEL_APPLET_EXPORT (
 Pager, 
 EDE_PANEL_APPLET_OPTION_ALIGN_LEFT,
 "Desktop switcher",
 "0.1",
 "empty",
 "Sanel Zukan"
)
