/*
 * $Id$
 *
 * Evoke, head honcho of everything
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2007-2009 EDE Authors.
 *
 * This program is licensed under terms of the 
 * GNU General Public License version 2 or newer.
 * See COPYING for details.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <FL/Fl.H>
#include <FL/x.H>

#include "EvokeService.h"
#include "Autostart.h"

#define FOREVER   1e20
#define LOCK_FILE "/tmp/.evoke.lock"

#define CHECK_ARGV(argv, pshort, plong) ((strcmp(argv, pshort) == 0) || (strcmp(argv, plong) == 0))

static void quit_signal(int sig) {
	EvokeService::instance()->stop();
}

static int xmessage_handler(int) {
	return EvokeService::instance()->handle(fl_xevent);
}

static const char* next_param(int curr, char** argv, int argc) {
	int j = curr + 1;
	if(j >= argc)
		return NULL;
	if(argv[j][0] == '-')
		return NULL;
	return argv[j];
}

static void help(void) {
	puts("Usage: evoke [OPTIONS]");
	puts("EDE startup manager responsible for desktop starting and quitting");
	puts("(including various bits and pieces desktop needs)");
	puts("Options:");
	puts("  -h, --help            this help");
	puts("  -s, --startup         run in starup mode");
	puts("  -n, --no-splash       do not show splash screen in starup mode");
	puts("  -d, --dry-run         run in starup mode, but don't execute anything");
	puts("  -a, --autostart       read autostart directory and run all items");
	puts("  -u, --autostart-safe  read autostart directory and display dialog what will be run");
	puts("  -c, --config [FILE]   use FILE as config file\n");
}

int main(int argc, char** argv) {
	const char* config_file = NULL;
	bool do_startup         = false;
	bool do_dryrun          = false;
	bool show_splash        = true;
	bool do_autostart       = false;
	bool do_autostart_safe  = false;

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
			}
			else if(CHECK_ARGV(a, "-s", "--starup"))
				do_startup = true;
			else if(CHECK_ARGV(a, "-d", "--dry-run"))
				do_dryrun = true;
			else if(CHECK_ARGV(a, "-n", "--no-splash"))
				show_splash = false;
			else if(CHECK_ARGV(a, "-a", "--autostart"))
				do_autostart = true;
			else if(CHECK_ARGV(a, "-u", "--autostart-safe"))
				do_autostart_safe = true;
			else {
				printf("Unknown parameter '%s'. Run 'evoke -h' for options\n", a);
				return 1;
			}
		}
	}

	/* make sure X11 is running before rest of code is called */
	fl_open_display();

	/* initialize main service object */
	EvokeService* service = EvokeService::instance();

	/* TODO: XDG dirs */
	if(!config_file)
		config_file = "ede-startup.conf";

	/*
	 * Main loop is not initialized before startup (and splash) are finished;
	 * this is intentional so we could use 'dryrun' to test a splash theme and startup items
	 * without interfering with already running evoke instance (and getting lock error)
	 *
	 * TODO: dryrun option is pretty dummy; better to use "test-splash" or similar
	 */
	if(do_startup) {
		service->read_startup(config_file);
		service->run_startup(show_splash, do_dryrun);
	}

	/* only testing, quit nicely */
	if(do_dryrun)
		return 0;

	if(!service->setup_lock(LOCK_FILE)) {
		printf("Either another evoke instance is running or I can't create lock file\n");
		printf("If program abnormaly crashed before, just remove '%s' and start it again\n", LOCK_FILE);
		return 1;
	}

	signal(SIGINT,  quit_signal);
	signal(SIGTERM, quit_signal);
	signal(SIGKILL, quit_signal);
	signal(SIGQUIT, quit_signal);

#ifdef USE_SIGHUP
	/*
	 * This is mostly used when we get request to shutdown session and close/kill all windows.
	 * If evoke is started from gui console (xterm, rxvt), closing that window we will get 
	 * SIGHUP since terminal will disconnect all controlling processes. On other hand, if evoke
	 * is started as session carrier (eg. run from xinitrc), this is not needed.
	 */
	signal(SIGHUP,  quit_signal);
#endif

	if(do_autostart || do_autostart_safe)
		perform_autostart(do_autostart_safe);

	service->start_xsettings_manager();

	/* set stuff so xsettings manager can receive events */
	XSelectInput(fl_display, RootWindow(fl_display, fl_screen), 
			PropertyChangeMask | SubstructureNotifyMask | ClientMessage);
	Fl::add_handler(xmessage_handler);

	service->start();

	while(service->running()) {
		/*
		 * FLTK will not report SelectionClear needed for XSETTINGS manager
		 * so we must catch it first before FLTK discards it (if we can :P)
		 * This can be missed greatly due a large number of XDamage events
		 * and using this method is not quite safe.
		 */
		if(fl_xevent && fl_xevent->type == SelectionClear)
			service->handle(fl_xevent);

		Fl::wait(FOREVER);
	}

	return 0;
}
