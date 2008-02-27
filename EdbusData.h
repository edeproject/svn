#ifndef __EDBUSDATA_H__
#define __EDBUSDATA_H__

#include <inttypes.h>

typedef char byte_t;

enum EdbusDataType {
	EDBUS_TYPE_INVALID,
	EDBUS_TYPE_BYTE,
	EDBUS_TYPE_BOOL,
	EDBUS_TYPE_INT16,
	EDBUS_TYPE_UINT16,
	EDBUS_TYPE_INT32,
	EDBUS_TYPE_UINT32,
	EDBUS_TYPE_INT64,
	EDBUS_TYPE_UINT64,
	EDBUS_TYPE_DOUBLE,
	EDBUS_TYPE_STRING,
	EDBUS_TYPE_OBJECT_PATH,
	EDBUS_TYPE_SIGNATURE,
	EDBUS_TYPE_ARRAY,
	EDBUS_TYPE_STRUCT,
	EDBUS_TYPE_VARIANT
};

struct EdbusDataPrivate;

class EdbusData {
	private:
		EdbusDataPrivate* impl;
	public:
		EdbusData();
		EdbusData(byte_t val);
		EdbusData(bool val);
		EdbusData(int16_t val);
		EdbusData(uint16_t val);
		EdbusData(int32_t val);
		EdbusData(uint32_t val);
		EdbusData(int64_t val);
		EdbusData(uint64_t val);
		EdbusData(double val);
		EdbusData(const char* val);
		EdbusData(const EdbusData& other); // essential for containers
		~EdbusData();

		EdbusDataType type();
};

#endif
