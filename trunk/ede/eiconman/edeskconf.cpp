/*
 * $Id$
 *
 * Eiconman, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include "edeskconf.h"
#include <efltk/Fl.h>
#include <efltk/fl_ask.h>
#include <efltk/Fl_Color_Chooser.h>
#include <efltk/Fl_Config.h>
#include <efltk/Fl_Image.h>
#include <efltk/Fl_File_Dialog.h>
#include "eiconman.h"

Fl_Window* bg_prop_window;

Fl_Box* mini_image_box;

Fl_Light_Button* color_button;

Fl_Check_Button* use_button;

Fl_Group* bg_image_group;

Fl_Input* image_input;

static Fl_Button* browse_button;

Fl_Value_Slider* opacity_slider;

Fl_Choice* mode_choice;

static void cb_Cancel(Fl_Button*, void*) {
  //bg_prop_window->do_callback();
  bg_prop_window->destroy();
}

void make_desktop_properties() {
  Fl_Window* w;
   {Fl_Window* o = bg_prop_window = new Fl_Window(560, 270, _("Background settings"));
    w = o;
    o->callback((Fl_Callback*)bg_prop_cb);
    ((Fl_Window*)(o))->hotspot(o);
     {Fl_Group* o = new Fl_Group(10, 20, 265, 205, _("Background"));
      o->box(FL_BORDER_FRAME);
      o->color((Fl_Color)39);
      o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
       {Fl_Box* o = new Fl_Box(1, 1, 263, 203);
        o->box(FL_BORDER_FRAME);
        o->color((Fl_Color)47);
      }
       {Fl_Group* o = new Fl_Group(20, 8, 230, 157);
         {Fl_Box* o = new Fl_Box(85, 135, 60, 7);
          o->box(FL_BORDER_BOX);
        }
         {Fl_Box* o = new Fl_Box(25, 6, 180, 131);
          o->box(FL_THIN_UP_BOX);
        }
         {Fl_Box* o = new Fl_Box(67, 140, 94, 12);
          o->box(FL_THIN_UP_BOX);
        }
         {Fl_Box* o = new Fl_Box(179, 127, 15, 6);
          o->box(FL_THIN_UP_BOX);
        }
         {Fl_Box* o = new Fl_Box(172, 127, 2, 5);
          o->box(FL_FLAT_BOX);
          o->color((Fl_Color)2);
        }
         {Fl_Box* o = new Fl_Box(35, 15, 160, 110);
          o->box(FL_THIN_DOWN_BOX);
        }
         {Fl_Box* o = mini_image_box = new Fl_Box(36, 16, 158, 108);
          o->box(FL_FLAT_BOX);
          o->button_box(FL_FLAT_BOX);
          o->color((Fl_Color)32);
          o->highlight_color((Fl_Color)32);
          o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
        }
        o->end();
      }
       {Fl_Light_Button* o = color_button = new Fl_Light_Button(170, 170, 85, 25, _("C&olor..."));
        o->type(0);
        o->box(FL_THIN_UP_BOX);
        o->value(1);
        o->callback((Fl_Callback*)bg_image_color);
      }
      o->end();
    }
     {Fl_Check_Button* o = use_button = new Fl_Check_Button(285, 20, 265, 20, _("Use wallpaper"));
      o->value(1);
      o->callback((Fl_Callback*)bg_image_use);
    }
    new Fl_Divider(290, 40, 255, 10, _("label"));
     {Fl_Group* o = bg_image_group = new Fl_Group(285, 50, 265, 175);
      o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
       {Fl_Input* o = image_input = new Fl_Input(15, 17, 240, 23, _("Image:"));
        o->callback((Fl_Callback*)bg_image_input);
        o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
        o->when(FL_WHEN_ENTER_KEY);
      }
       {Fl_Button* o = browse_button = new Fl_Button(175, 50, 80, 23, _("&Browse..."));
        o->callback((Fl_Callback*)bg_image_browse);
      }
       {Fl_Value_Slider* o = opacity_slider = new Fl_Value_Slider(60, 85, 195, 18, _("Opacity:"));
        o->type(Fl_Value_Slider::HORIZONTAL);
        o->maximum(255);
        o->step(1.01);
        o->slider_size(10);
        o->callback((Fl_Callback*)bg_image_opacity);
        o->align(FL_ALIGN_LEFT);
        o->when(FL_WHEN_RELEASE);
        o->step(1);
      }
       {Fl_Choice* o = mode_choice = new Fl_Choice(60, 114, 195, 23, _("Mode:")); o->begin();
        o->callback((Fl_Callback*)bg_image_mode);
         {Fl_Item* o = new Fl_Item(_("Center"));
          o->user_data((void*)(0));
        }
         {Fl_Item* o = new Fl_Item(_("Stretch"));
          o->user_data((void*)(1));
        }
         {Fl_Item* o = new Fl_Item(_("Stretch (aspect)"));
          o->user_data((void*)(2));
        }
         {Fl_Item* o = new Fl_Item(_("Tiled"));
          o->deactivate();
        }
        o->end();
      }
      o->end();
    }
    new Fl_Divider(5, 225, 550, 15, _("label"));
     {Fl_Button* o = new Fl_Button(290, 240, 80, 25, _("&OK"));
      o->callback((Fl_Callback*)bg_ok);
    }
     {Fl_Button* o = new Fl_Button(380, 240, 80, 25, _("&Apply"));
      o->callback((Fl_Callback*)bg_apply);
    }
     {Fl_Button* o = new Fl_Button(470, 240, 80, 25, _("&Cancel"));
      o->callback((Fl_Callback*)cb_Cancel);
    }
    o->end();
  }
  bg_prop_window->end();
}
