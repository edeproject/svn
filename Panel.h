#ifndef __PANEL_H__
#define __PANEL_H__

#include <edelib/Window.h>
#include "AppletManager.h"

#define EDE_PANEL_CAST_TO_PANEL(obj) ((Panel*)(obj))

EDELIB_NS_USING_AS(Window, PanelWindow)

class Panel : public PanelWindow {
private:
	Fl_Widget *clicked;
	int        sx, sy;
	int        screen_w, screen_h;
	bool       can_move_widgets;

	AppletManager mgr;

	void do_layout(void);

public:
	Panel() : PanelWindow(300, 30), clicked(0), sx(0), sy(0), screen_w(0), screen_h(0), can_move_widgets(false) 
	{ box(FL_UP_BOX); }

	int  handle(int e);
	void show(void);
	void load_applets(void);

	int panel_w(void) { return w(); }
	int panel_h(void) { return h(); }
};

#endif
