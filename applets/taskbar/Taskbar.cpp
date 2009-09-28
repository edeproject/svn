#include "Applet.h"

#include <stdlib.h>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Button.H>
#include <edelib/Debug.h>

#include "Netwm.h"
#include "TaskButton.h"

#define DEFAULT_CHILD_W 175

class Taskbar : public Fl_Pack {
public:
	Taskbar();
	~Taskbar();

	void create_task_buttons(void);

	void resize(int X, int Y, int W, int H);
	void layout_children(void);

	void update_active_button(int xid = -1);
	void activate_window(TaskButton *b);
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
	}
}

Taskbar::Taskbar() : Fl_Pack(0, 0, 40, 25) {
	end();

	type(Fl_Pack::HORIZONTAL);
	spacing(5);

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

				title = netwm_get_window_title(wins[i]);
				if(title) {
					b->copy_label_with_tooltip(title);
					free(title);
				}

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
	int X = 0, W = DEFAULT_CHILD_W, sz = children(), all_buttons_w = 0;

	for(int i = 0; i < sz; i++)
		all_buttons_w += child(i)->w() + spacing();

	float scale = 0.0;

	if(all_buttons_w > w())
		scale = (float)w() / (float)all_buttons_w;

	for(int i = 0; i < sz; i++) {
		o = child(i);

		if(scale > 0.0)
			W = (int)((float)W * scale);

		o->resize(X, o->y(), W, o->h());
		X += W + spacing();
	}



#if 0
	E_DEBUG("%i %i\n", all_buttons_w, w());

	if(all_buttons_w > w()) {
		int reduce = (all_buttons_w - w()) / sz;
		W -= reduce; 
	}

	for(int i = 0; i < sz; i++) {
		o = child(i);

		o->resize(X, o->y(), W, o->h());
		X += W + spacing();
	}
#endif
}

void Taskbar::update_active_button(int xid) {
	if(!children())
		return;

	if(xid == -1)
		xid = netwm_get_active_window();

	if(xid < 0)
		return;

	TaskButton *o;
	for(int i = 0; i < children(); i++) {
		o = (TaskButton*)child(i);

		if(o->get_window_xid() == xid)
			o->box(FL_DOWN_BOX);
		else
			o->box(FL_UP_BOX);
	}

	parent()->redraw();
}

void Taskbar::activate_window(TaskButton *b) {
	E_RETURN_IF_FAIL(b != NULL);

	/* TODO: disallow activating already activated window */
	netwm_set_active_window(b->get_window_xid());
	update_active_button(b->get_window_xid());
}


EDE_PANEL_APPLET_EXPORT (
 Taskbar, 
 EDE_PANEL_APPLET_OPTION_ALIGN_LEFT | EDE_PANEL_APPLET_OPTION_RESIZABLE_H,
 "Taskbar",
 "0.1",
 "empty",
 "Sanel Zukan"
)
