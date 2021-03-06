/*
 * $Id$
 *
 * Run external program
 * Part of edelib.
 * Copyright (c) 2005-2007 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include <sys/types.h> // fork
#include <unistd.h>    // fork, open, close, dup
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#include <edelib/Run.h>
#include <edelib/Debug.h>

EDELIB_NAMESPACE {

int run_fork(const char* cmd, bool wait) 
{
	if(!cmd)
		return RUN_EMPTY;

	int nulldev = -1;
	int status = 0;
	int status_ret = 0;
	
	//pid_t pid = vfork();
	pid_t pid = fork();

	if(pid == -1)
		return RUN_FORK_FAILED;

	// run the child
	if(pid == 0) {
		char* argv[4];
		argv[0] = "/bin/sh";
		argv[1] = "-c";
		argv[2] = (char*)cmd;
		argv[3] = NULL;

		extern char** environ;

		/*
		 * The following is to avoid X locking when executing 
		 * terminal based application that requires user input
		 */
		if((nulldev = open("/dev/null", O_RDWR)) == -1)
			return RUN_FORK_FAILED;

		close(0); dup(nulldev);
		close(1); dup(nulldev);
		close(2); dup(nulldev);

		if(execve(argv[0], argv, environ) == -1) {
			close(nulldev);
			// should not get here
			return RUN_EXECVE_FAILED;
		}
	}

	if(wait) {
		if(waitpid(pid, &status, 0) == -1) 
			status_ret = RUN_WAITPID_FAILED;
		else {
			if(!WIFEXITED(status)) {
				status_ret = WEXITSTATUS(status);
				EDEBUG(ESTRLOC ": run_fork(): Child '%s' died with %i\n", cmd, status_ret);
			}
			else if(WIFSIGNALED(status)) {
				status_ret = WTERMSIG(status);
				EDEBUG(ESTRLOC ": run_fork(): Child '%s' signaled with %i\n", cmd,  status_ret);
			}
			else {
				// convert status signal to errno format
				int s = WEXITSTATUS(status);
				if(s == 127)
					status_ret = RUN_NOT_FOUND;
				else if(s == 126)
					status_ret = RUN_NOT_EXEC;
				else
					status_ret = s;
			}
		}
	}

	/*
	if(nulldev > -1)
		close(nulldev);
	*/

	return status_ret;
}

int run_program(const char* cmd, bool wait, bool root) 
{
	return run_fork(cmd, wait);
}

}
