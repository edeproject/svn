#ifndef _WINHINTS_H_
#define _WINHINTS_H_

#include <X11/Xlib.h>
#include "Mwm.h"

//extern Atom _XA_SM_CLIENT_ID;

extern Atom _XA_UTF8_STRING;

// ICCCM
extern Atom _XA_WM_CLIENT_LEADER;
extern Atom _XA_WM_PROTOCOLS;
extern Atom _XA_WM_TAKE_FOCUS;
extern Atom _XA_WM_DELETE_WINDOW;
extern Atom _XA_WM_STATE;
extern Atom _XA_WM_CHANGE_STATE;
extern Atom _XA_WM_COLORMAP_WINDOWS;

//MWM
extern Atom _XATOM_MWM_HINTS;
extern Atom _XATOM_MOTIF_WM_INFO;

// Extended WM Hints:
#define _NET_WM_MOVERESIZE_SIZE_TOPLEFT      0
#define _NET_WM_MOVERESIZE_SIZE_TOP          1
#define _NET_WM_MOVERESIZE_SIZE_TOPRIGHT     2
#define _NET_WM_MOVERESIZE_SIZE_RIGHT        3
#define _NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT  4
#define _NET_WM_MOVERESIZE_SIZE_BOTTOM       5
#define _NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT   6
#define _NET_WM_MOVERESIZE_SIZE_LEFT         7
#define _NET_WM_MOVERESIZE_MOVE              8   /* Movement only */
//_NET_SUPPORTED, ATOM[]/32
extern Atom _XA_NET_SUPPORTED; 
//_NET_CLIENT_LIST, XA_WINDOW[]/32
extern Atom _XA_NET_CLIENT_LIST;
//_NET_CLIENT_LIST_STACKING, XA_WINDOW[]/32
extern Atom _XA_NET_CLIENT_LIST_STACKING;
//_NET_NUMBER_OF_DESKTOPS, CARDINAL/32
extern Atom _XA_NET_NUM_DESKTOPS;
//_NET_DESKTOP_GEOMETRY width,height, CARDINAL[2]/32 | C
extern Atom _XA_NET_DESKTOP_GEOMETRY;
//_NET_DESKTOP_VIEWPORT x,y, CARDINAL[][2]/32 | C
extern Atom _XA_NET_DESKTOP_VIEWPORT;
//_NET_CURRENT_DESKTOP <desktop>, CARDINAL[1]/32 | C
extern Atom _XA_NET_CURRENT_DESKTOP;
//_NET_DESKTOP_NAMES, UTF8
extern Atom _XA_NET_DESKTOP_NAMES;
//_NET_ACTIVE_WINDOW, WINDOW/32
extern Atom _XA_NET_ACTIVE_WINDOW;
//_NET_WORKAREA, CARDINAL[][4]/32
extern Atom _XA_NET_WORKAREA;
//_NET_SUPPORTING_WM_CHECK, XA_WINDOW/32
extern Atom _XA_NET_SUPPORTING_WM_CHECK;
//_NET_VIRTUAL_ROOTS, XA_WINDOW[]/32
extern Atom _XA_NET_VIRTUAL_ROOTS;
//_NET_CLOSE_WINDOW - closes active window
extern Atom _XA_NET_CLOSE_WINDOW;
//_NET_WM_MOVERESIZE | C
extern Atom _XA_NET_WM_MOVERESIZE;
//_NET_WM_NAME - UTF8
extern Atom _XA_NET_WM_NAME;
//_NET_WM_VISIBLE_NAME, UTF8
extern Atom _XA_NET_WM_VISIBLE_NAME;
//_NET_WM_ICON_NAME, UTF8
extern Atom _XA_NET_WM_ICON_NAME;
//_NET_WM_VISIBLE_ICON_NAME, UTF8
extern Atom _XA_NET_WM_ICON_VISIBLE_NAME;
//_NET_WM_DESKTOP <desktop>, CARDINAL/32 | C
extern Atom _XA_NET_WM_DESKTOP;
//_NET_WM_WINDOW_TYPE, ATOM[]/32
extern Atom _XA_NET_WM_WINDOW_TYPE;
extern Atom _XA_NET_WM_WINDOW_TYPE_DESKTOP;
extern Atom _XA_NET_WM_WINDOW_TYPE_DOCK;
extern Atom _XA_NET_WM_WINDOW_TYPE_TOOLBAR;
extern Atom _XA_NET_WM_WINDOW_TYPE_MENU;
extern Atom _XA_NET_WM_WINDOW_TYPE_UTIL;
extern Atom _XA_NET_WM_WINDOW_TYPE_SPLASH;
extern Atom _XA_NET_WM_WINDOW_TYPE_DIALOG; //Transient type
extern Atom _XA_NET_WM_WINDOW_TYPE_NORMAL; //Normal
//_NET_WM_STATE, ATOM[] | C
extern Atom _XA_NET_WM_STATE;
extern Atom _XA_NET_WM_STATE_MODAL; //Needs transient for (root for whole group)
extern Atom _XA_NET_WM_STATE_STICKY; //Pos fixed, even if virt. desk. scrolls
extern Atom _XA_NET_WM_STATE_MAXIMIZED_VERT;
extern Atom _XA_NET_WM_STATE_MAXIMIZED_HORZ;
extern Atom _XA_NET_WM_STATE_SHADED;
extern Atom _XA_NET_WM_STATE_SKIP_TASKBAR;
extern Atom _XA_NET_WM_STATE_SKIP_PAGER;
//_NET_WM_STRUT, CARDINAL[4]/32
extern Atom _XA_NET_WM_STRUT;
//_NET_WM_ICON_GEOMETRY, CARDINAL[4] - array of x,y,w,h of type CARDINAL, format 32
extern Atom _XA_NET_WM_ICON_GEOMETRY;
//_NET_WM_ICON CARDINAL[][2+n]/32 - 32bit packed CARDINAL ARGB with high byte being A, low byte being B
extern Atom _XA_NET_WM_ICON;
//_NET_WM_PID CARDINAL/32
extern Atom _XA_NET_WM_PID;
//_NET_WM_HANDLED_ICONS
extern Atom _XA_NET_WM_HANDLED_ICONS;
//_NET_WM_PING
extern Atom _XA_NET_WM_PING;
// the next one is not in the spec but KDE use it
extern Atom _XA_NET_WM_STATE_STAYS_ON_TOP;
	
// KDE1 icon support, thats all of it :)
extern Atom _XA_KWM_WIN_ICON;

extern void init_atoms();
extern void register_protocols(Window root);

#endif
