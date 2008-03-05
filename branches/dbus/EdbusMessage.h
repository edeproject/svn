#ifndef __EDBUSMESSAGE_H__
#define __DEBUSMESSAGE_H__

#include "EdbusData.h"

class  EdbusConnection;
struct EdbusMessageImpl;
struct EdbusMessageIteratorImpl;
struct DBusMessage;

/**
 * \class EdbusMessageIterator
 * \brief Iterator for EdbusMessage
 *
 * This class allows you to access the content of EdbusMessage object.
 * With it, you will be able to retrieve or skip a parts of EdbusMessage object.
 *
 * EdbusMessageIterator is <em>const</em> iterator; you will be able to
 * get a content from EdbusMessage, but not to modify it or remove (I wasn't able
 * to find a convinient way to directly modify dbus message).
 *
 * If you want to change content of EdbusMessage, you will have to copy interested
 * data in new EdbusMessage object appending changed one. The same applies for removing.
 *
 * \note You should not use this class directly; use <em>EdbusMessage::iterator</em> instead
 */
class EdbusMessageIterator {
	private:
		EdbusMessageIteratorImpl* impl;

	public:
		/**
		 * Creates an empty iterator
		 */
		EdbusMessageIterator();

		/**
		 * Creates an iterator from other EdbusMessageIterator object
		 */
		EdbusMessageIterator(const EdbusMessageIterator& it);

		/**
		 * Creates an iterator from DBusMessage. This function exists for
		 * easier internal usage; you should not use it directly
		 */
		EdbusMessageIterator(DBusMessage* dm);

		/**
		 * Clears iterator data
		 */
		~EdbusMessageIterator();

		/**
		 * Prefix; increase iterator by one
		 */
		EdbusMessageIterator& operator++();

		/**
		 * Compare if two iterators are equal
		 */
		bool operator==(const EdbusMessageIterator& it);

		/**
		 * Compare if two iterators are not equal
		 */
		bool operator!=(const EdbusMessageIterator& it) { return !operator==(it); }

		/**
		 * Dereference iterator; with it you get access to the content
		 * iterator currently points to
		 */
		const EdbusData& operator*(void) const;
};

/**
 * \class EdbusMessage
 * \brief Data transporter for D-BUS
 *
 * EdbusMessage is essentially the way you send and receive a bunch of EdbusData objects.
 * You can see it as e-mail message: EdbusData represents a content you wrote or read and 
 * EdbusMessage is that content plus headers with origin, destination and message type.
 *
 * Knowing that, EdbusMessage can be:
 * - signal, created with create_signal(); it represents D-BUS signal
 * - method call, created with create_method_call(); calls a method on service objects
 * - method reply, created with create_reply(); sends a reply on called method
 * - error reply, created with create_error_reply(); sends a error reply on called method
 *
 * As you will see from below, signals do not have destination; they are broadcasted
 * over session or system bus and all clients (connected on session or system bus) that 
 * do not filter signals will receive it.
 *
 * On other hand, methods (calls and replies) do have destination and they are sent directly
 * to the client. This is the main difference between signals and methods.
 *
 * Creating a specific EdbusMessage type you do via one of the <em>create_</em> members.
 * If you call them on already created EdbusMessage with some content, that content will
 * be discarded and EdbusMessage will become appropriate type.
 *
 * When you create a message, you wants to add some data. This is done via append() member
 * like:
 * \code
 *   EdbusMessage m;
 *   m.create_signal("/org/example/SignalObject", "org.example.Signal", "MySignal");
 *   m.append(EdbusData::from_int32(34));
 *   m.append(EdbusData::from_bool(true));
 *   m.append(EdbusData::from_string("some string"));
 * \endcode
 *
 * EdbusMessage have operator<<() that makes adding data much more... sexy ;-)
 *
 * There is above sample via this opeartor:
 * \code
 *   EdbusMessage m;
 *   m.create_signal("/org/example/SignalObject", "org.example.Signal", "MySignal");
 *   m << EdbusData::from_int32(32) << EdbusData::from_bool(true) << EdbusData::from_string("some string");
 * \endcode
 *
 * Getting data from message is done via iterator. Since iterator do not know what type
 * it points to, you <b>must</b> check it manually before calling appropriate EdbusData function
 * to fetch concrete data or bad things will happen (assertion will be upon you).
 *
 * Let say you received above message and wants to get a content from it. You will do it like:
 * \code
 *   // this will probably be changed to const_iterator
 *   EdbusMessage::iterator it = m.begin(), it_end = m.end();
 *
 *   for(; it != it_end; ++it) {
 *     if((*it).is_int32())
 *       printf("Got int with: %i\n", (*it).to_int32());
 *
 *     if((*it).is_bool())
 *       printf("Got bool with: %i\n", (*it).to_bool());
 *
 *     if((*it).is_string())
 *       printf("Got string with: %s\n", (*it).to_string());
 *   }
 * \endcode
 *
 * As you will see from EdbusMessageIterator documentation, this is constant iterator: you will not
 * be able to modify existing data in message, nor to remove it. You have to create a new message and
 * manually copy interested content (\see EdbusMessageIterator for info about this limitation).
 *
 * On other hand, you can add a new data to existing message without copying it.
 */
class EdbusMessage {
	public:
		/**
		 * Declare EdbusMessage iterator
		 * \todo This should be const_iterator
		 */
		typedef EdbusMessageIterator iterator;

	private:
		friend class EdbusConnection;
		EdbusMessageImpl* dm;
		DBusMessage* message(void) const;

		EdbusMessage& operator=(const EdbusMessage&);

	public:
		/**
		 * Create an empty EdbusMessage object. Nothing will be allocated
		 * until you call one of the <em>create_</em> members. Until that, message
		 * is marked as invalid and EdbusConnection will refuse to send it
		 */
		EdbusMessage();

		/**
		 * Create an EdbusMessage object from already created one
		 */
		EdbusMessage(const EdbusMessage& m);

		/**
		 * Create an EdbusMessage from DBusMessage. This is used to simplify
		 * internals and you should not use it directly
		 */
		EdbusMessage(DBusMessage* m);

		/**
		 * Clears internal data
		 */
		~EdbusMessage();

		/**
		 * Create an EdbusMessage from DBusMessage. This is used to simplify
		 * internals and you should not use it directly
		 */
		void create(DBusMessage* m);

		/**
		 * Create a signal message
		 *
		 * \param path is destination object path
		 * \param interface is destination interface name
		 * \param name is signal name
		 */
		void create_signal(const char* path, const char* interface, const char* name);

		/**
		 * Create a method call
		 *
		 * \param service is destination service name
		 * \param param is destination object path
		 * \param interface is destination interface name
		 * \param method is method to be called
		 */
		void create_method_call(const char* service, const char* path, const char* interface, const char* method);

		/**
		 * Create a reply for method
		 *
		 * \param replying_to is a message to be replyed
		 */
		void create_reply(const EdbusMessage& replying_to);

		/**
		 * Create error reply
		 *
		 * \param replying_to is a message to be replyed
		 * \param errmsg is error string to be sent
		 */
		void create_error_reply(const EdbusMessage& replying_to, const char* errmsg);

		/**
		 * Clears all EdbusMessage data and marks it as invalid
		 */
		void clear(void);

		/**
		 * Returns true if current message is signal type
		 */
		bool is_signal(void);

		/**
		 * Returns true if current message is method call type
		 */
		bool is_method_call(void);

		/**
		 * Returns true if current message is error reply type
		 */
		bool is_error_reply(const char* errmsg);

		/**
		 * Set object path for destination
		 */
		void path(const char* np);

		/**
		 * Get object path for destination
		 */
		const char* path(void) const;

		/**
		 * Set interface name for destination
		 */
		void interface(const char* ni);

		/**
		 * Get interface name for destination
		 */
		const char* interface(void) const;

		/**
		 * Sets the message's destination.
		 *
		 * The destination is the name of another connection on the bus 
		 * and may be either the unique name assigned by the bus to each 
		 * connection, or a well-known name specified in advance
		 */
		void destination(const char* nd);

		/**
		 * Get a message destination
		 */
		const char* destination(void) const;

		/**
		 * Set method name to be called.
		 * \note This function can be used to set signal name too
		 */
		void member(const char* nm);

		/**
		 * Get method or signal name from message
		 */
		const char* member(void) const;

		/**
		 * Sets the message sender.
		 *
		 * The sender must be a valid bus name as defined in the D-Bus 
		 * specification
		 */
		void sender(const char* ns);

		/**
		 * Gets the unique name of the connection which originated this 
		 * message, or NULL if unknown or inapplicable.
		 *
		 * The sender is filled in by the message bus.
		 */
		const char* sender(void) const;

		/**
		 * Returns the signature of this message. You will not need this
		 * unless you know what returned value means
		 */
		const char* signature(void) const;

		/**
		 * Append EdbusData object in message
		 */
		void append(const EdbusData& data);

		/**
		 * Returns iterator at the message start. It points to the first element
		 */
		iterator begin(void) const;

		/**
		 * Returns iterator at the message end. It <b>does not</b> points to
		 * the last element, but element after the last, and you must not dereferce it
		 */
		iterator end(void) const;

		/**
		 * Returns the size of EdbusMessage content. Beware that this function
		 * runs in linear time.
		 */
		unsigned int size(void);
};

/**
 * A convinient shortcut for EdbusMessage::append(). You can use it as:
 * \code
 *   EdbusMessage m;
 *   m << EdbusData::from_int32(4) << EdbusData::from_string("sample string") << EdbusData::from_bool(true);
 * \endcode
 */
inline EdbusMessage& operator<<(EdbusMessage& m, const EdbusData& val) {
	m.append(val);
	return m;
}

#endif

