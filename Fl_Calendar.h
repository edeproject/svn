/***************************************************************************
                          Fl_Calendar.h  -  description
                             -------------------
    begin                : Sun Aug 18 2002
    copyright            : (C) 2002 by Alexey Parshin
    email                : alexeyp@m7.tts-sf.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef __Fl_Calendar_H__
#define __Fl_Calendar_H__

#include <efltk/Fl_Popup_Window.h>
#include <efltk/Fl_Date_Time.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Button.h>

class Fl_Calendar : public Fl_Group {
   Fl_Group     *m_headerBox;
   Fl_Group     *m_buttonBox;
   Fl_Box       *m_monthNameBox;
   Fl_Box       *m_dayNameBoxes[7];
   Fl_Button    *m_dayButtons[31];
   Fl_Button    *m_switchButtons[4];
   Fl_Date_Time  m_date;
   Fl_String     m_headerLabel;
   int           m_activeButtonIndex;

   static void cbDayButtonClicked(Fl_Widget *,void *);
   static void cbSwitchButtonClicked(Fl_Widget *,void *);
public:
   Fl_Calendar(int x,int y,int w,int h,const char *lbl=0L);
   void layout();
   void measure(int& w,int& h) const;

   void date(Fl_Date_Time dt);
   Fl_Date_Time date() const;

   void dayButtonClicked(unsigned day);
   void switchButtonClicked(int monthChange);
};

class Fl_Popup_Calendar : public Fl_Popup_Window {
   friend class Fl_Calendar;
   Fl_Calendar *m_calendar;
   Fl_Widget   *m_dateControl;
protected:
   int  handle(int);
public:
   Fl_Popup_Calendar(Fl_Widget *dateControl=NULL);

   void      layout();
   void      clicked() { set_value(); }

   void      date(Fl_Date_Time dt) { m_calendar->date(dt); }
   Fl_Date_Time date() const       { return m_calendar->date(); }

   bool      popup();
};

#endif
