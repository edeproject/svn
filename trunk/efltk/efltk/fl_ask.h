/*
 * $Id$
 *
 * Extended Fast Light Toolkit (EFLTK)
 * Copyright (C) 2002-2003 by EDE-Team
 * WWW: http://www.sourceforge.net/projects/ede
 *
 * Fast Light Toolkit (FLTK)
 * Copyright (C) 1998-2003 by Bill Spitzak and others.
 * WWW: http://www.fltk.org
 *
 * This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
 * version 2. See COPYING for details.
 *
 * Author : Mikko Lahteenmaki
 * Email  : mikko@fltk.net
 *
 * Please report all bugs and problems to "efltk-bugs@fltk.net"
 *
 */

#ifndef _FL_ASK_H_
#define _FL_ASK_H_

#include "Enumerations.h"
#include "Fl_Font.h"
#include "Fl_Style.h"

class FL_API Fl_Widget;

/** BeepTypes */
enum BeepTypes {
    FL_BEEP_DEFAULT = 0,
    FL_BEEP_MESSAGE,
    FL_BEEP_ERROR,
    FL_BEEP_QUESTION,
    FL_BEEP_PASSWORD,
    FL_BEEP_NOTIFICATION
};

/** fl_beep */
FL_API void fl_beep(int type = FL_BEEP_DEFAULT);

/** fl_message */
FL_API void fl_message(const char *,...);

/** fl_alert */
FL_API void fl_alert(const char *,...);

/** fl_ask */
FL_API int fl_ask(const char *,...);

/** fl_choice */
FL_API int fl_choice(const char *q,const char *b0,const char *b1,const char *b2,...);

/** fl_input */
FL_API const char *fl_input(const char *label, const char *deflt = 0, ...);

/** fl_password */
FL_API const char *fl_password(const char *label, const char *deflt = 0, ...);

extern FL_API Fl_Named_Style* fl_icon_style;
extern FL_API Fl_Named_Style* fl_message_style;

#endif
