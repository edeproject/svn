#include <stdio.h>
#include <unistd.h>
#include <FL/Enumerations.h>

#include "EdbusConnection.h"
#include "EdbusDict.h"

int main() {
	EdbusConnection cl;
	if(!cl.connect(EDBUS_SESSION)) {
		puts("Can't connect to session bus");
		return 1;
	}

	EdbusMessage msg;

	msg.create_signal("/org/equinoxproject/Signal", "org.equinoxproject.SignalType", "Signal");
	msg << "Hello";

	cl.send(msg);

	msg.clear();

	puts("Changing color of window to FL_RED");

	msg.create_method_call("org.equinoxproject.server",  "/org/equinoxproject/Object",
			"org.equinoxproject.Type", "ChangeBackground");

	EdbusDict dict;
	dict.append("key1", "val1");
	dict.append("key2", "val2");
	dict.append("key3", "val3");
	dict.append("key4", "val4");

	msg << EdbusData::from_int16(FL_RED) << EdbusData::from_dict(dict);
	cl.send(msg);

	sleep(3);

	puts("Changing color of window to FL_GRAY");

	msg.create_method_call("org.equinoxproject.server",  "/org/equinoxproject/Object",
			"org.equinoxproject.Type", "ChangeBackground");

	msg << EdbusData::from_int16(FL_GRAY);
	cl.send(msg);

	return 0;
}

#if 0
int main() {
	EdbusConnection cl;
	if(!cl.connect(EDBUS_SYSTEM)) {
		puts("No system connection");
		return 1;
	}

	EdbusMessage msg;
	msg.create_method_call("org.freedesktop.Hal", 
			"/org/freedesktop/Hal/devices/computer", "org.freedesktop.DBus.Introspectable", "Introspect");

	EdbusMessage reply;
	cl.send_with_reply_and_block(msg, 1000, reply);

	EdbusMessage::iterator it = reply.begin(), it_end = reply.end();
	while(it != it_end) {
		if((*it).is_string())
			printf("%s", (*it).to_string());
		++it;
	}

	return 0;
}
#endif

#if 0
int main() {
	EdbusVariant v;
	v.value = EdbusData::from_int32(34);

	EdbusData d = EdbusData::from_variant(v);
	printf("%s\n", d.signature());

	EdbusVariant vv = d.to_variant();
	vv.value = EdbusData::from_variant(v);

	d = EdbusData::from_variant(vv);
	printf("%s\n", d.signature());

	printf("%i\n", d.is_variant());

	v = d.to_variant();

	printf("%i\n", v.value.is_variant());

	return 0;
}
#endif

#if 0
int main1() {
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
#endif
