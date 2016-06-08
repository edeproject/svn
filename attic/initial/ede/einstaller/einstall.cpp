// Installer for EDE
// Copyright (C) 2001-2002 Martin Pekar
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

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
	return;
    }	

    char line[1024];
    FILE* log = fopen(logname, "r");

    if (log != NULL) {
	while(fgets(line, sizeof(line), log)) 
	{
    	    result_output->insert(line);
  	}
  	fclose(log);
    }
    unlink(logname);    
}
