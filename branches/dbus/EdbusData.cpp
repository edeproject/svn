#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <list>

#include "EdbusData.h"

using std::list;

struct EdbusDataPrivate {
	EdbusDataType type;

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
	impl->type = EDBUS_TYPE_INVALID;
}

EdbusData::EdbusData(byte_t val) {
	impl = new EdbusDataPrivate;
	impl->type = EDBUS_TYPE_BYTE;
	impl->value.v_byte = val;
}

EdbusData::EdbusData(bool val) {
	impl = new EdbusDataPrivate;
	impl->type = EDBUS_TYPE_BOOL;
	impl->value.v_bool = val;
}

EdbusData::EdbusData(int16_t val) {
	impl = new EdbusDataPrivate;
	impl->type = EDBUS_TYPE_INT16;
	impl->value.v_int16 = val;
}

EdbusData::EdbusData(uint16_t val) {
	impl = new EdbusDataPrivate;
	impl->type = EDBUS_TYPE_UINT16;
	impl->value.v_uint16 = val;
}

EdbusData::EdbusData(int32_t val) {
	impl = new EdbusDataPrivate;
	impl->type = EDBUS_TYPE_INT32;
	impl->value.v_int32 = val;
}

EdbusData::EdbusData(uint32_t val) {
	impl = new EdbusDataPrivate;
	impl->type = EDBUS_TYPE_UINT32;
	impl->value.v_uint32 = val;
}

EdbusData::EdbusData(int64_t val) {
	impl = new EdbusDataPrivate;
	impl->type = EDBUS_TYPE_INT64;
	impl->value.v_int64 = val;
}

EdbusData::EdbusData(uint64_t val) {
	impl = new EdbusDataPrivate;
	impl->type = EDBUS_TYPE_UINT64;
	impl->value.v_uint64 = val;
}

EdbusData::EdbusData(double val) {
	impl = new EdbusDataPrivate;
	impl->type = EDBUS_TYPE_DOUBLE;
	impl->value.v_double = val;
}

EdbusData::EdbusData(const char* val) {
	impl = new EdbusDataPrivate;
	impl->type = EDBUS_TYPE_STRING;
	impl->value.v_pointer = strdup(val);
}

EdbusData::EdbusData(const EdbusData& other) {
	if(this == &other)
		return;
	impl = new EdbusDataPrivate;
	impl->type = other.impl->type;

	if(impl->type != EDBUS_TYPE_STRING)
		impl->value = other.impl->value;
	else
		impl->value.v_pointer = strdup((const char*)other.impl->value.v_pointer);
}

EdbusData::~EdbusData() {
	if(impl->type == EDBUS_TYPE_STRING)
		free(impl->value.v_pointer);
	delete impl;
}

EdbusDataType EdbusData::type() const {
	return impl->type;
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
		case EDBUS_TYPE_INT32:
			return impl->value.v_int32 == other.impl->value.v_int32;
		case EDBUS_TYPE_INT64:
			return impl->value.v_int64 == other.impl->value.v_int64;
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
		case EDBUS_TYPE_SIGNATURE:
		case EDBUS_TYPE_ARRAY:
		case EDBUS_TYPE_STRUCT:
		case EDBUS_TYPE_VARIANT:
			/* TODO */
			return false;
	}

	return false;
}

int main() {
	list<EdbusData> data;
	data.push_back(3);
	data.push_back('c');
	data.push_back(34);
	data.push_back("Some sample demo");
	data.push_back(0.4);
	data.push_back(true);

	list<EdbusData>::iterator first = data.begin(), last = data.end();
	for(; first != last; ++first)
		printf("%i ", (*first).type());
	puts("");

	EdbusData d1(2);
	EdbusData d2(2);
	EdbusData d3(5);
	EdbusData d4("Some sample");
	EdbusData d5("Sample some");

	printf("%i %i %i %i %i\n", d1 == d2, d1 == d2, d1 == d3, d1 == d5, d4 == d5);

	return 0;
}
