#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>

#include "Panel.h"
#include "AppletManager.h"

int main(int argc, char **argv) {
	Panel* panel = new Panel();
	panel->begin();
		new Fl_Button(195, 25, 90, 25, "button 2");
		new Fl_Button(95, 25, 90, 25, "button 1");
	panel->end();

	AppletManager mgr;
	mgr.load("./applets/demo/edepanel_demo.so");
	mgr.fill_group(panel);

	panel->show();
	return Fl::run();
}
