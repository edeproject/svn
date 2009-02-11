// generated by Fast Light User Interface Designer (fluid) version 2.0001

#include "CubeViewUI.h"

inline void CubeViewUI::cb_vrot_i(Fl_Roller* o, void*) {
  cube->v_angle(((Fl_Roller *)o)->value());
  cube->redraw();
}
void CubeViewUI::cb_vrot(Fl_Roller* o, void* v) {
  ((CubeViewUI*)(o->parent()->parent()->parent()->user_data()))->cb_vrot_i(o,v);
}

inline void CubeViewUI::cb_ypan_i(Fl_Slider* o, void*) {
  cube->pany(((Fl_Slider *)o)->value());
  cube->redraw();
}
void CubeViewUI::cb_ypan(Fl_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->parent()->parent()->user_data()))->cb_ypan_i(o,v);
}

inline void CubeViewUI::cb_xpan_i(Fl_Slider* o, void*) {
  cube->panx(((Fl_Slider *)o)->value());
  cube->redraw();
}
void CubeViewUI::cb_xpan(Fl_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->parent()->parent()->user_data()))->cb_xpan_i(o,v);
}

inline void CubeViewUI::cb_hrot_i(Fl_Roller* o, void*) {
  cube->h_angle(((Fl_Roller *)o)->value());
  cube->redraw();
}
void CubeViewUI::cb_hrot(Fl_Roller* o, void* v) {
  ((CubeViewUI*)(o->parent()->parent()->parent()->user_data()))->cb_hrot_i(o,v);
}

inline void CubeViewUI::cb_zoom_i(Fl_Value_Slider* o, void*) {
  cube->size=((Fl_Value_Slider *)o)->value();
  cube->redraw();
}
void CubeViewUI::cb_zoom(Fl_Value_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->parent()->user_data()))->cb_zoom_i(o,v);
}

CubeViewUI::CubeViewUI() {
  Fl_Window* w;
   {Fl_Window* o = mainWindow = new Fl_Window(498, 442, "CubeView");
    w = o;
    o->user_data((void*)(this));
     {Fl_Group* o = new Fl_Group(5, 3, 454, 435);
       {Fl_Group* o = VChange = new Fl_Group(0, 104, 37, 213);
         {Fl_Roller* o = vrot = new Fl_Roller(0, 0, 17, 186, "V Rot");
          o->label_type(FL_NO_LABEL);
          o->minimum(-180);
          o->maximum(180);
          o->step(1);
          o->callback((Fl_Callback*)cb_vrot);
          o->align(FL_ALIGN_WRAP);
        }
         {Fl_Slider* o = ypan = new Fl_Slider(20, 0, 17, 186);
          o->label_type(FL_NO_LABEL);
          o->label_color((Fl_Color)136);
          o->minimum(-25);
          o->maximum(25);
          o->step(0.1);
          o->callback((Fl_Callback*)cb_ypan);
          o->align(FL_ALIGN_CENTER);
        }
        o->end();
      }
       {Fl_Group* o = HChange = new Fl_Group(132, 395, 236, 40);
         {Fl_Slider* o = xpan = new Fl_Slider(2, 2, 186, 17);
          o->type(Fl_Slider::HORIZONTAL);
          o->label_type(FL_NO_LABEL);
          o->label_color((Fl_Color)136);
          o->minimum(25);
          o->maximum(-25);
          o->step(0.1);
          o->callback((Fl_Callback*)cb_xpan);
          o->align(FL_ALIGN_CENTER);
        }
         {Fl_Roller* o = hrot = new Fl_Roller(2, 21, 186, 17, "H Rotation");
          o->type(Fl_Roller::HORIZONTAL);
          o->label_type(FL_NO_LABEL);
          o->minimum(-180);
          o->maximum(180);
          o->step(1);
          o->callback((Fl_Callback*)cb_hrot);
          o->align(FL_ALIGN_RIGHT);
        }
        o->end();
      }
       {Fl_Group* o = MainView = new Fl_Group(41, 24, 413, 369);
         {Fl_Box* o = cframe = new Fl_Box(0, 0, 413, 369);
          o->box(FL_DOWN_BOX);
          o->color((Fl_Color)4);
          o->selection_color((Fl_Color)69);
        }
         {CubeView* o = cube = new CubeView(2, 2, 409, 365);
          Fl_Group::current()->resizable(o);
        }
        o->end();
        Fl_Group::current()->resizable(o);
      }
       {Fl_Value_Slider* o = zoom = new Fl_Value_Slider(115, 0, 281, 19, "Zoom");
        o->type(Fl_Value_Slider::HORIZONTAL);
        o->label_font(fl_fonts+1);
        o->label_color((Fl_Color)136);
        o->minimum(1);
        o->maximum(50);
        o->step(0.1);
        o->value(10);
        o->callback((Fl_Callback*)cb_zoom);
        o->align(FL_ALIGN_LEFT);
      }
      o->end();
      Fl_Group::current()->resizable(o);
    }
    o->end();
  }
}

void CubeViewUI::show() {
  mainWindow->show();
}
