// generated by Fast Light User Interface Designer (fluid) version 2,0003

#include "inactive.h"

Fl_Group* the_group;

static void cb_active(Fl_Button*, void*) {
  the_group->activate();
}

static void cb_inactive(Fl_Button*, void*) {
  the_group->deactivate();
}

int main (int argc, char **argv) {

  Fl_Window* w;
   {Fl_Window* o = new Fl_Window(395, 280);
    w = o;
     {Fl_Group* o = the_group = new Fl_Group(5, 6, 384, 239, "activate()/deactivate() called on this Fl_Group");
      o->box(FL_ENGRAVED_BOX);
      o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
      new Fl_Button(25, 27, 105, 22, "button");
       {Fl_Group* o = new Fl_Group(25, 75, 105, 105, "Child group");
         {Fl_Check_Button* o = new Fl_Check_Button(0, 0, 105, 25, "red");
          o->type(Fl_Check_Button::RADIO);
          o->label_color((Fl_Color)1);
          o->selection_color((Fl_Color)1);
          o->selection_text_color((Fl_Color)1);
          o->highlight_label_color((Fl_Color)1);
        }
         {Fl_Check_Button* o = new Fl_Check_Button(0, 25, 105, 25, "green");
          o->type(Fl_Check_Button::RADIO);
          o->label_color((Fl_Color)2);
          o->selection_color((Fl_Color)2);
          o->selection_text_color((Fl_Color)2);
          o->highlight_label_color((Fl_Color)2);
        }
         {Fl_Check_Button* o = new Fl_Check_Button(0, 50, 105, 25, "blue");
          o->type(Fl_Check_Button::RADIO);
          o->label_color((Fl_Color)4);
          o->selection_color((Fl_Color)4);
          o->selection_text_color((Fl_Color)4);
          o->highlight_label_color((Fl_Color)4);
        }
         {Fl_Check_Button* o = new Fl_Check_Button(0, 75, 105, 25, "white");
          o->type(Fl_Check_Button::RADIO);
          o->label_color((Fl_Color)7);
          o->selection_color((Fl_Color)7);
          o->selection_text_color((Fl_Color)55);
          o->highlight_label_color((Fl_Color)55);
        }
        o->end();
      }
       {Fl_Slider* o = new Fl_Slider(140, 25, 24, 150, "Fl_Slider");
        o->value(0.5);
      }
       {Fl_Input* o = new Fl_Input(170, 27, 195, 22);
        o->static_value("Fl_Input");
      }
       {Fl_Input_Browser* o = new Fl_Input_Browser(255, 60, 110, 22, "input browser"); o->begin();
        new Fl_Item("Moe");
        new Fl_Item("Larry");
        new Fl_Item("Curly");
        o->value("Shemp");
        o->end();
      }
       {Fl_Box* o = new Fl_Box(215, 137, 145, 88, "Fl_Box");
        o->box(FL_ENGRAVED_BOX);
        o->label_font(fl_fonts+3);
        o->label_type(FL_SHADOW_LABEL);
        o->label_size(38);
      }
       {Fl_Value_Output* o = new Fl_Value_Output(230, 93, 135, 33, "value:");
        o->text_font(fl_fonts+5);
        o->text_color((Fl_Color)4);
        o->text_size(24);
        o->maximum(10000);
        o->step(1);
        o->tooltip("This Fl_Value_Output widget has a tooltip.");
      }
       {Fl_Scrollbar* o = new Fl_Scrollbar(15, 195, 180, 20, "scrollbar");
        o->type(Fl_Scrollbar::HORIZONTAL);
        o->maximum(100);
      }
      o->end();
      Fl_Group::current()->resizable(o);
    }
     {Fl_Button* o = new Fl_Button(5, 250, 190, 25, "active");
      o->type(Fl_Button::RADIO);
      o->value(1);
      o->callback((Fl_Callback*)cb_active);
    }
     {Fl_Button* o = new Fl_Button(205, 250, 184, 25, "inactive");
      o->type(Fl_Button::RADIO);
      o->callback((Fl_Callback*)cb_inactive);
    }
    o->end();
  }
  w->show(argc, argv);
  return  Fl::run();
}
