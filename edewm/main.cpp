/*
 * $Id$
 *
 * Edewm, window manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include "Windowmanager.h"
#include "Frame.h"
#include "Desktop.h"
#include "config.h"
#include "debug.h"
#include <edeconf.h>

#include <efltk/filename.h>
#include <efltk/fl_draw.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

bool got_signal = false;

void DBG(const char *str, ...)
{
	#ifdef _DEBUG
	fprintf(stderr, "edewm debug: ");

	va_list args;
	va_start(args, str);
	vfprintf(stderr, str, args);
	va_end(args);

	fprintf(stderr, "\n");
	fflush(stderr);
	#endif
}

void exit_signal(int signum)
{
	DBG("EDEWM: Exiting (got signal %d)\n", signum);
	got_signal = true;
}

int main(int argc, char ** argv)
{
	signal(SIGTERM, exit_signal);
	signal(SIGKILL, exit_signal);
	signal(SIGINT, exit_signal);

	Fl::args(argc, argv);

	fl_init_locale_support("edewm", PREFIX"/share/locale");

	WindowManager::init(argc, argv);
	Fl_Style::load_theme();

	while(WindowManager::instance()->running() && !got_signal)
	{
		Fl::wait();
		WindowManager::instance()->idle();
	}

	Frame_List l(map_order);
	for(uint n=0; n<l.size(); n++)
	{
		Frame *f = l[n];
		delete f;
	}

	WindowManager::shutdown();
	XCloseDisplay(fl_display);
	return 0;
}
