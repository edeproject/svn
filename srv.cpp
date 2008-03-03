#include <stdio.h>
#include <Fl/Fl_Window.h>
#include <Fl/Fl.h>
#include "EdbusConnection.h"

int signal_cb(const EdbusMessage* m, void*) {
	printf("Got signal: %s : %s : %s\n", m->path(), m->interface(), m->member());
	return 1;
}

int method_cb(const EdbusMessage* m, void*) {
	printf("Got call: %s : %s : %s\n", m->path(), m->interface(), m->member());
	return 1;
}

int main() {
	EdbusConnection srv;
	if(!srv.connect(EDBUS_SESSION)) {
		puts("No session connection");
		return 1;
	}

	if(!srv.request_name("org.equinoxproject.server")) {
		puts("Can't set known name");
		return 1;
	}

	srv.signal_callback(signal_cb, 0);
	srv.method_callback(method_cb, 0);

	Fl_Window* win = new Fl_Window(100, 100, "bla bla");

	win->end();
	win->show();
	srv.setup_listener_with_fltk();

	return Fl::run();

	/*
	srv.setup_listener();

	while(srv.wait(9000))
		puts("Tick");
	*/
}
