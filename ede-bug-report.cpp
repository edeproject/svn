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

#ifndef HAVE_CONFIG_H
#include <config.h>
#endif

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>
#include <stdio.h>

#define EDE_BUGZILLA_URL "http://bugs.equinox-project.org/xmlrpc.cgi"

/* must be static; don't ask me why */
static struct xmlrpc_clientparms gparms;

int main(int argc, char** argv) {
	xmlrpc_env          xenv;
	xmlrpc_client*      xcli;

	xmlrpc_env_init(&xenv);
	xmlrpc_client_setup_global_const(&xenv);

	/* only cURL transport is supported */
	gparms.transport = "curl";

	xmlrpc_client_create(&xenv, XMLRPC_CLIENT_NO_FLAGS, "ede-bug-report", "0.1", &gparms, sizeof(gparms), &xcli);
	if(xenv.fault_occurred) {
		puts(xenv.fault_string);
		return 1;
	}

	xmlrpc_value* result = NULL;

	xmlrpc_client_call2f(&xenv, xcli, EDE_BUGZILLA_URL, "Bugzilla.version", &result, "()");
	if(xenv.fault_occurred) {
		printf("err: %s\n", xenv.fault_string);
		return 1;
	}

	char* tmp;
	xmlrpc_decompose_value(&xenv, result, "{s:s,*}", "version", &tmp);
	printf("version: %s\n", tmp);

	xmlrpc_DECREF(result);

	return 0;
}

