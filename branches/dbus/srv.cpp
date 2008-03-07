#include <stdio.h>
#include <string.h>
#include <Fl/Fl_Window.h>
#include <Fl/Fl.h>

#include "EdbusConnection.h"
#include "EdbusDict.h"

int signal_cb(const EdbusMessage* m, void*) {
	printf("Got signal: %s : %s : %s\n", m->path(), m->interface(), m->member());
	return 1;
}

int method_cb(const EdbusMessage* m, void* w) {
	Fl_Window* win = (Fl_Window*)w;
	printf("Got call: %s : %s : %s\n", m->path(), m->interface(), m->member());
	printf("Message signature is: %s\n", m->signature());

	if(strcmp(m->member(), "ChangeBackground") == 0) {
		EdbusMessage::iterator it = m->begin(), it_end = m->end();
		for(; it != it_end; ++it) {
			if((*it).is_int16()) {
				int c = (*it).to_int16();
				win->color(c);
				win->redraw();
			}

			if((*it).is_dict()) {
				EdbusDict dict = (*it).to_dict();
				EdbusDict::iterator dict_it = dict.begin(), dict_it_end = dict.end();

				for(; dict_it != dict_it_end; ++dict_it) {
					if((*dict_it).key.is_string())
						printf("+++> key is %s\n", (*dict_it).key.to_string());
					if((*dict_it).value.is_string())
						printf("+++> value is %s\n", (*dict_it).value.to_string());
				}
			}
		}
	}

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

	Fl_Window* win = new Fl_Window(100, 100, "bla bla");
	win->end();

	srv.signal_callback(signal_cb, 0);
	srv.method_callback(method_cb, win);

	win->show();
	srv.setup_listener_with_fltk();

	return Fl::run();

	/*
	srv.setup_listener();

	while(srv.wait(9000))
		puts("Tick");
	*/
}
