#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "EdbusData.h"

struct EdbusDataPrivate {
	uint32_t      refs;
	EdbusDataType type;
	char*         sig;   /* DBUS signature */

	union {
		bool     v_bool;
		byte_t   v_byte;
		int16_t  v_int16;
		uint16_t v_uint16;
		int32_t  v_int32;
		uint32_t v_uint32;
		int64_t  v_int64;
		uint64_t v_uint64;
		double   v_double;
		void*    v_pointer;
	} value;
};

EdbusData::EdbusData() {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_INVALID;
	impl->sig = NULL;
}

EdbusData::EdbusData(byte_t val) {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_BYTE;
	impl->sig = strdup("y");
	impl->value.v_byte = val;
}

EdbusData::EdbusData(bool val) {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_BOOL;
	impl->sig = strdup("b");
	impl->value.v_bool = val;
}

EdbusData::EdbusData(int16_t val) {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_INT16;
	impl->sig = strdup("n");
	impl->value.v_int16 = val;
}

EdbusData::EdbusData(uint16_t val) {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_UINT16;
	impl->sig = strdup("q");
	impl->value.v_uint16 = val;
}

EdbusData::EdbusData(int32_t val) {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_INT32;
	impl->sig = strdup("i");
	impl->value.v_int32 = val;
}

EdbusData::EdbusData(uint32_t val) {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_UINT32;
	impl->sig = strdup("u");
	impl->value.v_uint32 = val;
}

EdbusData::EdbusData(int64_t val) {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_INT64;
	impl->sig = strdup("x");
	impl->value.v_int64 = val;
}

EdbusData::EdbusData(uint64_t val) {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_UINT64;
	impl->sig = strdup("t");
	impl->value.v_uint64 = val;
}

EdbusData::EdbusData(double val) {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_DOUBLE;
	impl->sig = strdup("d");
	impl->value.v_double = val;
}

EdbusData::EdbusData(const char* val) {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_STRING;
	impl->sig = strdup("s");
	impl->value.v_pointer = strdup(val);
}

EdbusData::EdbusData(const EdbusObjectPath& val) {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_OBJECT_PATH;
	impl->sig = strdup("o");
	impl->value.v_pointer = strdup(val.path());
}

EdbusData::EdbusData(const EdbusVariant& val) {
	impl = new EdbusDataPrivate;
	impl->refs = 1;
	impl->type = EDBUS_TYPE_VARIANT;

	/*
	 * variant holds 'v' + OTHER_TYPE_SIGNATURE 
	 * I'm not sure can variant hold invalid type so I will
	 * (at least temporary) assert on this
	 */
	const char* s = val.value.signature();
	assert(s != NULL && "Variant on invalid type is not supported yet");

	unsigned int len = strlen(s);
	char* varint_sig = (char*)malloc(len + 2);
	sprintf(varint_sig, "v%s", s);

	impl->sig = varint_sig;
	impl->value.v_pointer = new EdbusVariant(val);
}

EdbusData::EdbusData(const EdbusData& other) {
	if(this == &other)
		return;

	impl = other.impl;
	other.impl->refs++;
}

EdbusData::~EdbusData() {
	if(!impl)
		return;

	impl->refs--;

	if(impl->refs == 0)
		dispose();
}

void EdbusData::dispose(void) {
	if(impl->type == EDBUS_TYPE_STRING || impl->type == EDBUS_TYPE_OBJECT_PATH)
		free(impl->value.v_pointer);
	else if(impl->type == EDBUS_TYPE_VARIANT) {
		/* variant uses new operator */
		EdbusVariant* v = (EdbusVariant*)impl->value.v_pointer;
		delete v;
	}

	if(impl->sig)
		free(impl->sig);

	delete impl;
	impl = 0;
}

EdbusData& EdbusData::operator=(const EdbusData& other) {
	other.impl->refs++;
	impl->refs--;

	if(impl->refs == 0)
		dispose();

	impl = other.impl;
	return *this;
}

EdbusDataType EdbusData::type() const {
	return impl->type;
}

const char* EdbusData::signature(void) const {
	return impl->sig;
}

byte_t EdbusData::to_byte(void) const {
	assert(is_byte() == true);
	return impl->value.v_byte;
}

bool EdbusData::to_bool(void) const {
	assert(is_bool() == true);
	return impl->value.v_bool;
}

int16_t EdbusData::to_int16(void) const {
	assert(is_int16() == true);
	return impl->value.v_int16;
}

uint16_t EdbusData::to_uint16(void) const {
	assert(is_uint16() == true);
	return impl->value.v_uint16;
}

int32_t EdbusData::to_int32(void) const {
	assert(is_int32() == true);
	return impl->value.v_int32;
}

uint32_t EdbusData::to_uint32(void) const {
	assert(is_uint32() == true);
	return impl->value.v_uint32;
}

int64_t EdbusData::to_int64(void) const {
	assert(is_int64() == true);
	return impl->value.v_int64;
}

uint64_t EdbusData::to_uint64(void) const {
	assert(is_uint64() == true);
	return impl->value.v_uint64;
}

double EdbusData::to_double(void) const {
	assert(is_uint64() == true);
	return impl->value.v_double;
}

const char* EdbusData::to_string(void) const {
	assert(is_string() == true);
	return (const char*)impl->value.v_pointer;
}

EdbusObjectPath EdbusData::to_object_path(void) const {
	assert(is_object_path() == true);
	return EdbusObjectPath((const char*)impl->value.v_pointer);
}

EdbusVariant EdbusData::to_variant(void) const {
	assert(is_variant() == true);
	/* copy variant */
	return EdbusVariant((*(EdbusVariant*)impl->value.v_pointer));
}

bool EdbusData::operator==(const EdbusData& other) {
	if(&other == this)
		return true;

	if(type() != other.type())
		return false;

	switch(type()) {
		case EDBUS_TYPE_INVALID:
			return true;
		case EDBUS_TYPE_BYTE:
			return impl->value.v_byte == other.impl->value.v_byte;
		case EDBUS_TYPE_BOOL:
			return impl->value.v_bool == other.impl->value.v_bool;
		case EDBUS_TYPE_INT16:
			return impl->value.v_int16 == other.impl->value.v_int16;
		case EDBUS_TYPE_UINT16:
			return impl->value.v_uint16 == other.impl->value.v_uint16;
		case EDBUS_TYPE_INT32:
			return impl->value.v_int32 == other.impl->value.v_int32;
		case EDBUS_TYPE_UINT32:
			return impl->value.v_uint32 == other.impl->value.v_uint32;
		case EDBUS_TYPE_INT64:
			return impl->value.v_int64 == other.impl->value.v_int64;
		case EDBUS_TYPE_UINT64:
			return impl->value.v_uint64 == other.impl->value.v_uint64;
		case EDBUS_TYPE_DOUBLE:
			return impl->value.v_double == other.impl->value.v_double;
		case EDBUS_TYPE_STRING:
			/* TODO: use edelib::String here */
			if(impl->value.v_pointer && other.impl->value.v_pointer) {
				const char* v1 = (const char*)impl->value.v_pointer;
				const char* v2 = (const char*)other.impl->value.v_pointer;
				return (strcmp(v1, v2) == 0);
			} else
				return false;
		case EDBUS_TYPE_OBJECT_PATH:
		case EDBUS_TYPE_ARRAY:
		case EDBUS_TYPE_STRUCT:
		case EDBUS_TYPE_DICT:
		case EDBUS_TYPE_VARIANT:
			/* TODO */
			return false;
	}

	return false;
}