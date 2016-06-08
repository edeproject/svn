//
// Harbour.hh for pekwm
// Copyright (C) 2003-2009 Claes Nasten <pekdon{@}pekdon{.}net>
//
// This program is licensed under the GNU GPL.
// See the LICENSE file for more information.
//

#include "../config.h"

#ifndef _HARBOUR_HH_
#define _HARBOUR_HH_

#include "pekwm.hh"

class PScreen;
class Theme;
class Workspaces;
class DockApp;
class Strut;
class BaseMenu;
class HarbourMenu;

#include "Action.hh"

#include <list>

class Harbour
{
public:
    Harbour(PScreen *s, Theme *t, Workspaces *w);
    ~Harbour(void);

    void addDockApp(DockApp* da);
    void removeDockApp(DockApp* da);
    void removeAllDockApps(void);

    DockApp* findDockApp(Window win);
    DockApp* findDockAppFromFrame(Window win);

    HarbourMenu* getHarbourMenu(void) { return _harbour_menu; }

    inline uint getSize(void) const { return _size; }

#ifdef HAVE_XRANDR
    void updateGeometry(void);
#endif // HAVE_XRANDR

    void restack(void);
    void rearrange(void);
    void loadTheme(void);
    void updateHarbourSize(void);

    void setStateHidden(StateAction sa);

    void handleButtonEvent(XButtonEvent* ev, DockApp* da);
    void handleMotionNotifyEvent(XMotionEvent* ev, DockApp* da);
    void handleConfigureRequestEvent(XConfigureRequestEvent* ev, DockApp* da);

private:
    void placeDockApp(DockApp *da);
    void placeDockAppsSorted(void);
    void placeDockAppInsideScreen(DockApp *da);

    void getPlaceStartPosition(DockApp *da, int &x, int &y, bool &inc_x);
    void insertDockAppSorted(DockApp *da);

    void updateStrutSize(void);

private:
    PScreen *_scr;
    Theme *_theme;
    Workspaces *_workspaces;

    std::list<DockApp*> _da_list;
    HarbourMenu *_harbour_menu;

    bool _hidden;
    uint _size;
    Strut *_strut;
    int _last_button_x, _last_button_y;
};

#endif // _HARBOUR_HH_
