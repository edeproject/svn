#ifndef __EDBUSDICT_H__
#define __EDBUSDICT_H__

#include "EdbusData.h"

#ifndef SKIP_DOCS
struct EdbusDictPrivate;
#endif

/**
 * \class EdbusDict
 * \brief Dictionary container for D-BUS types
 *
 * EdbusDict is a dictionary container (or <em>map</em> in STL).
 * Data is stored in key//value pairs so data search and fetching is 
 * done via it's key association.
 *
 * Stored data will be unique by key: if you add key//value pair and later
 * add another pair with already added key (or to use better term: assign
 * a new value with aleady known key), previous value will be overriden.
 *
 * \note
 * Adding (or updating) pairs (via push_back()) is not efficient like
 * in e.g. STL map; it is more or less linear operation assuring uniqueness
 * since D-BUS protocol can tolerate, but not prefer duplicate keys in dictionary.
 * Dict's in D-BUS are often used to contain a small number of elements so
 * this is not a big deal :)
 *
 * Class use implicit sharing.
 *
 * EdbusDict uses EdbusData as base type which means you can put any
 * type EdbusData can hold as value. On other hand, D-BUS specification
 * restricts key types to be only <em>basic</em> D-BUS types (\see Edbus::basic_type()).
 *
 * If you try to add non-basic D-BUS type as key, it will be ignored.
 * \todo This should be assertion
 */
class EdbusDict {
	private:
		EdbusDictPrivate* impl;
		void dispose(void);
		void list_dispose(void);
		void unhook(void);

	public:
		/**
		 * Creates an empty dict
		 */
		EdbusDict();

		/**
		 * Creates a dict from other dict
		 */
		EdbusDict(const EdbusDict&);

		/**
		 * Assign other dict content
		 */
		EdbusDict& operator=(const EdbusDict&);

		/**
		 * Destructor
		 */
		~EdbusDict();

		/**
		 * Assign value with the key and add it. If key already exists,
		 * previous value will be overriden with the new one.
		 *
		 * \param key is key in <em>basic</em> D-BUS type
		 * \param value is any value EdbusData can hold
		 */
		void push_back(const EdbusData& key, const EdbusData& value);

		/**
		 * Clear content
		 */
		void clear(void);

		/**
		 * Returns size of dict content. This is a constant operation
		 */
		unsigned int size(void);

		/**
		 * Remove value and key for the dict. If key is not found, it will
		 * do nothing
		 * 
		 * \param key is key to be removed with it's value
		 */
		void remove(const EdbusData& key);

		/**
		 * Find and retrieve value associated with this key. If value is
		 * not find, it will retrieve invalid EdbusData type (\see EdbusData for the
		 * details about invalid (EDBUS_TYPE_INVALID) type)
		 *
		 * \return associated value or invalid type
		 * \param key is key//value pair to be searched
		 */
		EdbusData find(const EdbusData& key);

		/**
		 * Compares if two dicts are equal
		 */
		bool operator==(const EdbusDict& other);

		/**
		 * Compares if two dicts are not equal
		 */
		bool operator!=(const EdbusDict& other) { return !operator==(other); }
};

#endif
