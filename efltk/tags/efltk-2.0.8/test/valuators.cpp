// generated by Fast Light User Interface Designer (fluid) version 2.0003

#include "valuators.h"

static void callback(Fl_Widget* o, void*) {
  printf("%g     \r", ((Fl_Valuator*)o)->value());
  fflush(stdout);
}

int main (int argc, char **argv) {

  Fl_Window* w;
   {Fl_Window* o = new Fl_Window(565, 490, "Valuator classes, showing values for the type()");
    w = o;
    o->label_size(10);
     {Fl_Box* o = new Fl_Box(10, 10, 280, 235, "Fl_Slider");
      o->box(FL_ENGRAVED_BOX);
      o->label_font(fl_fonts+1);
      o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
    }
     {Fl_Slider* o = new Fl_Slider(25, 49, 20, 157, "VERTICAL");
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
      o->align(FL_ALIGN_TOP);
    }
     {Fl_Slider* o = new Fl_Slider(60, 70, 20, 158, "VERTICAL|TICK_LEFT");
      o->type(Fl_Slider::VERTICAL|Fl_Slider::TICK_ABOVE);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Slider* o = new Fl_Slider(100, 49, 20, 157, "VERTICAL|TICK_RIGHT");
      o->type(Fl_Slider::VERTICAL|Fl_Slider::TICK_BELOW);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
      o->align(FL_ALIGN_TOP);
    }
     {Fl_Slider* o = new Fl_Slider(140, 54, 130, 16, "HORIZONTAL");
      o->type(Fl_Slider::HORIZONTAL);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Slider* o = new Fl_Slider(140, 81, 130, 22, "HORIZONTAL|TICK_ABOVE");
      o->type(Fl_Slider::HORIZONTAL|Fl_Slider::TICK_ABOVE);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Slider* o = new Fl_Slider(140, 119, 130, 22, "HORIZONTAL|TICK_ABOVE,box");
      o->type(Fl_Slider::HORIZONTAL|Fl_Slider::TICK_ABOVE);
      o->box(FL_DOWN_BOX);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Slider* o = new Fl_Slider(140, 157, 130, 22, "HORIZONTAL|TICK_BELOW");
      o->type(Fl_Slider::HORIZONTAL|Fl_Slider::TICK_BELOW);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Slider* o = new Fl_Slider(140, 201, 130, 22, "HORIZONTAL|TICK_BOTH");
      o->type(Fl_Slider::HORIZONTAL|Fl_Slider::TICK_BOTH);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Box* o = new Fl_Box(295, 10, 260, 126, "Fl_Value_Input");
      o->box(FL_ENGRAVED_BOX);
      o->label_font(fl_fonts+1);
      o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
    }
     {Fl_Value_Input* o = new Fl_Value_Input(360, 35, 180, 22, "outside label");
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->linesize(0);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Value_Input* o = new Fl_Value_Input(310, 63, 100, 22, "inside");
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
      o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    }
     {Fl_Value_Input* o = new Fl_Value_Input(410, 63, 65, 22, "x");
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->maximum(100);
      o->callback((Fl_Callback*)callback);
      o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    }
     {Fl_Value_Input* o = new Fl_Value_Input(475, 63, 65, 22, "y");
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->maximum(100);
      o->step(1);
      o->linesize(0);
      o->callback((Fl_Callback*)callback);
      o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    }
     {Fl_Box* o = new Fl_Box(10, 250, 280, 229, "Fl_Value_Slider");
      o->box(FL_ENGRAVED_BOX);
      o->label_font(fl_fonts+1);
      o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
    }
     {Fl_Box* o = new Fl_Box(295, 141, 145, 131, "   Fl_Scrollbar");
      o->box(FL_ENGRAVED_BOX);
      o->label_font(fl_fonts+1);
      o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    }
     {Fl_Scrollbar* o = new Fl_Scrollbar(300, 240, 105, 20, "HORIZONTAL");
      o->type(Fl_Scrollbar::HORIZONTAL);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->maximum(100);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Scrollbar* o = new Fl_Scrollbar(405, 145, 20, 115, "VERTICAL");
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->minimum(100);
      o->maximum(0);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Value_Slider* o = new Fl_Value_Slider(25, 277, 30, 158, "VERTICAL");
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->text_size(10);
      o->callback((Fl_Callback*)callback);
      o->align(FL_ALIGN_TOP);
    }
     {Fl_Value_Slider* o = new Fl_Value_Slider(65, 310, 30, 152, "VERTICAL|TICK_LEFT");
      o->type(Fl_Value_Slider::VERTICAL|Fl_Slider::TICK_ABOVE);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->text_size(10);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Value_Slider* o = new Fl_Value_Slider(105, 283, 35, 158, "VERTICAL|TICK_LEFT,box");
      o->type(Fl_Value_Slider::VERTICAL|Fl_Slider::TICK_ABOVE);
      o->box(FL_DOWN_BOX);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->text_size(10);
      o->callback((Fl_Callback*)callback);
      o->align(FL_ALIGN_TOP);
    }
     {Fl_Value_Slider* o = new Fl_Value_Slider(145, 294, 130, 21, "HORIZONTAL");
      o->type(Fl_Value_Slider::HORIZONTAL);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Value_Slider* o = new Fl_Value_Slider(145, 337, 130, 22, "HORIZONTAL|TICK_BELOW");
      o->type(Fl_Value_Slider::HORIZONTAL|Fl_Slider::TICK_BELOW);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Value_Slider* o = new Fl_Value_Slider(145, 381, 130, 21, "HORIZONTAL|TICK_BELOW,box");
      o->type(Fl_Value_Slider::HORIZONTAL|Fl_Slider::TICK_BELOW);
      o->box(FL_DOWN_BOX);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Value_Slider* o = new Fl_Value_Slider(145, 424, 130, 33, "HORIZONTAL|TICK_BOTH");
      o->type(Fl_Value_Slider::HORIZONTAL|Fl_Slider::TICK_BOTH);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Box* o = new Fl_Box(295, 277, 145, 136, "Fl_Roller");
      o->box(FL_ENGRAVED_BOX);
      o->label_font(fl_fonts+1);
      o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
    }
     {Fl_Roller* o = new Fl_Roller(305, 340, 90, 20, "HORIZONTAL");
      o->type(Fl_Roller::HORIZONTAL);
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Roller* o = new Fl_Roller(405, 299, 20, 103, "VERTICAL");
      o->color((Fl_Color)10);
      o->selection_color((Fl_Color)1);
      o->label_size(8);
      o->callback((Fl_Callback*)callback);
    }
     {Fl_Group* o = new Fl_Group(445, 141, 110, 272, "Fl_Dial");
      o->box(FL_ENGRAVED_BOX);
      o->label_font(fl_fonts+1);
      o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
       {Fl_Dial* o = new Fl_Dial(22, 24, 65, 65, "NORMAL");
        o->color((Fl_Color)10);
        o->selection_color((Fl_Color)1);
        o->label_size(8);
        o->callback((Fl_Callback*)callback);
        o->angles(0,315);
      }
       {Fl_Dial* o = new Fl_Dial(22, 104, 65, 65, "LINE");
        o->type(Fl_Dial::LINE);
        o->color((Fl_Color)10);
        o->selection_color((Fl_Color)1);
        o->label_size(8);
        o->callback((Fl_Callback*)callback);
      }
       {Fl_Dial* o = new Fl_Dial(22, 184, 65, 65, "FILL");
        o->type(Fl_Dial::FILL);
        o->color((Fl_Color)10);
        o->selection_color((Fl_Color)1);
        o->label_size(8);
        o->callback((Fl_Callback*)callback);
        o->angles(0,360);
      }
      o->end();
    }
     {Fl_Box* o = new Fl_Box(295, 419, 260, 60, "All widgets have color(green) and selection_color(red) to show the areas thes\
e colors affect.");
      o->box(FL_ENGRAVED_BOX);
      o->label_size(10);
      o->align(FL_ALIGN_WRAP);
    }
     {Fl_Value_Output* o = new Fl_Value_Output(430, 90, 110, 35, "Fl_Value_Output");
      o->value(0.15);
    }
    o->end();
    o->resizable(o);
  }
  w->show(argc, argv);
  return  Fl::run();
}
