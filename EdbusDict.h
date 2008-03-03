#ifndef __EDBUSDICT_H__
#define __EDBUSDICT_H__

#include "EdbusData.h"

#ifndef SKIP_DOCS
struct EdbusDictPrivate;
#endif

/**
 * \class EdbusDict
 * \brief Represents a dictionary-like container
 */
class EdbusDict {
	private:
		EdbusDictPrivate* impl;
		void dispose(void);
		void list_dispose(void);
		void unhook(void);

	public:
		EdbusDict();
		EdbusDict(const EdbusDict&);
		EdbusDict& operator=(const EdbusDict&);
		~EdbusDict();

		void push_back(const EdbusData& key, const EdbusData& value);
		void clear(void);
		unsigned int size(void);
		void remove(const EdbusData& key);
		EdbusData find(const EdbusData& key);
};

#endif
