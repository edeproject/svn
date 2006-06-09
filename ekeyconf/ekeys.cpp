/*
 * $Id$
 *
 * EControl applet for keyboard shortcuts
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */
#include "ekeys.h"

#define	NR_HOTKEYS	30


static struct {
	char systemname[20];
	char uiname[50];
	char command[50];
} hotkeys[] = {
	{"NextWindow",		"Next window", ""},
	{"PreviousWindow",	"Previous window", ""},
	{"NextDesktop",		"Next workspace", ""},
	{"PreviousDesktop",	"Previous workspace", ""},
	{"FastRun",		"Run program", ""},
	{"FindUtil",		"Find file", ""},
	{"CloseWindow",		"Close window", ""},
	{"MinimizeWindow",	"Minimize window", ""},
	{"MaximizeWindow",	"Maximize window", ""},
	{"Desktop1",		"Workspace 1", ""},
	{"Desktop2",		"Workspace 2", ""},
	{"Desktop3",		"Workspace 3", ""},
	{"Desktop4",		"Workspace 4", ""},
	{"Desktop5",		"Workspace 5", ""},
	{"Desktop6",		"Workspace 6", ""},
	{"Desktop7",		"Workspace 7", ""},
	{"Desktop8",		"Workspace 8", ""},
	{"App1",		"", ""},
	{"App2",		"", ""},
	{"App3",		"", ""},
	{"App4",		"", ""},
	{"App5",		"", ""},
	{"App6",		"", ""},
	{"App7",		"", ""},
	{"App8",		"", ""},
	{"App9",		"", ""},
	{"App10",		"", ""},
	{"App11",		"", ""},
	{"App12",		"", ""},

};



int keycodes[NR_HOTKEYS];




static void sendClientMessage(Window w, Atom a, long x)
{
  XEvent ev;
  long mask;

  memset(&ev, 0, sizeof(ev));
  ev.xclient.type = ClientMessage;
  ev.xclient.window = w;
  ev.xclient.message_type = a;
  ev.xclient.format = 32;
  ev.xclient.data.l[0] = x;
  ev.xclient.data.l[1] = CurrentTime;
  mask = 0L;
  if (w == RootWindow(fl_display, fl_screen))
    mask = SubstructureRedirectMask;	   
  XSendEvent(fl_display, w, False, mask, &ev);
}

void sendUpdateInfo() 
{
    unsigned int i, nrootwins;
    Window dw1, dw2, *rootwins = 0;
    int screen_count = ScreenCount(fl_display);
    extern Atom FLTKChangeSettings;
    for (int s = 0; s < screen_count; s++) {
        Window root = RootWindow(fl_display, s);
        XQueryTree(fl_display, root, &dw1, &dw2, &rootwins, &nrootwins);
        for (i = 0; i < nrootwins; i++) {
            if (rootwins[i]!=RootWindow(fl_display, fl_screen)) {
                sendClientMessage(rootwins[i], FLTKChangeSettings, 0);
            }
        }
    }
    XFlush(fl_display);
}

int getshortcutfor(Fl_String action)
{
    for (int i=0; i<NR_HOTKEYS; i++)
        if (action == Fl_String(hotkeys[i].uiname)) return keycodes[i];

    return 0;
}

void setshortcutfor(Fl_String action, int svalue)
{
    for (int i=0; i<NR_HOTKEYS; i++)
        if (action == Fl_String(hotkeys[i].uiname)) keycodes[i] = svalue;
}

int name_to_svalue(char *hotkey)
{
	static struct {
		char *name;
		int value;
	} keys[] = {
		{"alt",  	FL_ALT},
		{"ctrl",  	FL_CTRL},
		{"shift",  	FL_SHIFT},
		{"win",  	FL_WIN},
		{"space",  	FL_Space},
		{"backspace",	FL_BackSpace},
		{"tab",  	FL_Tab},
		{"enter",  	FL_Enter},
		{"escape",  	FL_Escape},
		{"home",  	FL_Home},
		{"left",  	FL_Left},
		{"up",  	FL_Up},
		{"right",  	FL_Right},
		{"down",  	FL_Down},
		{"pageup",  	FL_Page_Up},
		{"pagedown",  	FL_Page_Down},
		{"end",  	FL_End},
		{"insert",  	FL_Insert},
		{"delete",  	FL_Delete},
		{"f1",  	FL_F(1)},
		{"f2",  	FL_F(2)},
		{"f3",  	FL_F(3)},
		{"f4",  	FL_F(4)},
		{"f5",  	FL_F(5)},
		{"f6",  	FL_F(6)},
		{"f7",  	FL_F(7)},
		{"f8",  	FL_F(8)},
		{"f9",  	FL_F(9)},
		{"f10",  	FL_F(10)},
		{"f11",  	FL_F(11)},
		{"f12",  	FL_F(12)},
		{0, 0}
	};
	int parsed = 0;
	char f[20];

	// The parser - case insensitive and hopefully robust
	Fl_String_List elements(hotkey, "+");
	for (unsigned int i=0; i<elements.count(); i++) {
		bool found = false;
		for (int j=0; keys[j].value; j++) {
			Fl_String buf = Fl_String(keys[j].name);
			if (!elements.item(i).casecmp(buf)) {
				parsed += keys[j].value;
				found = true;
			}
		}

		if (!found) {
		// use first letter as shortcut key
			strcpy(f, elements.item(i));
			if ((f[0] >= 'a') && (f[0] <= 'z')) {
				parsed += f[0];
			} else if ((f[0] >= 'A') && (f[0] <= 'Z')) {
				parsed += (f[0] - 'A' + 'a');
			}
		}
	}

	return parsed;
}

void readKeysConfiguration()
{
	Fl_Config globalConfig(fl_find_config_file("wmanager.conf", 0), true, false);
	globalConfig.set_section("Hotkeys");
	
	for (int i=0; i<NR_HOTKEYS; i++) {
		Fl_String tmp;
		globalConfig.read(hotkeys[i].systemname, tmp, "");
		keycodes[i] = name_to_svalue(tmp);
	}

	globalConfig.set_section("Applications");
	for (int i=0; i<NR_HOTKEYS; i++) {
		Fl_String tmp;
		if ((strncmp(hotkeys[i].systemname,"App",3) == 0) && (keycodes[i] != 0)) {
			globalConfig.read(hotkeys[i].systemname, tmp, "");
			if (tmp != "") strncpy(hotkeys[i].command, tmp, 50);
			if (keycodes[i]>0 && tmp != "") strncpy(hotkeys[i].uiname, hotkeys[i].systemname, 20);
		}
	}

	globalConfig.set_section("ApplicationNames");
	for (int i=0; i<NR_HOTKEYS; i++) {
		Fl_String tmp;
		if ((strncmp(hotkeys[i].systemname,"App",3) == 0) && (keycodes[i] != 0)) {
			globalConfig.read(hotkeys[i].systemname, tmp, "");
			if (tmp != "") strncpy(hotkeys[i].uiname, tmp, 50);
		}
	}




}

void writeKeysConfiguration()
{
	Fl_Config globalConfig(fl_find_config_file("wmanager.conf", 1));
	globalConfig.set_section("Hotkeys");
	
	for (int i=0; i<NR_HOTKEYS; i++)
		globalConfig.write(hotkeys[i].systemname, Fl::key_name(keycodes[i]));
	
	globalConfig.set_section("Applications");
	for (int i=0; i<NR_HOTKEYS; i++)
		if ((strncmp(hotkeys[i].systemname,"App",3) == 0) 
			&& (strcmp(hotkeys[i].uiname,"") != 0)  
			&& (strcmp(hotkeys[i].command,"") != 0))
				globalConfig.write(hotkeys[i].systemname, hotkeys[i].command);

	globalConfig.set_section("ApplicationNames");
	for (int i=0; i<NR_HOTKEYS; i++)
		if ((strncmp(hotkeys[i].systemname,"App",3) == 0) 
			&& (strcmp(hotkeys[i].uiname,"") != 0)  
			&& (strcmp(hotkeys[i].command,"") != 0))
				globalConfig.write(hotkeys[i].systemname, hotkeys[i].uiname);
}

void populatelist(Fl_Input_Browser *action) 
{
	action->clear();
	for (int i=0; i<NR_HOTKEYS; i++)
		if (strcmp(hotkeys[i].uiname,"") != 0) action->add ( hotkeys[i].uiname);
}

void addShortcut(const char *name, const char *cmd)
{
	if ((strcmp(name,"") !=0) && (strcmp(cmd,"") != 0)) {
		for (int i=0; i<NR_HOTKEYS; i++) {
			if ((strncmp(hotkeys[i].systemname,"App",3) == 0) && (strcmp(hotkeys[i].uiname,name) == 0)) {
				fl_alert(_("Shortcut already defined! Please use a different name"));
				return;
			}
		}
		for (int i=0; i<NR_HOTKEYS; i++) {
			if ((strncmp(hotkeys[i].systemname,"App",3) == 0) && (strcmp(hotkeys[i].uiname,"") == 0)) {
				strncpy(hotkeys[i].uiname, name, 50);
				strncpy(hotkeys[i].command, cmd, 50);
				return;
			}
		}
	}
	fl_alert(_("Maximum number of user shortcuts exceeded"));
}


void removeShortcut(Fl_String action)
{
	for (int i=0; i<NR_HOTKEYS; i++)
		if (action == Fl_String(hotkeys[i].uiname)) {
			keycodes[i]=0;
			if (strncmp(hotkeys[i].systemname,"App",3) == 0)
				strcpy(hotkeys[i].uiname,"");
		}
}
