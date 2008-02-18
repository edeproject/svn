#include "EdbusConnection.h"
#include <dbus/dbus.h>
#include <stdio.h>

struct EdbusConnImpl {
	DBusConnection* conn;
	bool is_shared;
};

EdbusConnection::EdbusConnection() : dc(NULL) {
}

EdbusConnection::~EdbusConnection() {
	disconnect();
	delete dc;
}

bool EdbusConnection::connect(EdbusConnectionType ctype) {
	if(dc == NULL) {
		dc = new EdbusConnImpl;
		dc->conn = NULL;
		dc->is_shared = false;
	}

	DBusBusType type;
	DBusError err;
	dbus_error_init(&err);

	switch(ctype) {
		case EDBUS_SYSTEM:
			type = DBUS_BUS_SYSTEM;
			break;
		case EDBUS_SESSION:
		default:
			type = DBUS_BUS_SESSION;
			break;
	}

	dc->conn = dbus_bus_get(type, &err);
	dc->is_shared = true;

	if(dbus_error_is_set(&err)) {
		printf("Connection error: %s\n", err.message);
		dbus_error_free(&err);
	}

	if(dc->conn == NULL)
		return false;

	return true;
}

bool EdbusConnection::disconnect(void) {
	/* only non-shared connections are allowed to be closed */
	if(!dc->is_shared && dc->conn)
		dbus_connection_close(dc->conn); 
	return true;
}

bool EdbusConnection::send_signal(const char* path, const char* interface, const char* name, const char* content) {
	DBusMessage* msg;
	bool ret;
	dbus_uint32_t serial;

	msg = dbus_message_new_signal(path, interface, name);
	dbus_message_append_args(msg, DBUS_TYPE_STRING, &content, DBUS_TYPE_INVALID);

	int i = 555;
	dbus_message_append_args(msg, DBUS_TYPE_INT32, &i, DBUS_TYPE_INVALID);

	if(!dbus_connection_send(dc->conn, msg, &serial)) {
		printf("Failed signal sending\n");
		ret = false;
	} else
		ret = true;

	dbus_message_unref(msg);
	return ret;
}

bool EdbusConnection::send(const EdbusMessage& content) {
	bool ret;
	dbus_uint32_t serial;

	if(!dbus_connection_send(dc->conn, content.message(), &serial)) {
		printf("Failed signal sending\n");
		ret = false;
	} else
		ret = true;

	dbus_connection_flush(dc->conn);
	return ret;
}

bool EdbusConnection::send_with_reply_and_block(const EdbusMessage& content, int timeout, EdbusMessage& ret) {
	DBusMessage* reply;
	DBusError err;

	dbus_error_init(&err);

	reply = dbus_connection_send_with_reply_and_block(dc->conn, content.message(), timeout, &err);

	if(dbus_error_is_set(&err)) {
		printf("Sending error: %s, %s\n", err.name, err.message);
		dbus_error_free(&err);
		return false;
	}

	ret.create(reply);
	return true;
}

int EdbusConnection::wait(int timout_milliseconds) {
	return dbus_connection_read_write_dispatch(dc->conn, timout_milliseconds);
}

int main() {
	EdbusConnection client;
	if(!client.connect(EDBUS_SESSION)) {
		puts("Can't connect to session bus");
		return false;
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
/*
	if(client.send_with_reply_and_block(msg, 20, ret))
		puts("Got reply");
*/

	return 0;
}
