#include "EdbusDict.h"
#include <stdio.h>

void EdbusDict::append(const EdbusData& key, const EdbusData& value) {
	if(!EdbusData::basic_type(key))
		return;

	/*
	 * Assure all keys are the same type. The same applies with the values.
	 * This is done by checking first added entry, if exists. 
	 * We can accept keys (or values) with different types, but D-BUS does not
	 * like it.
	 */
	if(size() > 0) {
		if(key.type() != key_type() || value.type() != value_type()) {
			puts("Key or value is different type than I already have. Ignoring...");
			return;
		}
	}

	unhook();

	/*
	 * If entry with the same key already exists, just replace value. Opposite
	 * add as new entry.
	 *
	 * Not efficient at all, but this will assure dict contains unique keys.
	 * Dbus specs tolerates duplicate keys in dict, but it can mark data parts
	 * with them as invalid.
	 */
	EdbusDict::iterator it = begin(), it_end = end();
	while(it != it_end) {
		if((*it).key == key) {
			(*it).value = value;
			return;
		}

		++it;
	}

	EdbusDictEntry n;
	n.key = key;
	n.value = value;

	impl->lst.push_back(n);
}

void EdbusDict::clear(void) {
	unhook();
	impl->lst.clear();
}

void EdbusDict::remove(const EdbusData& key) {
	unhook();

	EdbusDict::iterator it = begin(), it_end = end();
	while(it != it_end) {
		if((*it).key == key) {
			impl->lst.erase(it);
			break;
		}

		++it;
	}
}

EdbusData EdbusDict::find(const EdbusData& key) {
	EdbusDict::iterator it = begin(), it_end = end();
	while(it != it_end) {
		if((*it).key == key)
			return (*it).value;
		++it;
	}

	return EdbusData::from_invalid();
}

bool EdbusDict::operator==(const EdbusDict& other) {
	if(&other == this)
		return true;

	if(size() != other.size())
		return false;

	EdbusDict::iterator it = begin(), it_end = end();
	EdbusDict::iterator it2 = other.begin();
	while(it != it_end) {
		if((*it) != (*it2))
			return false;
		++it;
		++it2;
	}

	return true;
}

EdbusDataType EdbusDict::key_type(void) {
	assert(size() > 0 && "Can't get key type on empty container");

	EdbusDict::iterator it = begin();
	return (*it).key.type();
}

EdbusDataType EdbusDict::value_type(void) {
	assert(size() > 0 && "Can't get value type on empty container");

	EdbusDict::iterator it = begin();
	return (*it).value.type();
}

EdbusDict::iterator EdbusDict::begin(void) const {
	return impl->lst.begin();
}

EdbusDict::iterator EdbusDict::end(void) const {
	return impl->lst.end();
}

unsigned int EdbusDict::size(void) const {
	return impl->lst.size();
}
