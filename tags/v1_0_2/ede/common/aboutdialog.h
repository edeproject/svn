// Copyright (c) 2000. - 2005. EDE Authors
// This program is licenced under terms of the
// GNU General Public Licence version 2 or newer.
// See COPYING for details.

#ifndef _ABOUTDIALOG_H_
#define _ABOUTDIALOG_H_

#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Button.h>

class AboutDialog 
{
public:
	AboutDialog(const char *, const char *, const char *c = 0);
private:
	void showCopyingInfo();
	Fl_Window *aboutWindow;
};

#endif
