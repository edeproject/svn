#include "EdbusConnection.h"
#include <stdio.h>

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

	srv.setup_listener();

	while(srv.wait(9000))
		puts("Tick");
}
