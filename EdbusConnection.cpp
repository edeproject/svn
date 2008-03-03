#include <stdio.h>
#include <assert.h>
#include <list>

#include <FL/Fl.h>
#include <dbus/dbus.h>

#include "EdbusConnection.h"

typedef int (*EdbusCallback)(const EdbusMessage*, void*);
typedef std::list<DBusWatch*> WatchList;
typedef std::list<DBusWatch*>::iterator WatchListIter;

struct EdbusConnImpl {
	DBusConnection* conn;
	bool            is_shared;

	EdbusCallback   signal_cb;
	void*           signal_cb_data;

	EdbusCallback   method_call_cb;
	void*           method_call_cb_data;

	/* 
	 * only used when setup_listener_with_fltk() is called
	 * TODO: can WatchList be replaced with DBusWatchList ?
	 */
	WatchList*      watch_list;
	DBusTimeout*    timeout;
};

static DBusHandlerResult edbus_signal_filter(DBusConnection* connection, DBusMessage* msg, void* data) {
	assert(data != NULL);
	assert(msg != NULL);

	// TODO: when connection is not available, DBUS_HANDLER_RESULT_NOT_YET_HANDLED should be returned
	
	EdbusConnImpl* dc = (EdbusConnImpl*)data;
	
	int mtype = dbus_message_get_type(msg);
	int ret = 0;

	if(mtype == DBUS_MESSAGE_TYPE_SIGNAL) {
		if(dc->signal_cb) {
			EdbusMessage m(msg);
			ret = dc->signal_cb(&m, dc->signal_cb_data);
		}
	} else if(mtype == DBUS_MESSAGE_TYPE_METHOD_CALL) {
		if(dc->method_call_cb) {
			EdbusMessage m(msg);
			ret = dc->method_call_cb(&m, dc->method_call_cb_data);
		}
	}

	return ((ret > 0) ? DBUS_HANDLER_RESULT_HANDLED : DBUS_HANDLER_RESULT_NOT_YET_HANDLED);
}

static void dispatch_cb(void* d) {
	EdbusConnImpl* dc = (EdbusConnImpl*)d;
	assert(dc != NULL);

	puts("dispatch_cb");

	while(dbus_connection_dispatch(dc->conn) == DBUS_DISPATCH_DATA_REMAINS)
		;

	Fl::remove_timeout(dispatch_cb);
}

static void read_watch_cb(int fd, void* d) { 
	puts("read_watch_cb");

	EdbusConnImpl* dc = (EdbusConnImpl*)d;
	assert(dc != NULL);
	assert(dc->watch_list != NULL);

	WatchListIter it = dc->watch_list->begin(), it_end = dc->watch_list->end();
	while(it != it_end) {
		if(dbus_watch_get_fd(*it) == fd && dbus_watch_get_enabled(*it)) {
			puts("aaa");
			if(!dbus_watch_handle(*it, DBUS_WATCH_READABLE))
				puts("Out of memory");
			break;
		}
		++it;
	}

	/*
	 * Check if there are more incomming data and process them. Note that 
	 * dbus_connection_dispatch() call will also remove data from queue. 
	 * This means that (here) timer will not be installed if only one unprocessed
	 * message is in queue; opposite, after installment it will process the rest
	 * of the messages without interrupting read_watch_cb() flow.
	 *
	 * If this is not set (e.g. all data are processed here), we can miss initial
	 * (or later) messages that are sent to us. Also, timer will be triggered faster
	 * as it can (seems that 0.5 as timer value misses some data...).
	 */
	if(dbus_connection_dispatch(dc->conn) == DBUS_DISPATCH_DATA_REMAINS)
		Fl::add_timeout(0.2, dispatch_cb, dc);
}

static void write_watch_cb(int fd, void* d) { 
	puts("write_watch_cb");

	EdbusConnImpl* dc = (EdbusConnImpl*)d;
	assert(dc != NULL);
	assert(dc->watch_list != NULL);

	WatchListIter it = dc->watch_list->begin(), it_end = dc->watch_list->end();
	while(it != it_end) { 
		if(dbus_watch_get_fd(*it) == fd && dbus_watch_get_enabled(*it)) {
			if(!dbus_watch_handle(*it, DBUS_WATCH_WRITABLE))
				puts("Out of memory");
			break;
		}
		++it;
	}
}

static void timeout_cb(void* d) {
	puts("timeout_cb");

	EdbusConnImpl* dc = (EdbusConnImpl*)d;
	assert(dc != NULL);
	assert(dc->timeout != NULL);

	dbus_timeout_handle(dc->timeout);
}

static dbus_bool_t edbus_add_watch(DBusWatch* watch, void* d) {
	assert(watch != NULL);

	/* 
	 * check if watch is enabled since dbus_watch_handle() can't be used
	 * on as connection will not be ready to handle this watch yet
	 */
 	if(!dbus_watch_get_enabled(watch))
		return 1;

	EdbusConnImpl* dc = (EdbusConnImpl*)d;
	assert(dc != NULL);
	assert(dc->watch_list != NULL);

	int fd = dbus_watch_get_fd(watch);
	int flags = dbus_watch_get_flags(watch);

	dc->watch_list->push_back(watch);

	if(flags & DBUS_WATCH_READABLE)
		Fl::add_fd(fd, FL_READ, read_watch_cb, d);

	if(flags & DBUS_WATCH_WRITABLE)
		Fl::add_fd(fd, FL_WRITE, write_watch_cb, d);

	return 1;
}

static void edbus_remove_watch(DBusWatch* watch, void* d) {
	assert(watch != NULL);

	EdbusConnImpl* dc = (EdbusConnImpl*)d;
	assert(dc != NULL);
	assert(dc->watch_list != NULL);

	puts("removing watch");

	int fd = dbus_watch_get_fd(watch);
	int flags = dbus_watch_get_flags(watch);

	if(flags & DBUS_WATCH_READABLE)
		Fl::remove_fd(fd, FL_READ);

	if(flags & DBUS_WATCH_WRITABLE)
		Fl::remove_fd(fd, FL_WRITE);

	WatchListIter it = dc->watch_list->begin(), it_end = dc->watch_list->end();
	while(it != it_end) {
		if(*it == watch) {
			dc->watch_list->erase(it);
			break;
		}
		++it;
	}
}

static void edbus_toggle_watch(DBusWatch* watch, void* data) {
	assert(watch != NULL);

	if(dbus_watch_get_enabled(watch))
		edbus_add_watch(watch, data);
	else
		edbus_remove_watch(watch, data);
}

static dbus_bool_t edbus_add_timeout(DBusTimeout* timeout, void* data) {
	assert(timeout != NULL);

	/* D-Bus interval sees in miliseconds, but FLTK see it in seconds */
	int interval = dbus_timeout_get_interval(timeout);

	printf("added timeout to %i ms\n", interval);
	Fl::add_timeout(interval / 1000, timeout_cb, data);
	return 1;
}

static void edbus_remove_timeout(DBusTimeout* timeout, void* data) {
	assert(timeout != NULL);
	Fl::remove_timeout(timeout_cb);
}

static void edbus_toggle_timeout(DBusTimeout* timeout, void* data) {
	assert(timeout != NULL);

	if(dbus_timeout_get_enabled(timeout))
		edbus_add_timeout(timeout, data);
	else
		edbus_remove_timeout(timeout, data);
}

static void edbus_wakeup_main(void*) {
	/* Not used */
}


EdbusConnection::EdbusConnection() : dc(NULL) {
}

EdbusConnection::~EdbusConnection() {
	disconnect();
	delete dc;
}

void EdbusConnection::setup_filter(void) {
	assert(dc != NULL);
	assert(dc->conn != NULL);

	DBusError err;
	dbus_error_init(&err);

	dbus_bus_add_match(dc->conn, "type='signal'", &err);

	if(dbus_error_is_set(&err)) {
		printf("Signal match failed: %s, %s\n", err.name, err.message);
		dbus_error_free(&err);
		return;
	}

	const char* name = unique_name();
	if(name) {
		/* TODO: edelib:String here */
		char buff[1024];
		sprintf(buff, "destination='%s'", name);

		dbus_bus_add_match(dc->conn, buff, &err);

		if(dbus_error_is_set(&err)) {
			printf("Destination match failed: %s, %s\n", err.name, err.message);
			dbus_error_free(&err);
			return;
		}
	} else
		puts("Unable to get unique name");

	dbus_connection_add_filter(dc->conn, edbus_signal_filter, dc, 0);
}

bool EdbusConnection::connect(EdbusConnectionType ctype) {
	if(dc == NULL) {
		dc = new EdbusConnImpl;
		dc->conn = NULL;
		dc->is_shared = false;

		dc->signal_cb = NULL;
		dc->signal_cb_data = NULL;

		dc->method_call_cb = NULL;
		dc->method_call_cb_data = NULL;

		dc->watch_list = NULL;
		dc->timeout = NULL;
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
	if(dc->conn)
		dbus_connection_unref(dc->conn);

	dc->conn = NULL;
	dc->is_shared = false;

	dc->signal_cb = NULL;
	dc->signal_cb_data = NULL;

	dc->method_call_cb = NULL;
	dc->method_call_cb_data = NULL;

	/* remove all FLTK notifiers so we can reuse EdbusConnection object again */
	if(dc->watch_list) {
		int fd;
		WatchListIter it = dc->watch_list->begin(), it_end = dc->watch_list->end();

		while(it != it_end) {
			fd = dbus_watch_get_fd(*it);
			Fl::remove_fd(fd);
			++it;
		}

		dc->watch_list->clear();
		delete dc->watch_list;
		dc->watch_list = NULL;
	}

	if(dc->timeout) {
		Fl::remove_timeout(timeout_cb);
		dc->timeout = NULL;
	}

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

bool EdbusConnection::send_with_reply_and_block(const EdbusMessage& content, int timeout_ms, EdbusMessage& ret) {
	if(!dc || !dc->conn)
		return false;

	DBusMessage* reply;
	DBusError err;

	dbus_error_init(&err);

	reply = dbus_connection_send_with_reply_and_block(dc->conn, content.message(), timeout_ms, &err);

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

void EdbusConnection::signal_callback(int (*cb)(const EdbusMessage*, void*), void* data) {
	if(!dc || !dc->conn)
		return;

	dc->signal_cb = cb;
	dc->signal_cb_data = data;
}

void EdbusConnection::method_callback(int (*cb)(const EdbusMessage*, void*), void* data) {
	if(!dc || !dc->conn)
		return;

	dc->method_call_cb = cb;
	dc->method_call_cb_data = data;
}

void EdbusConnection::setup_listener_with_fltk(void) {
	if(!dc || !dc->conn)
		return;

	setup_filter();

	/* allocate our watch list */
	dc->watch_list = new WatchList;

	dbus_connection_set_watch_functions(dc->conn, edbus_add_watch, edbus_remove_watch, edbus_toggle_watch, dc, 0);
	dbus_connection_set_timeout_functions(dc->conn, edbus_add_timeout, edbus_remove_timeout, 
			edbus_toggle_timeout, dc, 0);
	dbus_connection_set_wakeup_main_function(dc->conn, edbus_wakeup_main, 0, 0);
}

void EdbusConnection::setup_listener(void) {
	if(!dc || !dc->conn)
		return;

	setup_filter();
}

int EdbusConnection::wait(int timout_ms) {
	if(!dc || !dc->conn)
		return 0;

	return dbus_connection_read_write_dispatch(dc->conn, timout_ms);
}
