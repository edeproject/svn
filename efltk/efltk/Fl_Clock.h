//
// "$Id$"
//
// Clock header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef Fl_Clock_H
#define Fl_Clock_H

#include "Fl_Widget.h"

// a Fl_Clock_Output can be used to display a program-supplied time:

class FL_API Fl_Clock_Output : public Fl_Widget {
public:
  enum {SQUARE = 0, ANALOG = 0, ROUND, DIGITAL};
  Fl_Clock_Output(int x,int y,int w,int h, const char *l = 0);
  void value(ulong v);	// set to this Unix time
  void value(int,int,int);	// set hour, minute, second
  ulong value() const {return value_;}
  int hour() const {return hour_;}
  int minute() const {return minute_;}
  int second() const {return second_;}
protected:
  void draw(int, int, int, int);
  void draw();
private:
  int hour_, minute_, second_;
  ulong value_;
  void drawhands(Fl_Color,Fl_Color); // part of draw
};

// a Fl_Clock displays the current time always by using a timeout:

class FL_API Fl_Clock : public Fl_Clock_Output {
public:
  int handle(int);
  void update();
  Fl_Clock(int x,int y,int w,int h, const char *l = 0);
  static Fl_Named_Style* default_style;
  ~Fl_Clock();
};

#ifndef FLTK_2
#define FL_SQUARE_CLOCK		Fl_Clock_Output::SQUARE
#define FL_ROUND_CLOCK		Fl_Clock_Output::ROUND
#define FL_ANALOG_CLOCK		Fl_Clock_Output::SQUARE
#define FL_DIGITAL_CLOCK	Fl_Clock_Oupput::DIGITAL
#endif

#endif

//
// End of "$Id$".
//
