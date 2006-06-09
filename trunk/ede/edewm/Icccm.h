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

#ifndef _ICCCM_H_
#define _ICCCM_H_

class Frame;
class WindowManager;

class ICCCM
{
public:
    static void state_iconic(Frame *f);
    static void state_normal(Frame *f);
    static void state_withdrawn(Frame *f);

    static void configure(Frame *f);
    static void set_iconsizes(WindowManager* wm);

    static bool get_size(Frame *f, int &w, int &h);
    static char *get_title(Frame *f);

    static bool size_hints(Frame *f);
};

#endif

