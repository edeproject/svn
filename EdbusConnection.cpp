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
	dc->conn = NULL;
	return true;
}

bool EdbusConnection::send(const EdbusMessage& content) {
	if(!dc || !dc->conn)
		return false;

	bool ret;
	dbus_uint32_t serial;

	if(!dbus_connection_send(dc->conn, content.message(), &serial)) {
		printf("Message sending failed\n");
		ret = false;
	} else
		ret = true;

	dbus_connection_flush(dc->conn);
	return ret;
}

bool EdbusConnection::send_with_reply_and_block(const EdbusMessage& content, int timeout, EdbusMessage& ret) {
	if(!dc || !dc->conn)
		return false;

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

bool EdbusConnection::request_name(const char* name, int mode) {
	if(!dc || !dc->conn)
		return false;

	/* TODO: this should be assertion */
	if(mode < 0)
		return false;

	int flags = 0;
	if(mode & EDBUS_NAME_ALLOW_REPLACE)
		flags |= DBUS_NAME_FLAG_ALLOW_REPLACEMENT;
	if(mode & EDBUS_NAME_REPLACE_EXISTITNG)
		flags |= DBUS_NAME_FLAG_REPLACE_EXISTING;

	DBusError err;
	dbus_error_init(&err);

	dbus_bus_request_name(dc->conn, name, flags, &err);

	if(dbus_error_is_set(&err)) {
		printf("Name request error: %s, %s\n", err.name, err.message);
		dbus_error_free(&err);
		return false;
	}

	return true;
}

const char* EdbusConnection::unique_name(void) {
	if(!dc || !dc->conn)
		return NULL;
	return dbus_bus_get_unique_name(dc->conn);
}

int EdbusConnection::wait(int timout_milliseconds) {
	return dbus_connection_read_write_dispatch(dc->conn, timout_milliseconds);
}

