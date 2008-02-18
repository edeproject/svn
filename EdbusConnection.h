#ifndef __EDBUSCONNECTION_H__
#define __DEBUSCONNECTION_H__

#include "EdbusMessage.h"

enum EdbusConnectionType {
	EDBUS_SYSTEM,
	EDBUS_SESSION
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
		bool send_signal(const char* path, const char* interface, const char* name, const char* content);
		bool send(const EdbusMessage& content);

		/* used to call remote methods and wait reply from them */
		bool send_with_reply_and_block(const EdbusMessage& content, int timeout, EdbusMessage& ret);

		int wait(int timout_milliseconds);
};

#endif
