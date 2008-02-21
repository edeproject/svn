#ifndef __EDBUSCONNECTION_H__
#define __EDBUSCONNECTION_H__

#include "EdbusMessage.h"

enum EdbusConnectionType {
	EDBUS_SYSTEM,
	EDBUS_SESSION
};

enum EdbusNameMode {
	EDBUS_NAME_NO_REPLACE        = 0,
	EDBUS_NAME_ALLOW_REPLACE     = 1,
	EDBUS_NAME_REPLACE_EXISTITNG = 2
};

struct EdbusConnImpl;

class EdbusConnection {
	private:
		EdbusConnImpl* dc;
	public:
		EdbusConnection();
		~EdbusConnection();

		bool connect(EdbusConnectionType ctype);
		bool disconnect(void);
		bool send(const EdbusMessage& content);

		/* used to call remote methods and wait reply from them */
		bool send_with_reply_and_block(const EdbusMessage& content, int timeout, EdbusMessage& ret);

		/* try to set readable name; flags are or-ed */
		bool request_name(const char* name, int mode = EDBUS_NAME_NO_REPLACE);

		/* get unique name for this connection (can be NULL) */
		const char* unique_name(void);

		void add_signal_callback(const char* match, int (*cb)(const EdbusMessage*, void*), void* data);
		void add_method_callback(const char* match, int (*cb)(const EdbusMessage*, void*), void* data);

		void setup_listener(void);

		int wait(int timout_milliseconds);
};

#endif
