#include "Applet.h"

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <edelib/Debug.h>

#include "Netwm.h"
#include "TaskButton.h"

#define DEFAULT_CHILD_W 175
#define DEFAULT_SPACING 5

class Taskbar : public Fl_Group {
public:
	Taskbar();
	~Taskbar();

	void create_task_buttons(void);

	void resize(int X, int Y, int W, int H);
	void layout_children(void);

	void update_active_button(int xid = -1);
	void activate_window(TaskButton *b);
	void update_child_title(Window xid);
};

static void button_cb(TaskButton *b, void *t) {
	Taskbar *tt = (Taskbar*)t;
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

	/* this is a message, so property is not changed and netwm_get_active_window() must be called */
	if(action == NETWM_CHANGED_ACTIVE_WINDOW) {
		Taskbar *tt = (Taskbar*)data;
		tt->update_active_button();
		return;
	}
}

Taskbar::Taskbar() : Fl_Group(0, 0, 40, 25) {
	end();

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
	clear();

	/* redraw it, in case no windws exists in this workspace */
	parent()->redraw();

	Window *wins;
	int     nwins = netwm_get_mapped_windows(&wins);

	if(nwins) {
		TaskButton *b;
		int curr_workspace = netwm_get_current_workspace();
		char *title;

		for(int i = 0; i < nwins; i++) {
			if(!netwm_manageable_window(wins[i]))
				continue;

			/* 
			 * show window per workspace
			 * TODO: allow showing all windows in each workspace
			 */
			if(curr_workspace == netwm_get_window_workspace(wins[i])) {
				b = new TaskButton(0, 0, DEFAULT_CHILD_W, 25);
				b->set_window_xid(wins[i]);
				b->update_title_from_xid();

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
		xid = netwm_get_active_window();
		/* TODO: make sure panel does not get 'active', or all buttons will be FL_UP_BOX */
	}

	TaskButton *o;
	for(int i = 0; i < children(); i++) {
		o = (TaskButton*)child(i);

		if(o->get_window_xid() == xid)
			o->box(FL_DOWN_BOX);
		else
			o->box(FL_UP_BOX);
	}

	redraw();
}

void Taskbar::activate_window(TaskButton *b) {
	E_RETURN_IF_FAIL(b != NULL);

	/* TODO: disallow activating already activated window */
	netwm_set_active_window(b->get_window_xid());
	update_active_button(b->get_window_xid());
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


EDE_PANEL_APPLET_EXPORT (
 Taskbar, 
 EDE_PANEL_APPLET_OPTION_ALIGN_LEFT | EDE_PANEL_APPLET_OPTION_RESIZABLE_H,
 "Taskbar",
 "0.1",
 "empty",
 "Sanel Zukan"
)
