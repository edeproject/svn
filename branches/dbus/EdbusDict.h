#ifndef __EDBUSDICT_H__
#define __EDBUSDICT_H__

#include "EdbusData.h"

#ifndef SKIP_DOCS
struct EdbusDictPrivate;
#endif

class EdbusDict;

/**
 * \class EdbusDictEntry
 * \brief An entry in EdbusDict
 */
struct EdbusDictEntry {
	/** key */
	EdbusData key;
	/** value */
	EdbusData value;
};

/**
 * \class EdbusDictIterator
 * \brief Allows iterating over EdbusDict
 *
 * With this class you can iterate over EdbusDict content. Dereferencing iterator
 * you will gain access to the EdbusDictEntry and it's members.
 *
 * This is a constant iterator, so you will not be able to change EdbusDict
 * content with it; you should use EdbusDict members instead.
 *
 * \note You should not use this class directly; use <em>EdbusDict::iterator</em> instead
 */
class EdbusDictIterator {
	private:
		EdbusDict* dict;
		bool end;

	public:
		/**
		 * Creates an empty iterator
		 */
		EdbusDictIterator();

		/**
		 * Creates an iterator from EdbusDict object
		 */
		EdbusDictIterator(const EdbusDict& dict);

		/**
		 * Creates an iterator from other EdbusDictIterator object
		 */
		EdbusDictIterator(const EdbusDictIterator&);

		/**
		 * Assign iterator value from other object
		 */
		EdbusDictIterator& operator=(const EdbusDictIterator&);

		/**
		 * Prefix increament
		 */
		EdbusDictIterator& operator++(void);

		/**
		 * Access to EdbusDictEntry value
		 */
		const EdbusDictEntry& operator*(void) const;

		/**
		 * Check if two iterators are equal
		 */
		bool operator==(const EdbusDictIterator& other);

		/**
		 * Check if two iterators are not equal
		 */
		bool operator!=(const EdbusDictIterator& other) { return !operator==(other); }
};

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
 * Adding (or updating) pairs (via append()) is not efficient like
 * in e.g. STL map; it is more or less linear operation assuring uniqueness;
 * D-BUS protocol can tolerate, but not prefer duplicate keys in dictionary.
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
 *
 * Each instance of EdbusDict object will have the same types for the keys and the
 * same types for the values. This means that if you add in freshly created 
 * EdbusDict object a key as int32_t and value as string, all further
 * adding is expecting that types too. On other hand, if you try to add a key with
 * different type (in dict that already have few keys with e.g. int32_t), entry (key and
 * value) will be ignored.
 *
 * This will summarize the thing:
 * \code
 *   EdbusDict d;
 *   d.append(EdbusData::to_string("foo"), EdbusData::to_int32(4));
 *   // now d will accept only keys of type string and values of type int32_t
 *
 *   // ok, it will be added
 *   d.append(EdbusData::to_string("baz"), EdbusData::to_int32(5));
 *
 *   // not ok, values are different type; it will not be added
 *   d.append(EdbusData::to_string("baz"), EdbusData::to_bool(true));
 * \endcode
 *
 * Besides using find() to get a content, you can use iterator too \see EdbusDictIterator.
 * Iterator points to the EdbusDictEntry which have two members, each EdbusData type. This means
 * that you should use <em>is_</em> members to check key and value types.
 *
 * This is a sample of listing dict content, without type checks:
 * \code
 *   EdbusDict d;
 *   d.append("foo", 4);
 *   d.append("baz", 12);
 *
 *   EdbusDict::iterator it = d.begin(), it_end = d.end();
 *   while(it != it_end) {
 *     printf("key: %s value: %i\n", (*it).key.to_string(), (*it).value.to_int32());
 *     ++it;
 *   }
 * \end
 */
class EdbusDict {
	private:
		friend class EdbusDictIterator;

		EdbusDictPrivate* impl;
		void dispose(void);
		void list_dispose(void);
		void unhook(void);

	public:
		/**
		 * Declare EdbusDict iterator
		 * \todo This should be const_iterator
		 */
		typedef EdbusDictIterator iterator;

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
		void append(const EdbusData& key, const EdbusData& value);

		/**
		 * Clear content
		 */
		void clear(void);

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

		/**
		 * Returns type of keys stored in dict
		 */
		EdbusDataType key_type(void);

		/**
		 * returns type of values stored in dict
		 */
		EdbusDataType value_type(void);

		/**
		 * Returns iterator at the dict start. It points to the first element
		 */
		iterator begin(void) const;

		/**
		 * Returns iterator at the dict end. It <b>does not</b> points to
		 * the last element, but element after the last, and you must not dereferce it
		 */
		iterator end() const;

		/**
		 * Returns size of dict content. This is a constant operation
		 */
		unsigned int size(void);
};

#endif
