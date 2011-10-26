#include "Applet.h"

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <edelib/Debug.h>
#include <edelib/Netwm.h>

#include "TaskButton.h"
#include "Taskbar.h"
#include "Panel.h"

#define DEFAULT_CHILD_W 175
#define DEFAULT_SPACING 5

EDELIB_NS_USING(netwm_callback_add)
EDELIB_NS_USING(netwm_callback_remove)
EDELIB_NS_USING(netwm_window_get_all_mapped)
EDELIB_NS_USING(netwm_window_is_manageable)
EDELIB_NS_USING(netwm_window_get_workspace)
EDELIB_NS_USING(netwm_window_get_active)
EDELIB_NS_USING(netwm_window_set_active)
EDELIB_NS_USING(netwm_workspace_get_current)
EDELIB_NS_USING(wm_window_set_state)
EDELIB_NS_USING(wm_window_get_state)
EDELIB_NS_USING(NETWM_CHANGED_ACTIVE_WINDOW)
EDELIB_NS_USING(NETWM_CHANGED_CURRENT_WORKSPACE)
EDELIB_NS_USING(NETWM_CHANGED_WINDOW_LIST)
EDELIB_NS_USING(NETWM_CHANGED_WINDOW_NAME)
EDELIB_NS_USING(NETWM_CHANGED_WINDOW_ICON)
EDELIB_NS_USING(WM_WINDOW_STATE_ICONIC)

static void button_cb(TaskButton *b, void *t) {
	Taskbar *tt = (Taskbar*)t;

	if(Fl::event_button() == FL_RIGHT_MOUSE)
		b->display_menu();
	else 
		tt->activate_window(b);
}

static void net_event_cb(int action, Window xid, void *data) {
	E_RETURN_IF_FAIL(data != NULL);

	if(action == NETWM_CHANGED_CURRENT_WORKSPACE || action == NETWM_CHANGED_WINDOW_LIST) {
		Taskbar *tt = (Taskbar*)data;
		tt->create_task_buttons();
		return;
	}

	if(action == NETWM_CHANGED_WINDOW_NAME) {
		Taskbar *tt = (Taskbar*)data;
		tt->update_child_title(xid);
		return;
	}

	/* this is a message, so property is not changed and netwm_window_get_active() must be called */
	if(action == NETWM_CHANGED_ACTIVE_WINDOW) {
		Taskbar *tt = (Taskbar*)data;
		tt->update_active_button();
		return;
	}

	if(action == NETWM_CHANGED_WINDOW_ICON) {
		Taskbar *tt = (Taskbar*)data;
		tt->update_child_icon(xid);
		return;
	}
}

Taskbar::Taskbar() : Fl_Group(0, 0, 40, 25), curr_active(NULL), prev_active(NULL), panel(NULL) {
	end();

	panel = EDE_PANEL_GET_PANEL_OBJECT(this);

	/* assure display is openned */
	fl_open_display();

	create_task_buttons();
	netwm_callback_add(net_event_cb, this);
}

Taskbar::~Taskbar() {
	netwm_callback_remove(net_event_cb);
}

void Taskbar::create_task_buttons(void) {
	/* erase all current elements */
	if(children())
		clear();

	/* also current/prev storage */
	curr_active = prev_active = NULL;

	/* redraw it, in case no windows exists in this workspace */
	panel_redraw();

	Window *wins, transient_prop_win;
	int     nwins = netwm_window_get_all_mapped(&wins);

	if(nwins > 0) {
		TaskButton *b;
		int         curr_workspace = netwm_workspace_get_current();

		for(int i = 0; i < nwins; i++) {
			transient_prop_win = None;

			if(!netwm_window_is_manageable(wins[i]))
				continue;

			/* 
			 * see if it has WM_TRANSIENT_FOR hint set; transient_prop_win would point to parent window, but
			 * parent should not be root window for this screen
			 */
			if(XGetTransientForHint(fl_display, wins[i], &transient_prop_win) 
					&& (transient_prop_win != None)
					&& (transient_prop_win != RootWindow(fl_display, fl_screen)))
			{
				continue;
			}

			/* 
			 * show window per workspace
			 * TODO: allow showing all windows in each workspace
			 */
			if(curr_workspace == netwm_window_get_workspace(wins[i])) {
				b = new TaskButton(0, 0, DEFAULT_CHILD_W, 25);
				b->set_window_xid(wins[i]);
				b->update_title_from_xid();
				b->update_image_from_xid();

				/* 
				 * catch the name changes 
				 * TODO: put this in Netwm.{h,cpp} 
				 */
				XSelectInput(fl_display, wins[i], PropertyChangeMask | StructureNotifyMask);

				b->callback((Fl_Callback*)button_cb, this);
				add(b);
			}
		}

		XFree(wins);
	}

	layout_children();
	update_active_button();
}

void Taskbar::resize(int X, int Y, int W, int H) {
	Fl_Widget::resize(X, Y, W, H);
	layout_children();
}

void Taskbar::layout_children(void) {
	if(!children())
		return;

	Fl_Widget *o;
	int X = x() + Fl::box_dx(box());
	int Y = y() + Fl::box_dy(box());
	int W = w() - Fl::box_dw(box());

	int child_w = DEFAULT_CHILD_W;
	int sz = children();
	int all_buttons_w = 0;

	/* figure out the bounds */
	for(int i = 0; i < sz; i++)
		all_buttons_w += child(i)->w() + DEFAULT_SPACING;

	if(all_buttons_w > W) {
		int reduce = (all_buttons_w - W) / sz;
		child_w -= reduce; 
	}

	/* now, position each child and resize it if needed */
	for(int i = 0; i < sz; i++) {
		o = child(i);

		o->resize(X, Y, child_w, o->h());
		X += o->w() + DEFAULT_SPACING;
	}
}

void Taskbar::update_active_button(int xid) {
	if(!children())
		return;

	if(xid == -1) {
		xid = netwm_window_get_active();
		/* TODO: make sure panel does not get 'active', or all buttons will be FL_UP_BOX */
	}

	TaskButton *o;
	for(int i = 0; i < children(); i++) {
		o = (TaskButton*)child(i);

		if(o->get_window_xid() == (Window)xid)
			o->box(FL_DOWN_BOX);
		else
			o->box(FL_UP_BOX);
	}

	redraw();
}

void Taskbar::activate_window(TaskButton *b) {
	E_RETURN_IF_FAIL(b != NULL);

	Window xid = b->get_window_xid();

	/* if clicked on activated button, it will be minimized, then next one will be activated */
	if(b == curr_active) {
		if(wm_window_get_state(xid) != WM_WINDOW_STATE_ICONIC) {
			/* minimize if not so */
			wm_window_set_state(xid, WM_WINDOW_STATE_ICONIC);

			if(prev_active && 
			   prev_active != b && 
			   wm_window_get_state(prev_active->get_window_xid()) != WM_WINDOW_STATE_ICONIC) 
			{
				xid = prev_active->get_window_xid();
				b   = prev_active;
			} else {
				return;
			}
		}
	} 

	/* active or restore minimized */
	netwm_window_set_active(xid);
	update_active_button(xid);

	/* TODO: use stack for this (case when this can't handle: minimize three window, out of four on the workspace) */
	prev_active = curr_active;
	curr_active = b;
}

void Taskbar::update_child_title(Window xid) {
	E_RETURN_IF_FAIL(xid >= 0);

	TaskButton *o;
	for(int i = 0; i < children(); i++) {
		o = (TaskButton*)child(i);

		if(o->get_window_xid() == xid) {
			o->update_title_from_xid();
			break;
		}
	}
}

void Taskbar::update_child_icon(Window xid) {
	E_RETURN_IF_FAIL(xid >= 0);

	TaskButton *o;
	for(int i = 0; i < children(); i++) {
		o = (TaskButton*)child(i);

		if(o->get_window_xid() == xid) {
			o->update_image_from_xid();
			o->redraw();
			break;
		}
	}
}

void Taskbar::panel_redraw(void) {
	E_RETURN_IF_FAIL(panel != NULL);
	panel->redraw();
}


EDE_PANEL_APPLET_EXPORT (
 Taskbar, 
 EDE_PANEL_APPLET_OPTION_ALIGN_LEFT | EDE_PANEL_APPLET_OPTION_RESIZABLE_H,
 "Taskbar",
 "0.1",
 "empty",
 "Sanel Zukan"
)
