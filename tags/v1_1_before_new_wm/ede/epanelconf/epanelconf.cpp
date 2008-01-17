/*
 * $Id$
 *
 * Epanelconf, panel configuration tool
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include "epanelconf.h"
#include <efltk/Fl_File_Dialog.h>
#include <efltk/Fl_Config.h>
#include <efltk/Fl_Locale.h>
#include <edeconf.h>

#include "util.h"
extern Fl_Input *workspaces[8];

Fl_Window* panelWindow;

static void cb_Apply(Fl_Button*, void*) {
  write_config();
  send_workspaces();
}

static void cb_Close(Fl_Button*, void*) {
  Fl::first_window()->hide();
}

Fl_Input* vcProgram;

static void cb_Browse(Fl_Button*, void*) {
  char *file_types = _("Executables (*.*), *, All files (*.*), *");
  const char *fileName = fl_select_file(0, file_types, _("File selection..."));    
  if (fileName) { 
    vcProgram->value(fileName);
  }
;}

Fl_Input* tdProgram;

static void cb_Br(Fl_Button*, void*) {
  char *file_types = _("Executables (*.*), *, All files (*.*), *");
  const char *fileName = fl_select_file(0, file_types, _("File selection..."));    
  if (fileName) 
  { 
    tdProgram->value(fileName);
  }
;}

Fl_Check_Button* autohide_check;

Fl_Value_Slider* ws_slider;

static void cb_ws_slider(Fl_Value_Slider*, void*) {
  int val = int(ws_slider->value());
  for(int n=0; n<8; n++) {
    if(n<val)
      workspaces[n]->activate();
    else
      workspaces[n]->deactivate();
  }
;}

Fl_Input* browserProgram;

static void cb_Browse1(Fl_Button*, void*) {
  char *file_types = _("Executables (*.*), *, All files (*.*), *");
  const char *fileName = fl_select_file(0, file_types, _("File selection..."));    
  if (fileName) { 
    browserProgram->value(fileName);
  }
;}

Fl_Input* terminalProgram;

static void cb_Br1(Fl_Button*, void*) {
  char *file_types = _("Executables (*.*), *, All files (*.*), *");
  const char *fileName = fl_select_file(0, file_types, _("File selection..."));    
  if (fileName) { 
    terminalProgram->value(fileName);
  }
;}

int main (int argc, char **argv) {

  Fl_Window* w;
  fl_init_locale_support("epanelconf", PREFIX"/share/locale");
   {Fl_Window* o = panelWindow = new Fl_Window(405, 264, _("Panel settings"));
    w = o;
     {Fl_Button* o = new Fl_Button(237, 235, 80, 25, _("&Apply"));
      o->callback((Fl_Callback*)cb_Apply);
    }
     {Fl_Button* o = new Fl_Button(323, 235, 80, 25, _("&Close"));
      o->callback((Fl_Callback*)cb_Close);
    }
     {Fl_Tabs* o = new Fl_Tabs(0, 2, 403, 230);
       {Fl_Group* o = new Fl_Group(0, 22, 403, 208, _("Utilities"));
         {Fl_Group* o = new Fl_Group(5, 20, 395, 110, _("Panel utilities"));
          o->box(FL_ENGRAVED_BOX);
          o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
           {Fl_Input* o = vcProgram = new Fl_Input(10, 22, 285, 23, _("Volume control program:"));
            o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
          }
           {Fl_Button* o = new Fl_Button(305, 20, 80, 25, _("&Browse..."));
            o->callback((Fl_Callback*)cb_Browse);
          }
           {Fl_Input* o = tdProgram = new Fl_Input(10, 67, 285, 23, _("Time and date program:"));
            o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
          }
           {Fl_Button* o = new Fl_Button(305, 65, 80, 25, _("Br&owse..."));
            o->callback((Fl_Callback*)cb_Br);
          }
          o->end();
        }
         {Fl_Group* o = new Fl_Group(5, 150, 395, 35, _("Autohide"));
          o->box(FL_ENGRAVED_BOX);
          o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
          autohide_check = new Fl_Check_Button(5, 5, 385, 25, _("Automaticaly hide panel"));
          ;
          o->end();
        }
        o->end();
      }
       {Fl_Group* o = new Fl_Group(0, 22, 403, 208, _("Workspaces"));
        o->hide();
         {Fl_Value_Slider* o = ws_slider = new Fl_Value_Slider(145, 10, 255, 20, _("Number of workspaces: "));
          o->type(Fl_Value_Slider::HORIZONTAL|Fl_Slider::TICK_BELOW);
          o->box(FL_THIN_DOWN_BOX);
          o->button_box(FL_THIN_UP_BOX);
          o->step(1);
          o->callback((Fl_Callback*)cb_ws_slider);
          o->align(132);
          o->step(1); ;
          o->range(1,8);
        }
         {Fl_Group* o = new Fl_Group(5, 60, 395, 130, _("Workspace names:"));
          o->box(FL_ENGRAVED_BOX);
          o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
           {Fl_Input* o = new Fl_Input(55, 10, 115, 20, _("WS 1:"));
            o->deactivate();
            workspaces[0] = o; ;
          }
           {Fl_Input* o = new Fl_Input(55, 40, 115, 20, _("WS 2:"));
            o->deactivate();
            workspaces[1] = o; ;
          }
           {Fl_Input* o = new Fl_Input(55, 70, 115, 20, _("WS 3:"));
            o->deactivate();
            workspaces[2] = o; ;
          }
           {Fl_Input* o = new Fl_Input(55, 100, 115, 20, _("WS 4:"));
            o->deactivate();
            workspaces[3] = o; ;
          }
           {Fl_Input* o = new Fl_Input(260, 10, 115, 20, _("WS 5:"));
            o->deactivate();
            workspaces[4] = o; ;
          }
           {Fl_Input* o = new Fl_Input(260, 40, 115, 20, _("WS 6:"));
            o->deactivate();
            workspaces[5] = o; ;
          }
           {Fl_Input* o = new Fl_Input(260, 70, 115, 20, _("WS 7:"));
            o->deactivate();
            workspaces[6] = o; ;
          }
           {Fl_Input* o = new Fl_Input(260, 100, 115, 20, _("WS 8:"));
            o->deactivate();
            workspaces[7] = o; ;
          }
          ;
          o->end();
        }
        o->end();
      }
       {Fl_Group* o = new Fl_Group(0, 22, 403, 208, _("Handlers"));
        o->hide();
         {Fl_Group* o = new Fl_Group(5, 20, 395, 110, _("Handlers programs"));
          o->box(FL_ENGRAVED_BOX);
          o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
           {Fl_Input* o = browserProgram = new Fl_Input(10, 22, 285, 23, _("Internet browser:"));
            o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
          }
           {Fl_Button* o = new Fl_Button(300, 20, 80, 25, _("&Browse..."));
            o->callback((Fl_Callback*)cb_Browse1);
          }
           {Fl_Input* o = terminalProgram = new Fl_Input(10, 67, 285, 23, _("Terminal:"));
            o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
          }
           {Fl_Button* o = new Fl_Button(300, 65, 80, 25, _("Br&owse..."));
            o->callback((Fl_Callback*)cb_Br1);
          }
          ;
          o->end();
        }
        o->end();
      }
      o->end();
    }
    o->end();
  }
  read_config();
  update_workspaces();
  w->show(argc, argv);
  return  Fl::run();
}