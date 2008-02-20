#include "EdbusConnection.h"
#include <stdio.h>

int main2() {
	EdbusConnection cl;
	if(!cl.connect(EDBUS_SYSTEM)) {
		puts("No system connection");
		return 1;
	}

		//void create_method_call(const char* service, const char* path, const char* interface, const char* method);
	EdbusMessage msg;
	msg.create_method_call("org.freedesktop.Hal", 
			"/org/freedesktop/Hal/devices/computer", "org.freedesktop.DBus.Introspectable", "Introspect");
			//"org.freedesktop.DBus.Introspectable.Introspect");

	EdbusMessage reply;
	cl.send_with_reply_and_block(msg, 1000, reply);
	printf("Got signature: %s\n", reply.signature());

	EdbusMessage::iterator it = reply.begin(), it_end = reply.end();
	while(it != it_end) {
		if(it.type() == EDBUS_TYPE_STRING)
			printf("got string: %s", it.get_string());
		++it;
	}

	return 0;
}

int main() {
	EdbusConnection client;
	if(!client.connect(EDBUS_SESSION)) {
		puts("Can't connect to session bus");
		return 1;
	}

	printf("Unique name: %s\n", client.unique_name());


	if(!client.request_name("org.equinoxproject.server")) {
		puts("Can't set name");
		return 1;
	}

	//client.send_signal("/test/signal/Object", "test.signal.Type", "Test", "bla");
#if 0
	EdbusMessage msg;
	msg.create_signal("/test/signal/Object", "test.signal.Type", "Test");
	msg.append("xxx xxx xxx");
	msg.append(66);
	msg.append(66);
	msg.append(66);

	EdbusMessage::iterator it = msg.begin(), it_end = msg.end();
	while(it != it_end) {
		if(it.type() == EDBUS_TYPE_INT)
			printf("%i\n", it.get_int());
		++it;
	}

	client.send_signal(msg);
#endif
	EdbusMessage msg;
	msg.create_method_call("test.method.server", "/test/method/Object", "test.method.Type", "Method");
	msg.append("foo");
	msg.append(34);

	printf("%s %s %s %s\n", msg.interface(), msg.path(), msg.member(), msg.signature());

	EdbusMessage ret;

	/*
	client.send(msg);
	while(client.wait(3000))
		puts("tick");
	*/

	if(!client.send_with_reply_and_block(msg, -1, ret)) {
		puts("No reply");
		return 1;
	}

	printf("reply signature: %s\n", ret.signature());

	EdbusMessage::iterator it = ret.begin(), it_end = ret.end();
	while(it != it_end) {
		if(it.type() == EDBUS_TYPE_INT)
			printf("Got: %i\n", it.get_int());
		if(it.type() == EDBUS_TYPE_UINT)
			printf("Got: %i\n", it.get_uint());
		if(it.type() == EDBUS_TYPE_BOOL)
			printf("Got bool: %i\n", it.get_bool());

		++it;
	}

	return 0;
}
