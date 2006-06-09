/*
 * $Id$
 *
 * Einstaller, frontend for package installation
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "einstall.h"
#include "einstaller.h"

char tempname[4096], logname[4096];

void install_package()
{
    const char *package = package_name_input->value();
    const char *e = fl_file_getext(package); 
    strncpy(tempname, "/tmp/einstXXXXXX", 4096);
    close(mkstemp(tempname)); 
    remove(tempname);
    strncpy(logname, tempname, 4096);
    
    if (strlen(e)<1) 
    {
	result_output->insert(_("Please enter the name of the package.\n"));
	result_output->relayout();
	return; 
    }
    
    if (strcmp(e, ".rpm")==0) 
    {
	char command[FL_PATH_MAX];

	if (nodeps_button->value()) 
	    snprintf(command, sizeof(command)-1, "rpm -i --nodeps %s >& %s", package, logname);
	else 
	    snprintf(command, sizeof(command)-1, "rpm -i %s >& %s", package, logname);
	fl_start_child_process(command);
	install_progress->value(100);
    }	
    else if (strcmp(e, ".tgz")==0) 
    {
	char command[FL_PATH_MAX];
	snprintf(command, sizeof(command)-1, "installpkg %s >& %s", package, logname);
	fl_start_child_process(command);
	install_progress->value(100);
    }	
    else if (strcmp(e, ".deb")==0) {
	char command[FL_PATH_MAX];
	snprintf(command, sizeof(command)-1, "dpkg -i %s >& %s", package, logname);
	fl_start_child_process(command);
	install_progress->value(100);
    }	
    else 
    {   
        result_output->insert(_("Please enter the name of the package.\n"));
	result_output->relayout();
	return;
    }	

    char line[1024];
    FILE* log = fopen(logname, "r");

    if (log != NULL) {
	while(fgets(line, sizeof(line), log)) 
	{
    	    result_output->insert(line);
  	}
	result_output->relayout();
  	fclose(log);
    }
    unlink(logname);    
}
