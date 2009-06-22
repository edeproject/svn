/*
 * $Id$
 *
 * ede-bug-report, a tool to report bugs on EDE bugzilla instance
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2009 EDE Authors.
 *
 * This program is licensed under terms of the 
 * GNU General Public License version 2 or newer.
 * See COPYING for details.
 */

#include <stdlib.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>
#include <edelib/Debug.h>

#include "Bugzilla.h"

EDELIB_NS_USING(String)

/* must be static; don't ask me why */
static struct xmlrpc_clientparms gparms;

struct BugzillaData {
	xmlrpc_env      xenv;
	xmlrpc_client  *xcli;
	String          url;
};

BugzillaData *bugzilla_new(const char *u) {
	BugzillaData *data = new BugzillaData;

	xmlrpc_env_init(&data->xenv);
	xmlrpc_client_setup_global_const(&data->xenv);
	data->url = u;

	/* only cURL transport is supported */
	gparms.transport = "curl";

	xmlrpc_client_create(&data->xenv, XMLRPC_CLIENT_NO_FLAGS, "ede-bug-report", "0.1", &gparms, sizeof(gparms), &data->xcli);
	if(data->xenv.fault_occurred) {
		E_WARNING(E_STRLOC ": Unable to init xmlrpc client data (%s)\n", data->xenv.fault_string);
		delete data;
		data = NULL;
	}

	return data;
}

void bugzilla_free(BugzillaData *data) {
	E_ASSERT(data != NULL);

	xmlrpc_client_destroy(data->xcli);
	xmlrpc_env_clean(&data->xenv);

	delete data;
}

String bugzilla_get_version(BugzillaData *data) {
	E_ASSERT(data != NULL);

	xmlrpc_value *result;
	xmlrpc_client_call2f(&data->xenv, data->xcli, data->url.c_str(), "Bugzilla.version", &result, "()");

	if(data->xenv.fault_occurred) {
		E_WARNING(E_STRLOC ": Unable to call xmlrpc function (%s)\n", data->xenv.fault_string);
		return "";
	}

	char  *tmp;
	String ret;

	xmlrpc_decompose_value(&data->xenv, result, "{s:s,*}", "version", &tmp);
	ret = tmp;
	/* xmlrpc_decompose_value() allocates returned string value */
	free(tmp);

	xmlrpc_DECREF(result);
	return ret;
}
