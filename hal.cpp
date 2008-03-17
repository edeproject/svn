#include "EdbusConnection.h"
#include <stdio.h>
#include <string.h>
#if 0
int signal_cb(const EdbusMessage* msg, void*) {
	printf("Got %s\n", msg->interface());

	if(strcmp(msg->interface(), "org.freedesktop.Hal.Manager") != 0)
		return 1;

	if(strcmp(msg->path(), "/org/freedesktop/Hal/Manager") != 0)
		return 1;

	printf("got member %s\n", msg->member());

	if(strcmp(msg->member(), "DeviceAdded") == 0) {
		EdbusMessage::const_iterator it = msg->begin();
		if(msg->size() > 0)
			printf("Added device %s\n", (*it).to_string());
	}
		
	if(strcmp(msg->member(), "DeviceRemoved") == 0) {
		EdbusMessage::const_iterator it = msg->begin();
		if(msg->size() > 0)
			printf("Removed device %s\n", (*it).to_string());
	}

	return 1;
}
#endif

#if 0
int device_added_removed_cb(const EdbusMessage* msg, void*) {
	EdbusMessage::const_iterator it = msg->begin();
	printf("got member %s\n", msg->member());
	if(msg->size() > 0)
		printf("Added/Removed device %s\n", (*it).to_string());
	return 1;
}
#endif 

int DeviceAdded(const EdbusMessage* msg, void*) {
	EdbusMessage::const_iterator it = msg->begin();
	printf("Added device %s\n", (*it).to_string());
	return 1;
}

int DeviceRemoved(const EdbusMessage* msg, void*) {
	EdbusMessage::const_iterator it = msg->begin();
	printf("Removed device %s\n", (*it).to_string());
	return 1;
}

EdbusCallbackItem signal_table[] = {
	{ "/org/freedesktop/Hal/Manager", "org.freedesktop.Hal.Manager", "DeviceAdded", DeviceAdded, NULL },
	{ "/org/freedesktop/Hal/Manager", "org.freedesktop.Hal.Manager", "DeviceRemoved", DeviceRemoved, NULL },
};

int main() {
	EdbusConnection conn;
	if(!conn.connect(EDBUS_SYSTEM)) {
		puts("No system connection");
		return 1;
	}

	//conn.signal_callback(signal_cb, 0);
	//conn.add_signal_match("/org/freedesktop/Hal/Manager", "org.freedesktop.Hal.Manager", "DeviceAdded");
	//conn.add_signal_match("/org/freedesktop/Hal/Manager", "org.freedesktop.Hal.Manager", "DeviceRemoved");
	//conn.signal_callback(device_added_removed_cb, 0);
	conn.signal_callback_table(signal_table, 2);

	conn.setup_listener();
	while(conn.wait(100)) 
		;
	return 0;
}
