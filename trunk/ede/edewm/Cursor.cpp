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

#include "Cursor.h"
#include "Frame.h"
#include "Tracers.h"

#include <efltk/x.h>
#include <X11/cursorfont.h>

#include <assert.h>

CursorHandler::CursorHandler()
{
}

CursorHandler::~CursorHandler()
{
}

void CursorHandler::load(CursorShapeType s)
{
	TRACE_FUNCTION("void CursorHandler::load(CursorShapeType s)");

	st = s;
	if(st == FLTK_CURSORS)
	{
		cursors[CURSOR_DEFAULT] = FL_CURSOR_DEFAULT;
		cursors[CURSOR_MOVE]    = FL_CURSOR_MOVE;
		cursors[CURSOR_WAIT]    = FL_CURSOR_WAIT;
		cursors[CURSOR_HELP]    = FL_CURSOR_HELP;
		cursors[CURSOR_N]       = FL_CURSOR_NS;
		cursors[CURSOR_NE]      = FL_CURSOR_NESW;
		cursors[CURSOR_E]       = FL_CURSOR_WE;
		cursors[CURSOR_SE]      = FL_CURSOR_NWSE;
		cursors[CURSOR_S]       = FL_CURSOR_NS;
		cursors[CURSOR_SW]      = FL_CURSOR_NESW;
		cursors[CURSOR_W]       = FL_CURSOR_WE;
		cursors[CURSOR_NW]      = FL_CURSOR_NWSE;
	}
	else
	{
		cursors[CURSOR_DEFAULT] = XCreateFontCursor(fl_display, XC_left_ptr);
		cursors[CURSOR_MOVE]    = XCreateFontCursor(fl_display, XC_fleur);
		cursors[CURSOR_WAIT]    = XCreateFontCursor(fl_display, XC_coffee_mug);
		cursors[CURSOR_HELP]    = XCreateFontCursor(fl_display, XC_question_arrow);
		cursors[CURSOR_N]       = XCreateFontCursor(fl_display, XC_top_side);
		cursors[CURSOR_NE]      = XCreateFontCursor(fl_display, XC_top_right_corner);
		cursors[CURSOR_E]       = XCreateFontCursor(fl_display, XC_right_side);
		cursors[CURSOR_SE]      = XCreateFontCursor(fl_display, XC_bottom_right_corner);
		cursors[CURSOR_S]       = XCreateFontCursor(fl_display, XC_bottom_side);
		cursors[CURSOR_SW]      = XCreateFontCursor(fl_display, XC_bottom_left_corner);
		cursors[CURSOR_W]       = XCreateFontCursor(fl_display, XC_left_side);
		cursors[CURSOR_NW]      = XCreateFontCursor(fl_display, XC_top_left_corner);
	}
}

void CursorHandler::set_fltk_cursor(Frame* f, CursorType t)
{
	TRACE_FUNCTION("void CursorHandler::set_fltk_cursor(Frame* f, CursorType t)");
	if(t == curr_cursor)
		return;

	curr_cursor = t;

	assert(f != NULL);
	f->cursor((Fl_Cursor)cursors[curr_cursor]);
}

void CursorHandler::set_x_cursor(Frame* f, CursorType t)
{
	TRACE_FUNCTION("void CursorHandler::set_x_cursor(Frame* f, CursorType t)");
	if(t == curr_cursor)
		return;

	curr_cursor = t;

	assert(f != NULL);
	XDefineCursor(fl_display, fl_xid(f), cursors[curr_cursor]);
}

// only for root window
void CursorHandler::set_root_cursor(void)
{
	TRACE_FUNCTION("void CursorHandler::set_root_cursor(void)");

	root_window_cursor = XCreateFontCursor(fl_display, XC_left_ptr);
	XDefineCursor(fl_display, RootWindow(fl_display, fl_screen), root_window_cursor);
}
