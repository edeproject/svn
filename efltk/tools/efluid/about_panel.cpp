// generated by Extended Fast Light User Interface Designer (eFluid) version 2.0003

#include "about_panel.h"

Fl_Double_Window* about_panel;
Fl_Group* display_group;

static void cb_2002(Fl_Button*, void*) {
  display_group->hide();
  copyright_box->show();
}
Fl_Box* copyright_box;

static void cb_OK(Fl_Return_Button* o, void*) {
  o->window()->hide();
}

Fl_Double_Window* make_about_panel(const char *copyright) {
  Fl_Double_Window* w;
   {Fl_Double_Window* o = about_panel = new Fl_Double_Window(330, 300, _("about fluid"));
    w = o;
    o->type(241);
    o->color((Fl_Color)97);
    o->selection_color((Fl_Color)47);
    o->shortcut(0xff1b);
    ((Fl_Window*)(o))->hotspot(o);
     {Fl_Group* o = display_group = new Fl_Group(0, 0, 330, 300);
       {Fl_Group* o = new Fl_Group(5, 145, 150, 150);
        o->box(FL_ROUND_UP_BOX);
        o->label_font(fl_fonts+1);
        o->label_type(FL_SHADOW_LABEL);
        o->color((Fl_Color)14);
        o->label_color((Fl_Color)6);
        o->selection_color((Fl_Color)47);
        o->label_size(40);
        o->align(FL_ALIGN_INSIDE);
         {Fl_Box* o = new Fl_Box(10, 10, 130, 130, _("eFluid"));
          o->box(FL_ROUND_DOWN_BOX);
          o->label_font(fl_fonts+1);
          o->label_type(FL_EMBOSSED_LABEL);
          o->color((Fl_Color)12);
          o->label_size(34);
          o->align(FL_ALIGN_INSIDE);
          o->parent()->resizable(o);
        }
        o->resizable(o);
        o->end();
      }
       {Fl_Group* o = new Fl_Group(175, 5, 150, 150);
        o->box(FL_ROUND_UP_BOX);
        o->label_font(fl_fonts+1);
        o->color((Fl_Color)14);
        o->label_color((Fl_Color)7);
        o->selection_color((Fl_Color)47);
        o->label_size(18);
         {Fl_Box* o = new Fl_Box(10, 10, 130, 130, _("EFLTK User\nInterface\nDesigner\nVersion 2.0.3"));
          o->box(FL_ROUND_DOWN_BOX);
          o->label_font(fl_fonts+1);
          o->label_type(FL_SHADOW_LABEL);
          o->color((Fl_Color)12);
          o->label_color((Fl_Color)7);
          o->selection_color((Fl_Color)47);
          o->label_size(16);
          o->parent()->resizable(o);
        }
        o->resizable(o);
        o->end();
      }
       {Fl_Button* o = new Fl_Button(10, 10, 135, 105, _("\302\251""2002-2003\nClick here for more information"));
        o->box(FL_BORDER_BOX);
        o->label_font(fl_fonts+1);
        o->color((Fl_Color)136);
        o->label_color((Fl_Color)6);
        o->highlight_color((Fl_Color)0xcc00);
        o->highlight_label_color((Fl_Color)6);
        o->callback((Fl_Callback*)cb_2002);
        o->align(FL_ALIGN_WRAP);
      }
      o->resizable(o);
      o->end();
      Fl_Group::current()->resizable(o);
    }
     {Fl_Box* o = copyright_box = new Fl_Box(0, 0, 330, 300);
      o->label_color((Fl_Color)6);
      o->label_size(10);
      o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_WRAP|FL_ALIGN_INSIDE);
      o->hide();
      o->label(copyright);
    }
     {Fl_Return_Button* o = new Fl_Return_Button(250, 265, 70, 25, _("OK"));
      o->label_size(10);
      o->shortcut(0xff0d);
      o->callback((Fl_Callback*)cb_OK);
    }
    o->size_range(o->w(), o->h());
    o->set_non_modal();
    o->end();
  }
  return w;
}
