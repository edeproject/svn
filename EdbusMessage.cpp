#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <dbus/dbus.h>

#include "EdbusMessage.h"

struct EdbusMessageImpl {
	DBusMessage* msg;
};

struct EdbusMessageIteratorImpl {
	DBusMessageIter* iter;
	DBusMessage*     msg;
	EdbusData        holder;
	bool             end;
};

static void to_dbus_type(DBusMessage* msg, const EdbusData& data) {
	if(data.is_bool()) {
		const dbus_bool_t v = data.to_bool(); /* so DBus knows real size */
		dbus_message_append_args(msg, DBUS_TYPE_BOOLEAN, &v, DBUS_TYPE_INVALID);
	} else if(data.is_byte()) {
		byte_t v = data.to_byte();
		dbus_message_append_args(msg, DBUS_TYPE_BYTE, &v, DBUS_TYPE_INVALID);
	} else if(data.is_int16()) {
		int16_t v = data.to_int16();
		dbus_message_append_args(msg, DBUS_TYPE_INT16, &v, DBUS_TYPE_INVALID);
	} else if(data.is_uint16()) {
		uint16_t v = data.to_uint16();
		dbus_message_append_args(msg, DBUS_TYPE_UINT16, &v, DBUS_TYPE_INVALID);
	} else if(data.is_int32()) {
		int32_t v = data.to_int32();
		dbus_message_append_args(msg, DBUS_TYPE_INT32, &v, DBUS_TYPE_INVALID);
	} else if(data.is_uint32()) {
		uint32_t v = data.to_uint32();
		dbus_message_append_args(msg, DBUS_TYPE_UINT32, &v, DBUS_TYPE_INVALID);
	} else if(data.is_int64()) {
		int64_t v = data.to_int64();
		dbus_message_append_args(msg, DBUS_TYPE_INT64, &v, DBUS_TYPE_INVALID);
	} else if(data.is_uint64()) {
		uint64_t v = data.to_uint64();
		dbus_message_append_args(msg, DBUS_TYPE_UINT64, &v, DBUS_TYPE_INVALID);
	} else if(data.is_double()) {
		double v = data.to_double();
		dbus_message_append_args(msg, DBUS_TYPE_DOUBLE, &v, DBUS_TYPE_INVALID);
	}
}

static void from_dbus_type(DBusMessageIter* iter, EdbusData& data) {
	int dtype = dbus_message_iter_get_arg_type(iter);

	if(dtype == DBUS_TYPE_BOOLEAN) {
		dbus_bool_t v;
		dbus_message_iter_get_basic(iter, &v);
		data = EdbusData::from_bool(v);
	} else if(dtype == DBUS_TYPE_BYTE) {
		/* DBus does not have dbus_byte_t so use our definition */
		byte_t v;
		dbus_message_iter_get_basic(iter, &v);
		data = EdbusData::from_byte(v);
	} else if(dtype == DBUS_TYPE_INT16) {
		dbus_int16_t v;
		dbus_message_iter_get_basic(iter, &v);
		data = EdbusData::from_int16(v);
	} else if(dtype == DBUS_TYPE_UINT16) {
		dbus_uint16_t v;
		dbus_message_iter_get_basic(iter, &v);
		data = EdbusData::from_uint16(v);
	} else if(dtype == DBUS_TYPE_INT32) {
		dbus_int32_t v;
		dbus_message_iter_get_basic(iter, &v);
		data = EdbusData::from_int32(v);
	} else if(dtype == DBUS_TYPE_UINT32) {
		dbus_uint32_t v;
		dbus_message_iter_get_basic(iter, &v);
		data = EdbusData::from_uint32(v);
	} else if(dtype == DBUS_TYPE_INT64) {
		dbus_int64_t v;
		dbus_message_iter_get_basic(iter, &v);
		data = EdbusData::from_int64(v);
	} else if(dtype == DBUS_TYPE_UINT64) {
		dbus_uint64_t v;
		dbus_message_iter_get_basic(iter, &v);
		data = EdbusData::from_uint64(v);
	} else if(dtype == DBUS_TYPE_STRING) {
		const char* v;
		dbus_message_iter_get_basic(iter, &v);
		data = EdbusData::from_string(v);
	}
}

EdbusMessageIterator::EdbusMessageIterator() {
	impl = new EdbusMessageIteratorImpl;
	impl->iter = NULL;
	impl->msg = NULL;
	impl->holder = EdbusData::from_invalid();
	impl->end = true;
}

EdbusMessageIterator::EdbusMessageIterator(DBusMessage* msg) {
	impl = new EdbusMessageIteratorImpl;
	impl->msg = msg;
	impl->iter = (DBusMessageIter*)malloc(sizeof(DBusMessageIter));

	dbus_message_iter_init(msg, impl->iter);

	if(!impl->iter) {
		puts("No iterator???");
		impl->end = true;
	} else 
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
	if(impl->end == true)
		return true;

	if(impl->end == other.impl->end)
		return true;

	return false;
}

const EdbusData& EdbusMessageIterator::operator*(void) const {
	if(impl->iter)
		from_dbus_type(impl->iter, impl->holder);

	/* will return invalid anyway if dbus_message_iter_init() failed */
	return impl->holder;
}


EdbusMessage::EdbusMessage() : dm(NULL) {
}

EdbusMessage::EdbusMessage(DBusMessage* m) : dm(NULL) {
	create(m);
}

EdbusMessage::EdbusMessage(const EdbusMessage& m) {
	create(m.message());
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

bool EdbusMessage::is_signal(void) {
	if(!dm)
		return false;
	return dbus_message_is_signal(dm->msg, interface(), member());
}

bool EdbusMessage::is_method_call(void) {
	if(!dm)
		return false;
	return dbus_message_is_method_call(dm->msg, interface(), member());
}

bool EdbusMessage::is_error_reply(const char* errmsg) {
	if(!dm)
		return false;
	return dbus_message_is_error(dm->msg, errmsg);
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

void EdbusMessage::sender(const char* ns) {
	dbus_message_set_sender(dm->msg, ns);
}

const char* EdbusMessage::sender(void) const {
	return dbus_message_get_sender(dm->msg);
}

const char* EdbusMessage::signature(void) const {
	return dbus_message_get_signature(dm->msg);
}

void EdbusMessage::append(const EdbusData& data) {
	/* TODO assert check */
	if(!dm)
		return;

	/* convert it to DBus type */
	to_dbus_type(dm->msg, data);
}

EdbusMessage::iterator EdbusMessage::begin(void) {
	if(!dm)
		return iterator();
	else
		return iterator(dm->msg);
}

EdbusMessage::iterator EdbusMessage::end(void) {
	return iterator();
}

unsigned int EdbusMessage::size(void) {
	EdbusMessage::iterator it = begin(), it_end = end();
	unsigned int ret = 0;

	while(it != it_end) {
		++ret;
		++it;
	}

	return ret;
}

