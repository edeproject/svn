#include <stdio.h>
#include <string.h>
#include <Fl/Fl_Window.h>
#include <Fl/Fl.h>

#include "EdbusConnection.h"
#include "EdbusDict.h"
#include "EdbusList.h"

EdbusConnection* global_conn = NULL;

int signal_cb(const EdbusMessage* m, void*) {
	printf("Got signal: %s : %s : %s\n", m->path(), m->interface(), m->member());
	return 1;
}

const char* introspection_data =
"<!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\"\n"
"\"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">\n"
"<node>\n"
"  <interface name=\"org.freedesktop.DBus.Introspectable\">\n"
"    <method name=\"Introspect\">\n"
"      <arg name=\"data\" direction=\"out\" type=\"s\"/>\n"
"    </method>\n"
"  </interface>\n"
"</node>\n";

int method_cb(const EdbusMessage* m, void* w) {
	if(!m) {
		puts("Got NULL?");
		return 1;
	}

	Fl_Window* win = (Fl_Window*)w;
	printf("Got call: %s : %s : %s\n", m->path(), m->interface(), m->member());
	printf("Message signature is: %s\n", m->signature());
	printf("Message size s: %i\n", m->size());

	if(strcmp(m->path(), "/org/equinoxproject/Foo") == 0) {
		EdbusMessage reply;
		reply.create_reply(*m);
		reply << introspection_data;
		global_conn->send(reply);
		return 1;
	}

	if(strcmp(m->member(), "ChangeBackground") == 0) {
		EdbusMessage::const_iterator it = m->begin(), it_end = m->end();

		for(; it != it_end; ++it) {
			if((*it).is_int16()) {
				int c = (*it).to_int16();
				win->color(c);
				win->redraw();
			}

			if((*it).is_dict()) {
				EdbusDict dict = (*it).to_dict();
				EdbusDict::const_iterator dict_it = dict.begin(), dict_it_end = dict.end();

				printf("dict size is %i\n", dict.size());

				for(; dict_it != dict_it_end; ++dict_it) {
					if((*dict_it).key.is_string())
						printf("+++> key is %s\n", (*dict_it).key.to_string());
					if((*dict_it).value.is_string())
						printf("+++> value is %s\n", (*dict_it).value.to_string());
				}
			}

			if((*it).is_array()) {
				EdbusList arr = (*it).to_array();
				EdbusList::const_iterator arr_it = arr.begin(), arr_it_end = arr.end();

				printf("array content:");
				for(; arr_it != arr_it_end; ++arr_it) {
					if((*arr_it).is_int32())
						printf(" %i", (*arr_it).to_int32());
				}
				printf("\n");
			}

			if((*it).is_variant()) {
				EdbusVariant var = (*it).to_variant();
				if(var.value.is_array()) {
					printf("Got variant with array, content:");

					EdbusList arr = var.value.to_array();
					EdbusList::const_iterator arr_it = arr.begin(), arr_it_end = arr.end();

					for(; arr_it != arr_it_end; ++arr_it) {
						if((*arr_it).is_int32()) {
							printf(" %i", (*arr_it).to_int32());
						}
					}

					printf("\n");
				}
			}

			if((*it).is_struct()) {
				EdbusList s = (*it).to_struct();

				if(s.size() == 3) {
					EdbusList::const_iterator it = s.begin();

					printf("Got struct with values:");
					printf(" %i", (*it).to_int32());
					++it;
					printf(" %c", (*it).to_char());
					++it;
					printf(" %s", (*it).to_string());

					printf("\n");
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

	global_conn = &srv;

	Fl_Window* win = new Fl_Window(100, 100, "bla bla");
	win->end();

	srv.signal_callback(signal_cb, 0);
	srv.method_callback(method_cb, win);

	srv.register_object("/org/equinoxproject/Foo");
	srv.register_object("/org/equinoxproject/Baz");
	srv.register_object("/org/equinoxproject/Object");

	win->show();
	srv.setup_listener_with_fltk();

	return Fl::run();

	/*
	srv.setup_listener();

	while(srv.wait(9000))
		puts("Tick");
	*/
}
