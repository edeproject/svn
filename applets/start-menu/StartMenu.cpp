#include "Applet.h"

#include <FL/Fl_Button.H>
#include <FL/x.H>
#include <FL/Fl.H>

#include <stdio.h>

#include "XdgMenuReader.h"

class StartMenu : public Fl_Button {
public:
	StartMenu() : Fl_Button(0, 0, 90, 25, "EDE") { 
		color(FL_RED);
		xdg_menu_load();
	}

	~StartMenu() { }
};

EDE_PANEL_APPLET_EXPORT (
 StartMenu, 
 "Start menu",
 "0.1",
 "empty",
 "Sanel Zukan"
)
