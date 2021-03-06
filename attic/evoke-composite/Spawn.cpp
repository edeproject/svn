/*
 * $Id$
 *
 * Evoke, head honcho of everything
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2007 EDE Authors.
 *
 * This program is licensed under terms of the 
 * GNU General Public License version 2 or newer.
 * See COPYING for details.
 */


#include <sys/types.h> // fork
#include <unistd.h>    // fork, open, close, dup
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>       // getrlimit, setrlimit
#include <sys/resource.h>   //

#include "Spawn.h"

extern char** environ;
SignalWatch* global_watch = 0;

void sigchld_handler(int sig) {
	int pid, status;
	do {
		errno = 0;
		pid = waitpid(WAIT_ANY, &status, WNOHANG | WUNTRACED);

		if(global_watch != 0) {
			if(WIFEXITED(status))
				status = WEXITSTATUS(status);
			else if(WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV)
				status = SPAWN_CHILD_CRASHED;
			else
				status = SPAWN_CHILD_KILLED;

			global_watch(pid, status);
		}

	} while(pid <= 0 && errno == EINTR);
}

int spawn_program(const char* cmd, SignalWatch wf, pid_t* child_pid_ret, const char* ofile) {
	if(!cmd)
		return SPAWN_EMPTY;

	int nulldev = -1;
	int status_ret = 0;

#if 0
	struct sigaction sa;
	sa.sa_handler = sigchld_handler;
	sa.sa_flags = SA_NOCLDSTOP;
	/*sa.sa_flags = SA_NOCLDWAIT;*/
	sigemptyset(&sa.sa_mask);
	sigaction(SIGCHLD, &sa, (struct sigaction*)0);
#endif

	struct sigaction sa;
	sigset_t chld_mask, old_mask;

	sa.sa_handler = sigchld_handler;
	sa.sa_flags = SA_NOCLDSTOP;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGCHLD, &sa, (struct sigaction*)0);

	// block SIGCHLD until we set up stuff
	sigemptyset(&chld_mask);
	sigaddset(&chld_mask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &chld_mask, &old_mask);

	global_watch = wf;

	pid_t pid = fork();
	if(pid == -1)
		return SPAWN_FORK_FAILED;

	if(pid == 0) {
		// child
		char* argv[4];
		argv[0] = "/bin/sh";
		argv[1] = "-c";
		argv[2] = (char*)cmd;
		argv[3] = NULL;

		/*
		 * The following is to avoid X locking when executing 
		 * terminal based application that requires user input
		 */
		if(ofile)
			nulldev = open(ofile, O_WRONLY | O_TRUNC | O_CREAT, 0770);
		else
			nulldev = open("/dev/null", O_RDWR);

		if(nulldev == -1)
			return SPAWN_OPEN_FAILED;

		/* TODO: redirect these to EvokeService log */
		close(0); dup(nulldev);
		close(1); dup(nulldev);
		close(2); dup(nulldev);

		errno = 0;
		if(execve(argv[0], argv, environ) == -1) {
			// should not get here
			return SPAWN_EXECVE_FAILED;
		}
	} 

	// parent
	if(nulldev != -1)
		close(nulldev);
	/*
	 * Record child pid; it is returned by fork(), but since this
	 * function does not wait until child quits, it will return
	 * immediately, filling (if) requested child pid
	 */
	if(child_pid_ret)
		*child_pid_ret = pid;
	
	// unblock SIGCHLD
	sigprocmask(SIG_SETMASK, &old_mask, NULL);

	return status_ret;
}

int spawn_program_with_core(const char* cmd, SignalWatch* wf, pid_t* child_pid_ret) {
	struct rlimit r;
	if(getrlimit(RLIMIT_CORE, &r) == -1)
		return -1;

	rlim_t old = r.rlim_cur;
	r.rlim_cur = RLIM_INFINITY;

	// FIXME: add own core limit ?
	if(setrlimit(RLIMIT_CORE, &r) == -1)
		return -1;

	int ret = spawn_program(cmd, wf, child_pid_ret);

	r.rlim_cur = old;
	if(setrlimit(RLIMIT_CORE, &r) == -1)
		return -1;

	return ret;
}
