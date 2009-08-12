#ifndef __PANEL_H__
#define __PANEL_H__

#include <FL/Fl_Window.H>

#define EDE_PANEL_CAST_TO_PANEL(obj) ((Panel*)(obj))

class Panel : public Fl_Window {
private:
	Fl_Widget *clicked;
	int        sx, sy;
	bool       can_move_widgets;

	void set_window_strut(int left, int right, int top, int bottom);

public:
	Panel() : Fl_Window(300, 30), clicked(0), sx(0), sy(0), can_move_widgets(false) { box(FL_UP_BOX); }
	~Panel() { }
	int  handle(int e);
	void show(void);

	int panel_w(void) { return w(); }
	int panel_h(void) { return h(); }
};

#endif
