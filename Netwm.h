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

#ifndef _NETWM_H_
#define _NETWM_H_

#include <X11/Xlib.h>
class Frame;

class NETWM
{
public:
    static void get_strut(Frame *f);
    static bool get_window_type(Frame *f);
    static char *get_title(Frame *f);

    static void set_active_window(Window win);
};

#endif
