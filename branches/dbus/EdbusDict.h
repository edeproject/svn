#ifndef __EDBUSDICT_H__
#define __EDBUSDICT_H__

#include "EdbusData.h"

struct EdbusDictEntry {
	EdbusData key;
	EdbusData value;
};

struct EdbusDictPrivate;

class EdbusDict {
	private:
		EdbusDictPrivate* impl;
		void dispose(void);
		void unhook(void);
	public:
		EdbusDict();
		EdbusDict(const EdbusDict&);
		EdbusDict& operator=(const EdbusDict&);
		~EdbusDict();

		void push_back(const EdbusData& key, const EdbusData& value);
		void remove(const EdbusData& key);
		EdbusData find(const EdbusData& key);
};

#endif
