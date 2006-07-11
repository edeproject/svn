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

#include "Utils.h"
#include <efltk/x.h>

int x_return_code = 0;

void* GetProperty(Window w, Atom a, Atom type, unsigned long *np, int *ret)
{
	Atom realType;
	int format;
	unsigned long n, extra;
	unsigned char *prop=0;
	int status = XGetWindowProperty(fl_display, w, a, 0L, 0x7fffffff,
		False, type, &realType, &format, &n, &extra, (uchar**)&prop);
	if(ret) 
		*ret = status;
	if(status != Success) 
		return 0;
	if(!prop) 
		return 0;
	if(!n)
   	{
		XFree(prop); 
		return 0; 
	}
	if(np) 
		*np = n;
	return prop;
}

void SendMessage(Window win, Atom a, Atom l)
{
	XEvent ev;
	long mask;
	memset(&ev, 0, sizeof(ev));
	ev.xclient.type = ClientMessage;
	ev.xclient.window = win;
	ev.xclient.message_type = a;
	ev.xclient.format = 32;
	ev.xclient.data.l[0] = long(l);
	ev.xclient.data.l[1] = long(fl_event_time);
	mask = 0L;
	XSendEvent(fl_display, win, False, mask, &ev);
}

/*
void SetProperty(Window w, Atom a, Atom type, unsigned long val)
{
	XChangeProperty(fl_display, w, a, type, 32, PropModeReplace, (uchar*)&val,1);
}

void AppendProperty(Window w, Atom a, Atom type, unsigned long val)
{
	XChangeProperty(fl_display, w, a, type, 32, PropModeAppend, (uchar*)&val,1);
}

void DeleteProperty(Window w, Atom a, Atom type)
{
#warning DeleteProperty() smells like shit !
	XDeleteProperty(fl_display, w, a);
}
*/

