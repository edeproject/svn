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

#ifndef _ICON_H_
#define _ICON_H_

#include "Frame.h"

#include <efltk/Fl_Image.h>
#include <efltk/Fl_Renderer.h>
#include <efltk/Fl_Map.h>

typedef Fl_String_Ptr_Map ImageMap;

class Icon
{
public:
    Icon(XWMHints *wm_hints);
    ~Icon();

    Fl_Image *get_icon(int W, int H);

private:
    ImageMap images;
    Fl_Image *image, *mask;
};

#endif

