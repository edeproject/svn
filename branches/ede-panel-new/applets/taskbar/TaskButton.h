#ifndef __TASKBUTTON_H__
#define __TASKBUTTON_H__

#include <FL/Fl_Button.H>
#include <FL/x.H>

class TaskButton : public Fl_Button {
private:
	/* window ID this button handles */
	Window xid;

public:
	TaskButton(int X, int Y, int W, int H, const char *l = 0);

	void draw(void);

	void    set_window_xid(Window win) { xid = win; }
	Window  get_window_xid(void) { return xid; }

	void copy_label_with_tooltip(const char *l) { 
		copy_label(l); 
		tooltip(label()); 
	}
};

#endif
