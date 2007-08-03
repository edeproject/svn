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

#include "Log.h"
#include "Logout.h"
#include "EvokeService.h"
#include "Splash.h"
#include "Spawn.h"

#include <edelib/File.h>
#include <edelib/Config.h>
#include <edelib/DesktopFile.h>
#include <edelib/Directory.h>
#include <edelib/StrUtil.h>
#include <edelib/Util.h>
#include <edelib/Debug.h>
#include <edelib/Nls.h>

#include <FL/fl_ask.h>

#include <sys/types.h> // getpid
#include <unistd.h>    // 
#include <stdlib.h>    // free
#include <string.h>    // strdup, memset

void resolve_path(const edelib::String& imgdir, edelib::String& img, bool have_imgdir) {
	if(img.empty())
		return;

	const char* im = img.c_str();

	if(!edelib::file_exists(im) && have_imgdir) {
		img = edelib::build_filename("/", imgdir.c_str(), im);
		im = img.c_str();
		if(!edelib::file_exists(im)) {
			// no file, send then empty
			img.clear();
		}
	}
}

int get_int_property_value(Atom at) {
	Atom real;
	int format;
	unsigned long n, extra;
	unsigned char* prop;

	int status = XGetWindowProperty(fl_display, RootWindow(fl_display, fl_screen),
			at, 0L, 0x7fffffff, False, XA_CARDINAL, &real, &format, &n, &extra,
			(unsigned char**)&prop);
	int ret = -1;
	if(status != Success && !prop)
		return ret;
	ret = int(*(long*)prop);
	XFree(prop);
	return ret;
}

int get_string_property_value(Atom at, char* txt, int txt_len) {
	XTextProperty names;
	XGetTextProperty(fl_display, RootWindow(fl_display, fl_screen), &names, at);
	if(!names.nitems || !names.value)
		return 0;

	char** vnames;
	int nsz = 0;
	if(!XTextPropertyToStringList(&names, &vnames, &nsz)) {
		XFree(names.value);
		return 0;
	}

	strncpy(txt, vnames[0], txt_len);
	txt[txt_len] = '\0';
	XFreeStringList(vnames);
	return 1;
}

EvokeService::EvokeService() : is_running(0), logfile(NULL), pidfile(NULL), lockfile(NULL) { 
	top_win = NULL;
}

EvokeService::~EvokeService() {
	if(logfile)
		delete logfile;

	if(lockfile) {
		edelib::file_remove(lockfile);
		free(lockfile);
	}

	if(pidfile) {
		edelib::file_remove(pidfile);
		free(pidfile);
	}
}

EvokeService* EvokeService::instance(void) {
	static EvokeService es;
	return &es;
}

bool EvokeService::setup_logging(const char* file) {
	if(!file)
		logfile = new DummyLog();
	else
		logfile = new RealLog();

	if(!logfile->open(file)) {
		delete logfile;
		return false;
	}

	return true;
}

bool EvokeService::setup_pid(const char* file, const char* lock) {
	if(!file)
		return false;

	if(edelib::file_exists(lock))
		return false;

	FILE* l = fopen(lock, "w");
	if(!l)
		return false;
	fprintf(l, " ");
	fclose(l);
	lockfile = strdup(lock);

	FILE* f = fopen(file, "w");
	if(!f)
		return false;

	fprintf(f, "%i", getpid());
	fclose(f);

	pidfile = strdup(file);
	return true;
}

bool EvokeService::init_splash(const char* config, bool no_splash, bool dry_run) {
	edelib::Config c;
	if(!c.load(config))
		return false;

	char buff[1024];
	bool have_imgdir = false;

	c.get("evoke", "ImagesDirectory", buff, sizeof(buff));

	// no evoke section
	if(c.error() == edelib::CONF_ERR_SECTION)
		return false;

	edelib::String imgdir;
	if(c.error() == edelib::CONF_SUCCESS) {
		imgdir = buff;
		have_imgdir = true;
	}

	edelib::String splashimg;
	if(c.get("evoke", "Splash", buff, sizeof(buff)))
		splashimg = buff;

	// Startup key must exists
	if(!c.get("evoke", "Startup", buff, sizeof(buff)))
		return false;

	StringList vs;
	edelib::stringtok(vs, buff, ",");

	// nothing, fine, do nothing
	unsigned int sz = vs.size();
	if(sz == 0)
		return true;

	EvokeClient ec;
	const char* key_name;

	for(StringListIter it = vs.begin(); it != vs.end(); ++it) {
		key_name = (*it).c_str();
		edelib::str_trim((char*)key_name);

		// probably listed but not the same key; also Exec value must exists
		if(!c.get(key_name, "Exec", buff, sizeof(buff)))
			continue;
		else
			ec.exec = buff;

		if(c.get(key_name, "Description", buff, sizeof(buff)))
			ec.desc = buff;

		if(c.get(key_name, "Icon", buff, sizeof(buff)))
			ec.icon = buff;

		clients.push_back(ec);
	}

	/*
	 * Now, before data is send to Splash, resolve directories
	 * since Splash expects that.
	 */
	resolve_path(imgdir, splashimg, have_imgdir);
	for(ClientListIter it = clients.begin(); it != clients.end(); ++it)
		resolve_path(imgdir, (*it).icon, have_imgdir);

	Splash sp(no_splash, dry_run);
	sp.set_clients(&clients);
	sp.set_background(&splashimg);

	sp.run();

	return true;
}

/*
 * This is implementation of Autostart Spec (http://standards.freedesktop.org/autostart-spec/autostart-spec-0.5.html).
 * The Autostart Directories are $XDG_CONFIG_DIRS/autostart.
 * If the same filename is located under multiple Autostart Directories 
 * only the file under the most important directory should be used.
 *  Example: If $XDG_CONFIG_HOME is not set the Autostart Directory in the user's home 
 *  directory is ~/.config/autostart/
 *  Example: If $XDG_CONFIG_DIRS is not set the system wide Autostart Directory 
 *  is /etc/xdg/autostart/
 *  Example: If $XDG_CONFIG_HOME and $XDG_CONFIG_DIRS are not set and the two 
 *  files /etc/xdg/autostart/foo.desktop and ~/.config/autostart/foo.desktop exist 
 *  then only the file ~/.config/autostart/foo.desktop will be used because ~/.config/autostart/ 
 *  is more important than /etc/xdg/autostart/
 * If Hidden key is set true in .desktop file, file MUST be ignored.
 * OnlyShowIn and NotShowIn (list of strings identifying desktop environments) if (or if not)
 * contains environment name, MUST not be started/not started.
 * TryExec is same as for .desktop spec.
 */
void EvokeService::init_autostart(void) {
	edelib::String home = edelib::user_config_dir();
	home += "/autostart/";
}

void EvokeService::setup_atoms(Display* d) {
	// with them must be send '1' or property will be ignored (except _EDE_EVOKE_SPAWN)
	_ede_shutdown_all    = XInternAtom(d, "_EDE_EVOKE_SHUTDOWN_ALL", False);
	_ede_evoke_quit      = XInternAtom(d, "_EDE_EVOKE_QUIT", False);

	_ede_spawn           = XInternAtom(d, "_EDE_EVOKE_SPAWN", False);
}

void EvokeService::quit_x11(void) {
	int ret = fl_ask(_("Nice quitting is not implemented yet and this will forcefully kill\nall running applications. Make sure to save what needs to be saved.\nSo, would you like to continue ?"));
	if(ret)
		stop();
#if 0
	/*
	 * This code is working, but not as I would like to see.
	 * It will (mostly) call XKillClient() for _every_ window
	 * (including i.e. buttons in some app), and that is not
	 * nice way to say good bye. This must be implemented in
	 * wm since it only knows what is actuall window and what not. 
	 * For now quit_x11() will simply quit itself, quitting X11
	 * session (if it holds it), which will in turn forcefully kill
	 * all windows.
	 */
	Window dummy, *wins;
	Window root = RootWindow(fl_display, fl_screen);
	unsigned int n;

	if(!XQueryTree(fl_display, root, &dummy, &dummy, &wins, &n))
		return;

	Atom _wm_protocols     = XInternAtom(fl_display, "WM_PROTOCOLS", False);
	Atom _wm_delete_window = XInternAtom(fl_display, "WM_DELETE_WINDOW", False);
	Atom* protocols;
	int np;
	XEvent ev;
	bool have_quit = 0;

	for(unsigned int i = 0; i < n; i++) {
		if(wins[i] == root)
			continue;

		have_quit = 0;
		if(XGetWMProtocols(fl_display, wins[i], &protocols, &np)) {
			for(int j = 0; j < np; j++) {
				if(protocols[j] == _wm_delete_window) {
					have_quit = 1;
					break;
				}
			}
		}

		if(have_quit) {
			memset(&ev, 0, sizeof(ev));
			ev.xclient.type = ClientMessage;
			ev.xclient.window = wins[i];
			ev.xclient.message_type = _wm_protocols;
			ev.xclient.format = 32;
			ev.xclient.data.l[0] = (long)_wm_delete_window;
			ev.xclient.data.l[1] = (long)fl_event_time;
			XSendEvent(fl_display, wins[i], False, 0L, &ev);
		} else {
			XKillClient(fl_display, wins[i]);
		}
	}

	XFree((void*)wins);
	stop();
#endif
}

int EvokeService::handle(const XEvent* ev) {
	logfile->printf("Got event %i\n", ev->type);

	if(ev->type == MapNotify) {
		if(top_win) {
			// for splash to keep it at top (working in old edewm)
			XRaiseWindow(fl_display, fl_xid(top_win));
			return 1;
		}
	} else if(ev->type == ConfigureNotify) {
	 	if(ev->xconfigure.event == DefaultRootWindow(fl_display) && top_win) {
			// splash too, but keep window under other wm's
			XRaiseWindow(fl_display, fl_xid(top_win));
			return 1;
		}
	} else if(ev->type == PropertyNotify) {
		if(ev->xproperty.atom == _ede_spawn) {
			char buff[1024];

			if(get_string_property_value(_ede_spawn, buff, sizeof(buff))) {
				logfile->printf("Got _EVOKE_SPAWN with %s. Starting client...\n", buff);
				int r = spawn_program(buff);

				if(r != 0)
					fl_alert("Unable to start %s. Got code %i", buff, r);
			} else
				logfile->printf("Got _EVOKE_SPAWN with malformed data. Ignoring...\n");

			return 1;
		}

		if(ev->xproperty.atom == _ede_evoke_quit) {
			int val = get_int_property_value(_ede_evoke_quit);
			if(val == 1) {
				logfile->printf("Got accepted _EDE_EVOKE_QUIT\n");
				stop();
			} else
				logfile->printf("Got _EDE_EVOKE_QUIT with bad code (%i). Ignoring...\n", val);
			return 1;
		}

		if(ev->xproperty.atom == _ede_shutdown_all) {
			int val = get_int_property_value(_ede_shutdown_all);
			if(val == 1) {
				logfile->printf("Got accepted _EDE_EVOKE_SHUTDOWN_ALL\n");

				int dw = DisplayWidth(fl_display, fl_screen);
				int dh = DisplayHeight(fl_display, fl_screen);

				printf("got %i\n", logout_dialog(dw, dh));
				//quit_x11();
			} else	
				logfile->printf("Got _EDE_EVOKE_SHUTDOWN_ALL with bad code (%i). Ignoring...\n", val);
			return 1;
		}
	}

	return 0;
}