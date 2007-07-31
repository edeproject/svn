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

#include "EvokeService.h"

#include <FL/Fl.h>
#include <FL/x.h>

#include <edelib/Config.h>
#include <edelib/Debug.h>
#include <edelib/File.h>

#include <string.h>
#include <signal.h>

#define FOREVER       1e20
#define CONFIG_FILE   "evoke.conf"
#define APPNAME       "evoke"
#define DEFAULT_PID   "/tmp/evoke.pid"
/*
 * Used to assure unique instance, even if is given another 
 * path for pid. This option can't be modified by user.
 * TODO: add lock on file so it can't be removed ?
 */
#define LOCK_FILE  "/tmp/.evoke.lock"

#define CHECK_ARGV(argv, pshort, plong) ((strcmp(argv, pshort) == 0) || (strcmp(argv, plong) == 0))

void quit_signal(int sig) {
	EVOKE_LOG("Got quit signal %i\n", sig);
	EvokeService::instance()->stop();
}

int xmessage_handler(int e) {
	return EvokeService::instance()->handle(fl_xevent);
}

const char* next_param(int curr, char** argv, int argc) {
	int j = curr + 1;
	if(j >= argc)
		return NULL;
	if(argv[j][0] == '-')
		return NULL;
	return argv[j];
}

void help(void) {
	puts("Usage: "APPNAME" [OPTIONS]");
	puts("EDE startup manager responsible for starting, quitting and tracking");
	puts("various pieces of desktop environment and external programs.");
	puts("...and to popup a nice window when something crashes...\n");
	puts("Options:");
	puts("  -h, --help            this help");
	puts("  -s, --startup         run in starup mode");
	puts("  -n, --no-splash       do not show splash screen in starup mode");
	puts("  -d, --dry-run         run in starup mode, but don't execute anything");
	puts("  -a, --autostart       read autostart directory");
	puts("  -c, --config [FILE]   use FILE as config file");
	puts("  -p, --pid [FILE]      use FILE to store PID number");
	puts("  -l, --log [FILE]      log traffic to FILE (FILE can be stdout/stderr for console output)\n");
}

int main(int argc, char** argv) {
	const char* config_file = NULL;
	const char* pid_file    = NULL;
	const char* log_file    = NULL;

	bool do_startup   = 0;
	bool do_dryrun    = 0;
	bool no_splash    = 0;
	bool do_autostart = 0;

	if(argc > 1) {
		const char* a;
		for(int i = 1; i < argc; i++) {
			a = argv[i];
			if(CHECK_ARGV(a, "-h", "--help")) {
				help();
				return 0;
			} else if(CHECK_ARGV(a, "-c", "--config")) {
				config_file = next_param(i, argv, argc);
				if(!config_file) {
					puts("Missing configuration filename");
					return 1;
				}
				i++;
			} else if(CHECK_ARGV(a, "-p", "--pid")) {
				pid_file = next_param(i, argv, argc);
				if(!pid_file) {
					puts("Missing pid filename");
					return 1;
				}
				i++;
			} else if(CHECK_ARGV(a, "-l", "--log")) {
				log_file = next_param(i, argv, argc);
				if(!log_file) {
					puts("Missing log filename");
					return 1;
				}
				i++;
			} 
			else if(CHECK_ARGV(a, "-s", "--startup"))
				do_startup = 1;
			else if(CHECK_ARGV(a, "-d", "--dry-run"))
				do_dryrun = 1;
			else if(CHECK_ARGV(a, "-n", "--no-splash"))
				no_splash = 1;
			else if(CHECK_ARGV(a, "-a", "--autostart"))
				do_autostart = 1;
			else {
				printf("Unknown parameter '%s'. Run '"APPNAME" -h' for options\n", a);
				return 1;
			}
		}
	}
	
	// make sure X11 is running before rest of code is called
	fl_open_display();

	EvokeService* service = EvokeService::instance();

	if(!service->setup_logging(log_file)) {
		printf("Can't open %s for logging. Please choose some writeable place\n", log_file);
		return 1;
	}

	EVOKE_LOG("= "APPNAME" started =\n");

	if(!pid_file)
		pid_file = DEFAULT_PID;

	if(!service->setup_pid(pid_file, LOCK_FILE)) {
		EVOKE_LOG("Either another "APPNAME" instance is running or can't create pid file. Please correct this\n");
		EVOKE_LOG("Note: if program abnormaly crashed before, just remove '%s' and start it again\n", LOCK_FILE);
		EVOKE_LOG("= "APPNAME" abrupted shutdown =\n");
		return 1;
	}

	if(!config_file)
		config_file = CONFIG_FILE; // TODO: XDG paths

	if(do_startup) {
		if(!service->init_splash(config_file, no_splash, do_dryrun)) {
			EVOKE_LOG("Unable to read correctly %s. Please check it is correct config file\n", config_file);
			EVOKE_LOG("= "APPNAME" abrupted shutdown =\n");
			return 1;
		}
	}

	if(do_autostart) {
		service->init_autostart();
	}

	service->setup_atoms(fl_display);

	signal(SIGINT, quit_signal);
	signal(SIGTERM, quit_signal);
	signal(SIGKILL, quit_signal);

	service->start();

	XSelectInput(fl_display, RootWindow(fl_display, fl_screen), PropertyChangeMask | SubstructureNotifyMask | ClientMessage);

	/*
	 * Register event listener and run in infinite loop. Loop will be
	 * interrupted from one of the received signals.
	 *
	 * I choose to use fltk for this since wait() will nicely pool events
	 * and pass expecting ones to xmessage_handler(). Other (non-fltk) solution would
	 * be to manually pool events via select() and that code could be very messy.
	 * So stick with the simplicity :)
	 */
	Fl::add_handler(xmessage_handler);
	while(service->running())
		Fl::wait(FOREVER);

	EVOKE_LOG("= "APPNAME" nice shutdown =\n");
	return 0;
}
