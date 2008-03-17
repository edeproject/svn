#ifndef __EDBUSERROR_H__
#define __EDBUSERROR_H__

/**
 * \enum EdbusErrorType
 * \brief EdbusError error type
 */
enum EdbusErrorType {
	EDBUS_ERROR_INVALID,              ///< empty error
	EDBUS_ERROR_FAILED,               ///< generic failure
	EDBUS_ERROR_NO_MEMORY,            ///< out of memory
	EDBUS_ERROR_SERVICE_UNKNOWN,      ///< address service is neither connected nor can be activated
	EDBUS_ERROR_NAME_HAS_NO_OWNER,    ///< a non-unique name used in message is not known
	EDBUS_ERROR_NO_REPLY,             ///< expected reply not received
	EDBUS_ERROR_IO_ERROR,             ///< IO error during operation
	EDBUS_ERROR_BAD_ADDRESS,          ///< malformed address to be connected to
	EDBUS_ERROR_NOT_SUPPORTED,        ///< requested operation not supported
	EDBUS_ERROR_LIMITS_EXCEEDED,      ///< resource reached it's limit
	EDBUS_ERROR_ACCESS_DENIED,        ///< not enough permissions to execute or access operation
	EDBUS_ERROR_AUTH_FAILED,          ///< authentication failed
	EDBUS_ERROR_NO_SERVER,            ///< unable to connect to the D-Bus server
	EDBUS_ERROR_TIMEOUT,              ///< an timeout occured during operation
	EDBUS_ERROR_NO_NETWORK,           ///< transport channel network not available
	EDBUS_ERROR_DISCONNECTED,         ///< trying to perform operation on unconnected connection
	EDBUS_ERROR_INVALID_ARGS,         ///< invalid arguments for method call
	EDBUS_ERROR_FILE_NOT_FOUND,       ///< a file needed for operation not present
	EDBUS_ERROR_FILE_EXISTS,          ///< file exists, but operation does not allow overwriting
	EDBUS_ERROR_UNKNOWN_METHOD,       ///< unknown method
	EDBUS_ERROR_TIMED_OUT,            ///< an operation timed out
	EDBUS_ERROR_INVALID_SIGNATURE,    ///< invalid signature for message
	EDBUS_ERROR_USER_DEFINED          ///< user defined error
};

struct EdbusErrorImpl;
struct DBusError;

class EdbusError {
	private:
		EdbusErrorImpl* impl;
		void dispose(void);
	public:
		EdbusError();
		~EdbusError();
		EdbusError(const DBusError* err);
		EdbusError(const EdbusError& other);
		EdbusError& operator=(const EdbusError& other);

		EdbusError(EdbusErrorType, const char* msg);

		/**
		 * Returns error type
		 */
		EdbusErrorType type(void) const;

		/**
		 * Returns the D-Bus error name
		 */
		const char* name() const;

		/**
		 * Returns a string describing the error
		 */
		const char* message() const;
};

#endif
