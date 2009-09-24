#include "Applet.h"

#include <stdio.h>

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl.H>
#include <FL/x.H>
#include <edelib/Debug.h>

static Atom _XA_NET_NUMBER_OF_DESKTOPS = 0;
static Atom _XA_NET_CURRENT_DESKTOP    = 0;
static Atom _XA_NET_DESKTOP_NAMES      = 0;

static int net_get_workspace_count(void) {
	Atom real;
	int format;
	unsigned long n, extra;
	unsigned char* prop;

	if(!_XA_NET_NUMBER_OF_DESKTOPS)
		_XA_NET_NUMBER_OF_DESKTOPS = XInternAtom(fl_display, "_NET_NUMBER_OF_DESKTOPS", False);

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


	if(!_XA_NET_CURRENT_DESKTOP)
		_XA_NET_CURRENT_DESKTOP = XInternAtom(fl_display, "_NET_CURRENT_DESKTOP", False);

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
	if(!_XA_NET_DESKTOP_NAMES)
		_XA_NET_DESKTOP_NAMES = XInternAtom(fl_display, "_NET_DESKTOP_NAMES", False);

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

static void box_cb(Fl_Widget *, void *) {
}

class Pager : public Fl_Group {
public:
	Pager();
};

Pager::Pager() : Fl_Group(0, 0, 25, 25, NULL) { 
	box(FL_DOWN_BOX);
	/* we will explicitly add elements */
	end();

	int X, Y, H;

	/* prepare the sizes */
	X = x() + Fl::box_dx(box());
	Y = y() + Fl::box_dy(box());
	H = h() - Fl::box_dh(box());

	int  nworkspaces, curr_workspace;
	char nbuf[16];

	nworkspaces    = net_get_workspace_count();
	curr_workspace = net_get_current_desktop();

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
		Fl_Box *bx = new Fl_Box(X, Y, 25, H);

		if(i == curr_workspace) {
			bx->color((Fl_Color)44);
			bx->labelcolor(FL_BLACK);
		} else {
			bx->color((Fl_Color)33);
			bx->labelcolor(FL_GRAY);
		}

		/* workspaces are started from 0 */
		snprintf(nbuf, sizeof(nbuf), "%i", i + 1);
		bx->copy_label(nbuf);
		bx->box(FL_FLAT_BOX);

		bx->callback(box_cb);

		add(bx);
		/* position for the next box */
		X = bx->x() + bx->w() + 1;
	}
}

EDE_PANEL_APPLET_EXPORT (
 Pager, 
 EDE_PANEL_APPLET_OPTION_ALIGN_LEFT,
 "Desktop switcher",
 "0.1",
 "empty",
 "Sanel Zukan"
)
