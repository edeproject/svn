#ifndef __EDBUSMESSAGE_H__
#define __DEBUSMESSAGE_H__

class  EdbusConnection;
struct EdbusMessageImpl;
struct EdbusMessageIteratorImpl;

struct DBusMessage;

enum EdbusValueType {
	EDBUS_TYPE_INT,
	EDBUS_TYPE_UINT,
	EDBUS_TYPE_LONG,
	EDBUS_TYPE_ULONG,
	EDBUS_TYPE_DOUBLE,
	EDBUS_TYPE_STRING,
	EDBUS_TYPE_INVALID
};

class EdbusMessageIterator {
	private:
		EdbusMessageIteratorImpl* impl;

	public:
		EdbusMessageIterator();
		EdbusMessageIterator(const EdbusMessageIterator& it);
		EdbusMessageIterator(DBusMessage* dm);
		~EdbusMessageIterator();

		EdbusMessageIterator& operator++();
		bool operator==(const EdbusMessageIterator& it);
		bool operator!=(const EdbusMessageIterator& it) { return !operator==(it); }

		EdbusValueType type(void);

		int get_int(void);
		unsigned int get_uint(void);
		char get_char(void);
		bool get_bool(void);
		long get_long(void);
		unsigned long get_ulong(void);
		double get_double(void);
		char* get_string(void);
};

class EdbusMessage {
	private:
		friend class EdbusConnection;

		EdbusMessageImpl* dm;
		DBusMessage* message(void) const;

	public:
		typedef EdbusMessageIterator iterator;

		EdbusMessage();
		EdbusMessage(DBusMessage* m);
		~EdbusMessage();

		void create(DBusMessage* m);

		void create_signal(const char* path, const char* interface, const char* name);
		void create_method_call(const char* service, const char* path, const char* interface, const char* method);
		void create_reply(const EdbusMessage& replying_to);

		void clear(void);

		void path(const char* np);
		const char* path(void) const;

		void interface(const char* ni);
		const char* interface(void) const;

		void destination(const char* nd);
		const char* destination(void);

		void member(const char* nm);
		const char* member(void);

		bool sender(const char* ns);
		const char* sender(void);

		const char* signature(void);

		void append(const char* val);
		void append(bool val);
		void append(char val);
		void append(int val);
		void append(unsigned int val);
		void append(long val);
		void append(unsigned long val);
		void append(double val);

		iterator begin(void) const;
		iterator end(void) const;
};

#endif
