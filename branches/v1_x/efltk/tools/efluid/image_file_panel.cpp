// generated by Extended Fast Light User Interface Designer (eFluid) version 2.0003

Fl_Window* images_dir_window;

static void cb_images_dir_window(Fl_Window*, void*) {
  images_dir_window->hide();
  modal=0;
}
Fl_Input* images_dir_input;

static void cb_Browse(Fl_Button*, void*) {
  browse_dir_cb();
}

static void cb_OK(Fl_Return_Button*, void*) {
  images_dir_window->hide();
  modal=0;
}

static void cb_Cancel(Fl_Button*, void*) {
  images_dir_window->hide();
  modal=0; cancel=1;
}

Fl_Window* make_images_dir_window() {
  Fl_Window* w;
   {Fl_Window* o = images_dir_window = new Fl_Window(310, 90, _("Images root directory"));
    w = o;
    o->shortcut(0xff1b);
    o->callback((Fl_Callback*)cb_images_dir_window);
     {Fl_Input* o = images_dir_input = new Fl_Input(10, 14, 220, 21);
      ((Fl_Window*)(o->parent()))->hotspot(o);
    }
     {Fl_Button* o = new Fl_Button(230, 14, 70, 21, _("Browse ..."));
      o->callback((Fl_Callback*)cb_Browse);
    }
     {Fl_Box* o = new Fl_Box(10, 35, 220, 15, _("(relative to the location of the .fl file)"));
      o->label_size(10);
      o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
    }
     {Fl_Return_Button* o = new Fl_Return_Button(160, 65, 70, 20, _("OK"));
      o->shortcut(0xff0d);
      o->callback((Fl_Callback*)cb_OK);
    }
     {Fl_Button* o = new Fl_Button(235, 65, 70, 20, _("Cancel"));
      o->callback((Fl_Callback*)cb_Cancel);
    }
    o->window_type(Fl_WM::UTIL);
    o->end();
  }
  return  w;
}
