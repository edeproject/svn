#include "EdbusMessage.h"
#include <dbus/dbus.h>
#include <stdlib.h>
#include <assert.h>

#include <stdio.h>

struct EdbusMessageImpl {
	DBusMessage* msg;
};

struct EdbusMessageIteratorImpl {
	DBusMessageIter* iter;
	DBusMessage* msg;
	bool end;
};

EdbusMessageIterator::EdbusMessageIterator() {
	impl = new EdbusMessageIteratorImpl;
	impl->iter = NULL;
	impl->msg = NULL;
	impl->end = true;
}

EdbusMessageIterator::EdbusMessageIterator(DBusMessage* msg) {
	impl = new EdbusMessageIteratorImpl;
	impl->msg = msg;
	impl->iter = (DBusMessageIter*)malloc(sizeof(DBusMessageIter));

	dbus_message_iter_init(msg, impl->iter);

	if(!impl->iter) {
		// no iterator ???
	}
	impl->end = false;
}

EdbusMessageIterator::~EdbusMessageIterator() {
	free(impl->iter);
	delete impl;
	impl = NULL;
}

EdbusMessageIterator& EdbusMessageIterator::operator++() {
	if(impl->end)
		return *this;

	if(!dbus_message_iter_next(impl->iter))
		impl->end = true;
	return *this;
}

bool EdbusMessageIterator::operator==(const EdbusMessageIterator& other) {
	// TODO: more checks !!!

	if(impl->end == true)
		return true;

	if(impl->end == other.impl->end)
		return true;

	return false;
}

EdbusValueType EdbusMessageIterator::type(void) {
	int ret = dbus_message_iter_get_arg_type(impl->iter);

	switch(ret) {
		case DBUS_TYPE_BOOLEAN:
			return EDBUS_TYPE_BOOL;
		case DBUS_TYPE_INT32:
			return EDBUS_TYPE_INT;
		case DBUS_TYPE_UINT32:
			return EDBUS_TYPE_UINT;
		case DBUS_TYPE_INT64:
			return EDBUS_TYPE_LONG;
		case DBUS_TYPE_UINT64:
			return EDBUS_TYPE_ULONG;
		case DBUS_TYPE_DOUBLE:
			return EDBUS_TYPE_DOUBLE;
		case DBUS_TYPE_STRING:
			return EDBUS_TYPE_STRING;
	}

	return EDBUS_TYPE_INVALID;
}

int EdbusMessageIterator::get_int(void) {
	assert(type() == EDBUS_TYPE_INT);

	dbus_int32_t val;
	dbus_message_iter_get_basic(impl->iter, &val);
	return val;
}

unsigned int EdbusMessageIterator::get_uint(void) {
	assert(type() == EDBUS_TYPE_UINT);

	dbus_uint32_t val;
	dbus_message_iter_get_basic(impl->iter, &val);
	return val;
}

char EdbusMessageIterator::get_char(void) {
	return get_int();
}

bool EdbusMessageIterator::get_bool(void) {
	assert(type() == EDBUS_TYPE_BOOL);

	dbus_bool_t val;
	dbus_message_iter_get_basic(impl->iter, &val);
	return val;
}

long EdbusMessageIterator::get_long(void) {
	assert(type() == EDBUS_TYPE_LONG);

	dbus_int64_t val;
	dbus_message_iter_get_basic(impl->iter, &val);
	return val;
}

unsigned long EdbusMessageIterator::get_ulong(void) {
	assert(type() == EDBUS_TYPE_ULONG);

	dbus_uint64_t val;
	dbus_message_iter_get_basic(impl->iter, &val);
	return val;
}

double EdbusMessageIterator::get_double(void) {
	assert(type() == EDBUS_TYPE_DOUBLE);

	double val;
	dbus_message_iter_get_basic(impl->iter, &val);
	return val;
}

const char* EdbusMessageIterator::get_string(void) {
	const char* v;
	dbus_message_iter_get_basic(impl->iter, &v);
	return v;
}


EdbusMessage::EdbusMessage() : dm(NULL) {
}

EdbusMessage::EdbusMessage(DBusMessage* m) : dm(NULL) {
	create(m);
}

EdbusMessage::~EdbusMessage() {
	if(!dm)
		return;

	clear();
	delete dm;
}

DBusMessage* EdbusMessage::message(void) const {
	if(!dm)
		return NULL;
	return dm->msg;
}

#define CREATE_OR_CLEAR(m)                      \
do {                                            \
	if(!m) {                                    \
		m = new EdbusMessageImpl;               \
		m->msg = NULL;                          \
	} else {                                    \
		/* destroy previously create message */ \
		clear();                                \
	}                                           \
} while(0)


void EdbusMessage::create(DBusMessage* m) {
	CREATE_OR_CLEAR(dm);
	dm->msg = m;
	/* increase counter or libdbus will scream with assertion */
	dm->msg = dbus_message_ref(dm->msg);
}

void EdbusMessage::create_signal(const char* path, const char* interface, const char* name) {
	CREATE_OR_CLEAR(dm);
	dm->msg = dbus_message_new_signal(path, interface, name);
}

void EdbusMessage::create_method_call(const char* service, const char* path, const char* interface, const char* method) {
	CREATE_OR_CLEAR(dm);
	dm->msg = dbus_message_new_method_call(service, path, interface, method);
}

void EdbusMessage::create_reply(const EdbusMessage& replying_to) {
	CREATE_OR_CLEAR(dm);
	dm->msg = dbus_message_new_method_return(replying_to.dm->msg);
}

void EdbusMessage::create_error_reply(const EdbusMessage& replying_to, const char* errmsg) {
	CREATE_OR_CLEAR(dm);
	dm->msg = dbus_message_new_error(replying_to.dm->msg, DBUS_ERROR_FAILED, errmsg);
}

void EdbusMessage::clear(void) {
	if(!dm)
		return;

	if(dm->msg != NULL) {
		dbus_message_unref(dm->msg);
		dm->msg = NULL;
	}
}

void EdbusMessage::path(const char* np) {
	dbus_message_set_path(dm->msg, np);
}

const char* EdbusMessage::path(void) const {
	return dbus_message_get_path(dm->msg);
}

void EdbusMessage::interface(const char* ni) {
	dbus_message_set_interface(dm->msg, ni);
}

const char* EdbusMessage::interface(void) const {
	return dbus_message_get_interface(dm->msg);
}

void EdbusMessage::destination(const char* nd) {
	dbus_message_set_destination(dm->msg, nd);
}

const char* EdbusMessage::destination(void) const {
	return dbus_message_get_destination(dm->msg);
}

void EdbusMessage::member(const char* nm) {
	dbus_message_set_member(dm->msg, nm);
}

const char* EdbusMessage::member(void) const {
	return dbus_message_get_member(dm->msg);
}

bool EdbusMessage::sender(const char* ns) {
	dbus_message_set_sender(dm->msg, ns);
}

const char* EdbusMessage::sender(void) const {
	dbus_message_get_sender(dm->msg);
}

const char* EdbusMessage::signature(void) const {
	return dbus_message_get_signature(dm->msg);
}

void EdbusMessage::append(const char* val) {
	dbus_message_append_args(dm->msg, DBUS_TYPE_STRING, &val, DBUS_TYPE_INVALID);
}

void EdbusMessage::append(bool val) {
	if(!dm)
		return;

	const dbus_bool_t bval = val; /* so DBus knows real size */
	dbus_message_append_args(dm->msg, DBUS_TYPE_BOOLEAN, &bval, DBUS_TYPE_INVALID);
}

void EdbusMessage::append(char val) {
	dbus_message_append_args(dm->msg, DBUS_TYPE_BYTE, &val, DBUS_TYPE_INVALID);
}

void EdbusMessage::append(int val) {
	dbus_message_append_args(dm->msg, DBUS_TYPE_INT32, &val, DBUS_TYPE_INVALID);
}

void EdbusMessage::append(unsigned int val) {
	dbus_message_append_args(dm->msg, DBUS_TYPE_UINT32, &val, DBUS_TYPE_INVALID);
}

void EdbusMessage::append(long val) {
	dbus_message_append_args(dm->msg, DBUS_TYPE_INT64, &val, DBUS_TYPE_INVALID);
}

void EdbusMessage::append(unsigned long val) {
	dbus_message_append_args(dm->msg, DBUS_TYPE_UINT64, &val, DBUS_TYPE_INVALID);
}

void EdbusMessage::append(double val) {
	dbus_message_append_args(dm->msg, DBUS_TYPE_DOUBLE, &val, DBUS_TYPE_INVALID);
}

EdbusMessage::iterator EdbusMessage::begin(void) const {
	return iterator(dm->msg);
}

EdbusMessage::iterator EdbusMessage::end(void) const {
	return iterator();
}

