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

static EdbusDictEntry* find_entry_by_key(EntryList& lst, const EdbusData& key) {
	EntryListIter it = lst.begin(), it_end = lst.end();

	while(it != it_end) {
		if((*it)->key == key)
			return (*it);

		++it;
	}

	return NULL;
}

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

	/*
	 * If entry with the same key already exists, just replace value. Opposite
	 * add as new entry.
	 *
	 * Not efficient at all, but this will assure dict contains unique keys.
	 * Dbus specs tolerates duplicate keys in dict, but it can mark data parts
	 * with them as invalid.
	 */
	EdbusDictEntry* e = find_entry_by_key(impl->lst, key);

	if(!e) {
		e = new EdbusDictEntry;
		e->key = key;
		e->value = value;
		impl->lst.push_back(e);
	} else {
		e->value = value;
	}
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
	EdbusDictEntry* e = find_entry_by_key(impl->lst, key);
	if(!e)
		return EdbusData::from_invalid();

	return EdbusData(e->value);
}

bool EdbusDict::operator==(const EdbusDict& other) {
	if(&other == this)
		return true;

	if(impl->lst.size() != other.impl->lst.size())
		return false;

	EntryListIter it = impl->lst.begin(), it_end = impl->lst.end();
	EntryListIter it2 = other.impl->lst.begin();

	while(it != it_end) {
		/* 
		 * EdbusData::operator==() will handle the cases when keys and
		 * values are different types
		 */
		if(((*it)->key != (*it2)->key) || ((*it)->value != (*it2)->value))
			return false;

		++it;
		++it2;
	}

	return true;
}
