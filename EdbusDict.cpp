#include <stdio.h>
#include <list>
#include <assert.h>

#include "EdbusDict.h"

struct EdbusDictEntry {
	EdbusData key;
	EdbusData value;
};

/* 
 * store them as pointers since list will require an empty constructor
 * which will construct (by default) invalid types
 */
typedef std::list<EdbusDictEntry*> EntryList;
typedef std::list<EdbusDictEntry*>::iterator EntryListIter;

struct EdbusDictPrivate {
	EntryList lst;
	unsigned int ref;
};

EdbusDict::EdbusDict() : impl(0) {
	impl = new EdbusDictPrivate;
	impl->ref = 1;
}

EdbusDict::EdbusDict(const EdbusDict& other) {
	if(this == &other)
		return;

	impl = other.impl;
	other.impl->ref++;
}

EdbusDict::~EdbusDict() {
	impl->ref--;

	if(impl->ref == 0)
		dispose();
}

EdbusDict& EdbusDict::operator=(const EdbusDict& other) {
	other.impl->ref++;
	impl->ref--;

	if(impl->ref == 0)
		dispose();

	impl = other.impl;
	return *this;
}

void EdbusDict::dispose(void) {
	if(!impl)
		return;

	list_dispose();
	puts("dispose");

	delete impl;
	impl = 0;
}

void EdbusDict::list_dispose(void) {
	if(impl->lst.size() > 0) {
		EntryListIter it = impl->lst.begin(), it_end = impl->lst.end();
		while(it != it_end) {
			delete *it;
			++it;
		}

		impl->lst.clear();
	}
}

void EdbusDict::unhook(void) {
	assert(impl != NULL);

	if(impl->ref == 1)
		return;

	puts("unhook");

	EdbusDictPrivate* new_one = new EdbusDictPrivate;
	new_one->ref = 1;

	/* 
	 * Copy the content
	 *
	 * edelib::list does not have implemented copy operator
	 * and that is the way I like
	 */

	if(impl->lst.size() > 0) {
		EntryListIter it = impl->lst.begin(), it_end = impl->lst.end();
		EdbusDictEntry* entry;

		while(it != it_end) {
			entry = new EdbusDictEntry;
			entry->key = (*it)->key;
			entry->value = (*it)->value;
			new_one->lst.push_back(entry);

			++it;
		}
	}

	impl->ref--;
	impl = new_one;
}

void EdbusDict::push_back(const EdbusData& key, const EdbusData& value) {
	if(!EdbusData::basic_type(key))
		return;

	unhook();
	
	EdbusDictEntry* entry = new EdbusDictEntry;
	entry->key = key;
	entry->value = value;
	impl->lst.push_back(entry);
}

void EdbusDict::clear(void) {
	unhook();
	list_dispose();
}

unsigned int EdbusDict::size(void) {
	return impl->lst.size();
}

void EdbusDict::remove(const EdbusData& key) {
	unhook();

	EntryListIter it = impl->lst.begin(), it_end = impl->lst.end();
	while(it != it_end) {
		if((*it)->key == key) {
			impl->lst.erase(it);
			break;
		}

		++it;
	}
}

EdbusData EdbusDict::find(const EdbusData& key) {
	EntryListIter it = impl->lst.begin(), it_end = impl->lst.end();

	while(it != it_end) {
		if((*it)->key == key) {
			return (*it)->value;
		}

		++it;
	}

	return EdbusData::from_invalid();
}
