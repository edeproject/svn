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

EdbusDataType EdbusData::type() {
	return impl->type;
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

	return 0;
}
