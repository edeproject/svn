#ifndef __PANEL_H__
#define __PANEL_H__

#include <FL/Fl_Window.H>

class Panel : public Fl_Window {
private:
	Fl_Widget *clicked;
	int        sx, sy;
public:
	Panel(int w, int h, const char *l = 0) : Fl_Window(w, h, l), clicked(0), sx(0), sy(0) { }
	~Panel() { }
	int handle(int e);
};

#endif
