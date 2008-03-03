#ifndef __EDBUSMESSAGE_H__
#define __DEBUSMESSAGE_H__

#include "EdbusData.h"

class  EdbusConnection;
struct EdbusMessageImpl;
struct EdbusMessageIteratorImpl;
struct DBusMessage;

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
		const EdbusData& operator*(void) const;
};

class EdbusMessage {
	public:
		typedef EdbusMessageIterator iterator;

	private:
		friend class EdbusConnection;
		EdbusMessageImpl* dm;
		DBusMessage* message(void) const;

		EdbusMessage& operator=(const EdbusMessage&);
	public:
		EdbusMessage();
		EdbusMessage(const EdbusMessage& m);
		EdbusMessage(DBusMessage* m);
		~EdbusMessage();

		void create(DBusMessage* m);

		void create_signal(const char* path, const char* interface, const char* name);
		void create_method_call(const char* service, const char* path, const char* interface, const char* method);
		void create_reply(const EdbusMessage& replying_to);
		void create_error_reply(const EdbusMessage& replying_to, const char* errmsg);

		void clear(void);

		bool is_signal(void);
		bool is_method_call(void);
		bool is_error_reply(const char* errmsg);

		void path(const char* np);
		const char* path(void) const;

		void interface(const char* ni);
		const char* interface(void) const;

		void destination(const char* nd);
		const char* destination(void) const;

		void member(const char* nm);
		const char* member(void) const;

		void sender(const char* ns);
		const char* sender(void) const;

		const char* signature(void) const;

		void append(const EdbusData& data);

		iterator begin(void);
		iterator end(void);
		unsigned int size(void);
};

inline EdbusMessage& operator<<(EdbusMessage& m, const EdbusData& val) {
	m.append(val);
	return m;
}

#endif

