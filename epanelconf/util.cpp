
#include "epanelconf.h"

#include <efltk/Fl_Input.h>
#include <efltk/x.h>
#include <efltk/Fl_WM.h>
#include <efltk/Fl_Config.h>
#include <efltk/Fl_String_List.h>

Fl_Input *workspaces[8];

void read_config()
{
    char temp_value[128];
    bool temp_bool=0;
    Fl_Config cfg(fl_find_config_file("ede.conf", 0));
    cfg.set_section("Panel");

    if(!cfg.read("Volume Control", temp_value, 0, sizeof(temp_value))) {
        vcProgram->value(temp_value);
    }

    if(!cfg.read("Time and date", temp_value, 0, sizeof(temp_value))) {
        tdProgram->value(temp_value);
    }

    cfg.read("AutoHide", temp_bool, false);
    autohide_check->value(temp_bool);

    cfg.read("ShowDesktop", temp_bool, false);
    showdesktop_check->value(temp_bool);
    cfg.read("Workspaces", temp_bool, true);
    workspace_check->value(temp_bool);
    cfg.read("QuickLaunchBar", temp_bool, true);
    qlb_check->value(temp_bool);
    cfg.read("RunBrowser", temp_bool, true);
    runbrowser_check->value(temp_bool);
    cfg.read("SoundMixer", temp_bool, true);
    soundmixer_check->value(temp_bool);
    cfg.read("CPUMonitor", temp_bool, true);
    cpumonitor_check->value(temp_bool);
    
    cfg.set_section("Web");
    if(!cfg.read("Browser", temp_value, 0, sizeof(temp_value))) {
        browserProgram->value(temp_value);
    }

    cfg.set_section("Terminal");
    if(!cfg.read("Terminal", temp_value, 0, sizeof(temp_value))) {
        terminalProgram->value(temp_value);
    }

}

void write_config()
{
    Fl_Config cfg(fl_find_config_file("ede.conf", 0));
    cfg.set_section("Panel");
    cfg.write("Volume Control", vcProgram->value());
    cfg.write("Time and date", tdProgram->value());
    cfg.write("AutoHide", autohide_check->value());

    cfg.write("ShowDesktop", showdesktop_check->value());
    cfg.write("Workspaces", workspace_check->value());
    cfg.write("QuickLaunchBar", qlb_check->value());
    cfg.write("RunBrowser", runbrowser_check->value());
    cfg.write("SoundMixer", soundmixer_check->value());
    cfg.write("CPUMonitor", cpumonitor_check->value());

    cfg.set_section("Web");
    cfg.write("Browser", browserProgram->value());
    cfg.set_section("Terminal");
    cfg.write("Terminal", terminalProgram->value());

    // Write workspace names to file, edewm will read and set on startup
    cfg.set_section("Workspaces");
    cfg.write("Count", int(ws_slider->value()));
    for(int n=0; n<8; n++) {
        char tmp[128]; snprintf(tmp, sizeof(tmp)-1, "Workspace%d", n+1);
        cfg.write(tmp, workspaces[n]->value());
    }
}

void get_workspaces(Fl_CString_List &desktops, int &count);
void update_workspaces()
{
    Fl_CString_List desktops;
    desktops.auto_delete(true);

    int count;
    get_workspaces(desktops, count);
    if(count>8) count=8;
    for(int n=0; n<8; n++) {
        const char *name = desktops.item(n);
        Fl_Input *i = workspaces[n];
        if(n<count) i->activate();
        if(name) {
            i->value(name);
        } else {
            char tmp[128];
            snprintf(tmp, sizeof(tmp)-1, "%s %d", "Workspace" ,n+1);
            i->value(tmp);
        }
    }
    ws_slider->value(count);
    desktops.clear();
}

/////////////////////////////////////
/////////////////////////////////////

static bool atoms_inited=false;

// NET-WM spec desktop atoms
static Atom _XA_NET_NUM_DESKTOPS;
static Atom _XA_NET_DESKTOP_NAMES;
// GNOME atoms:
static Atom _XA_WIN_WORKSPACE_COUNT;
static Atom _XA_WIN_WORKSPACE_NAMES;

static void init_atoms()
{
    if(atoms_inited) return;
    fl_open_display();

#define A(name) XInternAtom(fl_display, name, False)

    _XA_NET_NUM_DESKTOPS    = A("_NET_NUMBER_OF_DESKTOPS");
    _XA_NET_DESKTOP_NAMES   = A("_NET_DESKTOP_NAMES");

    _XA_WIN_WORKSPACE_COUNT = A("_WIN_WORKSPACE_COUNT");
    _XA_WIN_WORKSPACE_NAMES = A("_WIN_WORKSPACE_NAMES");

    atoms_inited=true;
}

void* getProperty(Window w, Atom a, Atom type, unsigned long* np=0)
{
    Atom realType;
    int format;
    unsigned long n, extra;
    int status;
    void* prop;
    status = XGetWindowProperty(fl_display, w,
                                a, 0L, 256L, False, type, &realType,
                                &format, &n, &extra, (uchar**)&prop);
    if (status != Success) return 0;
    if (!prop) return 0;
    if (!n) {XFree(prop); return 0;}
    if (np) *np = n;
    return prop;
}

int getIntProperty(Window w, Atom a, Atom type, int deflt) {
    void* prop = getProperty(w, a, type);
    if(!prop) return deflt;
    int r = int(*(long*)prop);
    XFree(prop);
    return r;
}

void setProperty(Window w, Atom a, Atom type, int v) {
    long prop = v;
    XChangeProperty(fl_display, w, a, type, 32, PropModeReplace, (uchar*)&prop,1);
}

void get_workspaces(Fl_CString_List &desktops, int &count)
{
    init_atoms();

    count = 0;
    ///current = 0;
    desktops.clear();
    desktops.auto_delete(true);

    int length=0;
    char *buffer=0;

    XTextProperty names;
    // First try to get NET desktop names
    XGetTextProperty(fl_display, RootWindow(fl_display, fl_screen), &names, _XA_NET_DESKTOP_NAMES);
    // If not found, look for GNOME ones
    if(!names.value) XGetTextProperty(fl_display, RootWindow(fl_display, fl_screen), &names, _XA_WIN_WORKSPACE_NAMES);
    buffer = (char *)names.value;
    length = names.nitems;

    if(buffer) {
        char* c = buffer;
        for (int i = 1; c < buffer+length; i++) {
            char* d = c;
            while(*d) d++;
            if(*c != '<') {
                if(strcmp(c, "")) {
                    desktops.append(strdup(c));
                }
            }
            c = d+1;
        }
        XFree(names.value);
    }

    count = getIntProperty(RootWindow(fl_display, fl_screen), _XA_NET_NUM_DESKTOPS, XA_CARDINAL, -1);
    if(count<0) count = getIntProperty(RootWindow(fl_display, fl_screen), _XA_WIN_WORKSPACE_COUNT, XA_CARDINAL, -1);
}

void send_workspaces()
{
    init_atoms();

    int cnt = int(ws_slider->value());

    // Tell windowmanager to update its internal desktop count
    Fl_WM::set_workspace_count(cnt);

    char *ws_names[8];
    for(int n=0; n<cnt; n++)
    {
        if(!strcmp(workspaces[n]->value(), "")) {
            char tmp[128];
            snprintf(tmp, sizeof(tmp)-1, "%s %d", "Workspace", n+1);
            ws_names[n] = strdup(tmp);
        } else
            ws_names[n] = strdup(workspaces[n]->value());
    }

    XTextProperty names;
    if(XStringListToTextProperty((char **)ws_names, cnt, &names)) {
        XSetTextProperty(fl_display, RootWindow(fl_display, fl_screen), &names, _XA_NET_DESKTOP_NAMES);
        XSetTextProperty(fl_display, RootWindow(fl_display, fl_screen), &names, _XA_WIN_WORKSPACE_NAMES);
        XFree(names.value);
    }
}

