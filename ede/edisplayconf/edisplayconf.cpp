/*
 * $Id$
 *
 * Edisplayconf, utility for display configuration
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */


#include "edisplayconf.h"
#include <stdio.h>
#include <stdlib.h>
#include "conf.h"
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Util.h>

#include <edeconf.h>

inline void edisplayconf::cb_win_i(Fl_Window*, void*) {
  _finish = true;
}
void edisplayconf::cb_win(Fl_Window* o, void* v) {
  ((edisplayconf*)(o->user_data()))->cb_win_i(o,v);
}

inline void edisplayconf::cb_Test_i(Fl_Button*, void*) {
  testbellCB();
}
void edisplayconf::cb_Test(Fl_Button* o, void* v) {
  ((edisplayconf*)(o->parent()->parent()->parent()->user_data()))->cb_Test_i(o,v);
}

inline void edisplayconf::cb_but_activate_i(Fl_Button*, void*) {
  TestBlankCB();
}
void edisplayconf::cb_but_activate(Fl_Button* o, void* v) {
  ((edisplayconf*)(o->parent()->parent()->parent()->user_data()))->cb_but_activate_i(o,v);
}

inline void edisplayconf::cb_radio_blank_i(Fl_Round_Button*, void*) {
  radioCB();
}
void edisplayconf::cb_radio_blank(Fl_Round_Button* o, void* v) {
  ((edisplayconf*)(o->parent()->parent()->parent()->parent()->user_data()))->cb_radio_blank_i(o,v);
}

inline void edisplayconf::cb_radio_pattern_i(Fl_Round_Button*, void*) {
  radioCB();
}
void edisplayconf::cb_radio_pattern(Fl_Round_Button* o, void* v) {
  ((edisplayconf*)(o->parent()->parent()->parent()->parent()->user_data()))->cb_radio_pattern_i(o,v);
}

inline void edisplayconf::cb_Cancel_i(Fl_Button*, void*) {
  cancelCB();
}
void edisplayconf::cb_Cancel(Fl_Button* o, void* v) {
  ((edisplayconf*)(o->parent()->user_data()))->cb_Cancel_i(o,v);
}

inline void edisplayconf::cb_but_kbd_apply_i(Fl_Button*, void*) {
  applyCB();
}
void edisplayconf::cb_but_kbd_apply(Fl_Button* o, void* v) {
  ((edisplayconf*)(o->parent()->user_data()))->cb_but_kbd_apply_i(o,v);
}

inline void edisplayconf::cb_OK_i(Fl_Return_Button*, void*) {
  okCB();
}
void edisplayconf::cb_OK(Fl_Return_Button* o, void* v) {
  ((edisplayconf*)(o->parent()->user_data()))->cb_OK_i(o,v);
}

edisplayconf::edisplayconf() {
  Fl_Window* w;
   {Fl_Window* o = win = new Fl_Window(265, 335, _("Display configuration"));
    w = o;
    o->callback((Fl_Callback*)cb_win, (void*)(this));
     {Fl_Tabs* o = new Fl_Tabs(5, 5, 255, 285);
       {Fl_Group* o = group_mouse = new Fl_Group(0, 20, 255, 265, _("Mouse"));
        o->align(FL_ALIGN_LEFT);
         {Fl_Value_Slider* o = slider_accel = new Fl_Value_Slider(25, 25, 220, 18, _("Acceleration"));
          o->type(Fl_Value_Slider::HORIZONTAL);
          o->maximum(10);
          o->step(1);
          o->value(2);
          o->align(FL_ALIGN_TOP);
        }
         {Fl_Value_Slider* o = slider_thresh = new Fl_Value_Slider(25, 65, 220, 18, _("Threshold (pixels)"));
          o->type(Fl_Value_Slider::HORIZONTAL);
          o->maximum(20);
          o->step(1);
          o->value(4);
          o->align(FL_ALIGN_TOP);
        }
        o->end();
      }
       {Fl_Group* o = group_bell = new Fl_Group(0, 20, 255, 265, _("Bell"));
        o->hide();
         {Fl_Value_Slider* o = slider_volume = new Fl_Value_Slider(25, 25, 220, 18, _("Volume in %"));
          o->type(Fl_Value_Slider::HORIZONTAL);
          o->maximum(100);
          o->step(1);
          o->value(50);
          o->align(FL_ALIGN_TOP);
        }
         {Fl_Value_Slider* o = slider_pitch = new Fl_Value_Slider(25, 70, 220, 18, _("Pitch in Hz"));
          o->type(Fl_Value_Slider::HORIZONTAL);
          o->minimum(100);
          o->maximum(1000);
          o->step(1);
          o->value(440);
          o->align(FL_ALIGN_TOP);
        }
         {Fl_Value_Slider* o = slider_duration = new Fl_Value_Slider(25, 115, 220, 18, _("Duration in ms"));
          o->type(Fl_Value_Slider::HORIZONTAL);
          o->maximum(1000);
          o->step(1);
          o->value(200);
          o->align(FL_ALIGN_TOP);
        }
         {Fl_Button* o = new Fl_Button(180, 165, 60, 25, _("Test"));
          o->callback((Fl_Callback*)cb_Test);
        }
        o->end();
      }
       {Fl_Group* o = group_keyboard = new Fl_Group(0, 20, 255, 265, _("Keyboard"));
        o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
        o->hide();
         {Fl_Check_Button* o = check_autorepeat = new Fl_Check_Button(24, 25, 221, 20, _("  Repeat key activated"));
          o->selection_color((Fl_Color)2);
          o->value(1);
        }
         {Fl_Value_Slider* o = slider_click = new Fl_Value_Slider(25, 65, 220, 18, _("Click volume %"));
          o->type(Fl_Value_Slider::HORIZONTAL);
          o->maximum(100);
          o->step(1);
          o->value(50);
          o->align(FL_ALIGN_TOP);
        }
        o->end();
      }
       {Fl_Group* o = group_screen = new Fl_Group(0, 20, 255, 265, _("Screen"));
        o->hide();
         {Fl_Button* o = but_activate = new Fl_Button(165, 187, 80, 25, _("&Test"));
          o->callback((Fl_Callback*)cb_but_activate);
        }
        new Fl_Box(10, 1, 234, 55);
         {Fl_Value_Slider* o = slider_delay = new Fl_Value_Slider(25, 77, 220, 18, _("Activation delay (min)"));
          o->type(Fl_Value_Slider::HORIZONTAL);
          o->minimum(5);
          o->maximum(120);
          o->step(1);
          o->value(15);
          o->align(FL_ALIGN_TOP);
        }
         {Fl_Check_Button* o = check_blanking = new Fl_Check_Button(23, 25, 222, 20, _(" Screen blanker activated"));
          o->selection_color((Fl_Color)2);
          o->value(1);
        }
        new Fl_Box(10, 100, 234, 37);
         {Fl_Value_Slider* o = slider_pattern = new Fl_Value_Slider(25, 152, 220, 18, _("Pattern change delay (min)"));
          o->type(Fl_Value_Slider::HORIZONTAL);
          o->minimum(1);
          o->maximum(5);
          o->step(0);
          o->value(2);
          o->align(FL_ALIGN_TOP);
          o->deactivate();
        }
         {Fl_Group* o = new Fl_Group(30, 105, 210, 25);
           {Fl_Round_Button* o = radio_blank = new Fl_Round_Button(0, 5, 105, 20, _("Blank"));
            o->type(Fl_Round_Button::RADIO);
            o->selection_color((Fl_Color)1);
            o->value(1);
            o->callback((Fl_Callback*)cb_radio_blank);
          }
           {Fl_Round_Button* o = radio_pattern = new Fl_Round_Button(115, 5, 95, 20, _("Pattern"));
            o->type(Fl_Round_Button::RADIO);
            o->selection_color((Fl_Color)1);
            o->callback((Fl_Callback*)cb_radio_pattern);
          }
          o->end();
        }
        o->end();
      }
      o->end();
    }
     {Fl_Button* o = new Fl_Button(190, 300, 70, 25, _("&Cancel"));
      o->callback((Fl_Callback*)cb_Cancel);
      o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    }
     {Fl_Button* o = but_kbd_apply = new Fl_Button(115, 300, 70, 25, _("&Apply"));
      o->callback((Fl_Callback*)cb_but_kbd_apply);
    }
     {Fl_Return_Button* o = new Fl_Return_Button(40, 300, 70, 25, _("OK"));
      o->shortcut(0xd);
      o->callback((Fl_Callback*)cb_OK);
    }
    o->end();
  }
}

edisplayconf::~edisplayconf() {
}

void edisplayconf::Run() {
  _finish = false;
  	win->show();
  	while (!_finish) Fl::wait();
  	win->hide();
}

void edisplayconf::make_window() {
}

void edisplayconf::radioCB() {
  int l_blank;
  
    l_blank = (int)radio_blank->value();
    if (l_blank)
      slider_pattern->deactivate();
    else
      slider_pattern->activate();
}

int main(int ac,char **av) {
  fl_init_locale_support("edisplayconf", PREFIX"/share/locale");
  app = new edisplayconf();
  read_disp_configuration();
  app->Run();
  delete app;
  return 0;
}
edisplayconf *app;
