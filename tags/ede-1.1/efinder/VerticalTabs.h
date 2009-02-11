/*
 * $Id$
 *
 * Efinder, file search tool
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */


#ifndef _VERTICALTABS_H_
#define _VERTICALTABS_H_

#include <efltk/Fl_Group.h>
#include <efltk/Fl.h>
#include <efltk/Fl_Tooltip.h>
#include <efltk/fl_draw.h>

class VerticalTabs : public Fl_Group {

public:
  int handle(int);

  VerticalTabs(int,int,int,int,const char * = 0);
//  static Fl_Named_Style* default_style;

  int value();
  bool value(int);
  int which(int event_x, int event_y);
//  Fl_Widget *selected_child();
//  bool selected_child(Fl_Widget *);

protected:
  void draw();
  void layout();

private:
  int push(Fl_Widget *);
//  int tab_positions(int*, int*);
//  int tab_height();
  int drawtab(int number, int height, Fl_Widget* o, bool selected);
  void drawblank();
};

#endif

//
// End of "$Id$".
//
