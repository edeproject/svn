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

		EdbusDataType type() const;

		bool is_byte(void) const { return type() == EDBUS_TYPE_BYTE; }
		bool is_char(void) const { return is_byte(); }
		bool is_bool(void) const { return type() == EDBUS_TYPE_BOOL; }
		bool is_int16(void) const { return type() == EDBUS_TYPE_INT16; }
		bool is_uint16(void) const { return type() == EDBUS_TYPE_UINT16; }
		bool is_int32(void) const { return type() == EDBUS_TYPE_INT32; }
		bool is_uint32(void) const { return type() == EDBUS_TYPE_UINT32; }
		bool is_int64(void) const { return type() == EDBUS_TYPE_INT64; }
		bool is_uint64(void) const { return type() == EDBUS_TYPE_UINT64; }
		bool is_string(void) const { return type() == EDBUS_TYPE_STRING; }
		bool is_object_path(void) const { return type() == EDBUS_TYPE_OBJECT_PATH; }
		bool is_signature(void) const { return type() == EDBUS_TYPE_SIGNATURE; }
		bool is_array(void) const { return type() == EDBUS_TYPE_ARRAY; }
		bool is_struct(void) const { return type() == EDBUS_TYPE_STRUCT; }
		bool is_variant(void) const { return type() == EDBUS_TYPE_VARIANT; }

		bool operator==(const EdbusData& other);
		bool operator!=(const EdbusData& other) { return !operator==(other); }
};

#endif
