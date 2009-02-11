/*
 * $Id$
 *
 * EControl applet for keyboard shortcuts
 * Part of Equinox Desktop Environment (EDE).
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

// -- copyied over from efluid (Fl_Menu_Type.cpp)
#include "Shortcut_Button.h"
#include <efltk/Fl_Button.h>
#include <efltk/fl_draw.h>
#include <efltk/Fl.h>


void Shortcut_Button::draw() {
  box()->draw(0, 0, w(), h(),
                   value() ? selection_color() : color());
  fl_font(text_font(), text_size());
  fl_color(value() ? selection_text_color() : text_color());
  fl_draw(Fl::key_name(svalue), 6, 0, w(), h(), FL_ALIGN_LEFT);
}

int Shortcut_Button::handle(int e) {
  when(0); type(TOGGLE);
  if (e == FL_KEY) {
    if (!value()) return 0;
    int v = Fl::event_text()[0];
    if (v > 32 && v < 0x7f || v > 0xa0 && v <= 0xff) {
      v = v | Fl::event_state()&(FL_WIN|FL_ALT|FL_CTRL);
    } else {
      v = Fl::event_state()&(FL_WIN|FL_ALT|FL_CTRL|FL_SHIFT) | Fl::event_key();
      if (v == FL_BackSpace && svalue) v = 0;
    }
    if (v != svalue) {svalue = v; do_callback(); redraw();}
    return 1;
  } else if (e == FL_UNFOCUS) {
    int c = changed(); value(0); if (c) set_changed();
    return 1;
  } else if (e == FL_FOCUS) {
    return value();
  } else {
    int r = Fl_Button::handle(e);
    if (e == FL_RELEASE && value() && Fl::focus() != this) take_focus();
    return r;
  }
}
