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
Fl_Check_Button* showdesktop_check;
Fl_Check_Button* workspace_check;
Fl_Check_Button* qlb_check;
Fl_Check_Button* runbrowser_check;
Fl_Check_Button* soundmixer_check;
Fl_Check_Button* cpumonitor_check;
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

static void cb_Browse(Fl_Button*, void*) {
  char *file_types = _("Executables (*.*), *, All files (*.*), *");
  const char *fileName = fl_select_file(0, file_types, _("File selection..."));    
  if (fileName) { 
    browserProgram->value(fileName);
  }
;}
Fl_Input* terminalProgram;

static void cb_B(Fl_Button*, void*) {
  char *file_types = _("Executables (*.*), *, All files (*.*), *");
  const char *fileName = fl_select_file(0, file_types, _("File selection..."));    
  if (fileName) { 
    terminalProgram->value(fileName);
  }
;}
Fl_Input* vcProgram;

static void cb_Br(Fl_Button*, void*) {
  char *file_types = _("Executables (*.*), *, All files (*.*), *");
  const char *fileName = fl_select_file(0, file_types, _("File selection..."));    
  if (fileName) { 
    vcProgram->value(fileName);
  }
;}
Fl_Input* tdProgram;

static void cb_Brow(Fl_Button*, void*) {
  char *file_types = _("Executables (*.*), *, All files (*.*), *");
  const char *fileName = fl_select_file(0, file_types, _("File selection..."));    
  if (fileName) 
  { 
    tdProgram->value(fileName);
  }
;}

int main (int argc, char **argv) {

  Fl_Window* w;
  fl_init_locale_support("epanelconf", PREFIX"/share/locale");
   {Fl_Window* o = panelWindow = new Fl_Window(405, 274, _("Panel settings"));
    w = o;
    o->shortcut(0xff1b);
     {Fl_Button* o = new Fl_Button(237, 245, 80, 25, _("&Apply"));
      o->callback((Fl_Callback*)cb_Apply);
    }
     {Fl_Button* o = new Fl_Button(323, 245, 80, 25, _("&Close"));
      o->callback((Fl_Callback*)cb_Close);
    }
     {Fl_Tabs* o = new Fl_Tabs(0, 2, 403, 238);
      o->color((Fl_Color)0xfffffffe);
       {Fl_Group* o = new Fl_Group(1, 25, 401, 212, _("&Options"));
         {Fl_Group* o = new Fl_Group(5, 20, 395, 93, _("Panel applets"));
          o->box(FL_ENGRAVED_BOX);
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
          showdesktop_check = new Fl_Check_Button(9, 8, 160, 25, _("Show &desktop button"));
          workspace_check = new Fl_Check_Button(9, 33, 160, 25, _("Wor&kspace switcher"));
          qlb_check = new Fl_Check_Button(9, 58, 160, 25, _("&Quick launch bar"));
          runbrowser_check = new Fl_Check_Button(189, 8, 160, 25, _("&Command line"));
          soundmixer_check = new Fl_Check_Button(189, 58, 160, 25, _("&Sound mixer"));
          cpumonitor_check = new Fl_Check_Button(189, 33, 160, 25, _("&Processor load graph"));
          o->end();
        }
         {Fl_Group* o = new Fl_Group(5, 133, 395, 35, _("Autohide"));
          o->box(FL_ENGRAVED_BOX);
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
          autohide_check = new Fl_Check_Button(5, 5, 385, 25, _("&Automatically hide panel"));
          ;
          o->end();
        }
         {Fl_Box* o = new Fl_Box(9, 173, 385, 35, _("NOTE: To apply changes, restart the panel (e.g. by logging out)"));
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        }
        o->end();
      }
       {Fl_Group* o = new Fl_Group(1, 25, 401, 212, _("&Workspaces"));
        o->hide();
         {Fl_Value_Slider* o = ws_slider = new Fl_Value_Slider(145, 10, 255, 20, _("&Number of workspaces: "));
          o->type(Fl_Value_Slider::HORIZONTAL|Fl_Slider::TICK_BELOW);
          o->box(FL_THIN_DOWN_BOX);
          o->button_box(FL_THIN_UP_BOX);
          o->step(1);
          o->callback((Fl_Callback*)cb_ws_slider);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
          o->step(1); ;
          o->range(1,8);
        }
         {Fl_Group* o = new Fl_Group(5, 60, 395, 130, _("Workspace names:"));
          o->box(FL_ENGRAVED_BOX);
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
           {Fl_Input* o = new Fl_Input(55, 10, 115, 20, _("WS &1:"));
            o->deactivate();
            workspaces[0] = o; ;
          }
           {Fl_Input* o = new Fl_Input(55, 40, 115, 20, _("WS &2:"));
            o->deactivate();
            workspaces[1] = o; ;
          }
           {Fl_Input* o = new Fl_Input(55, 70, 115, 20, _("WS &3:"));
            o->deactivate();
            workspaces[2] = o; ;
          }
           {Fl_Input* o = new Fl_Input(55, 100, 115, 20, _("WS &4:"));
            o->deactivate();
            workspaces[3] = o; ;
          }
           {Fl_Input* o = new Fl_Input(260, 10, 115, 20, _("WS &5:"));
            o->deactivate();
            workspaces[4] = o; ;
          }
           {Fl_Input* o = new Fl_Input(260, 40, 115, 20, _("WS &6:"));
            o->deactivate();
            workspaces[5] = o; ;
          }
           {Fl_Input* o = new Fl_Input(260, 70, 115, 20, _("WS &7:"));
            o->deactivate();
            workspaces[6] = o; ;
          }
           {Fl_Input* o = new Fl_Input(260, 100, 115, 20, _("WS &8:"));
            o->deactivate();
            workspaces[7] = o; ;
          }
          ;
          o->end();
        }
        o->end();
      }
       {Fl_Group* o = new Fl_Group(1, 25, 401, 212, _("&Utilities"));
        o->hide();
         {Fl_Group* o = new Fl_Group(5, 20, 395, 188, _("Handlers programs"));
          o->box(FL_ENGRAVED_BOX);
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
           {Fl_Input* o = browserProgram = new Fl_Input(10, 22, 285, 23, _("&Internet browser:"));
            o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
          }
           {Fl_Button* o = new Fl_Button(304, 20, 80, 25, _("&Browse..."));
            o->callback((Fl_Callback*)cb_Browse);
          }
           {Fl_Input* o = terminalProgram = new Fl_Input(10, 67, 285, 23, _("&Terminal:"));
            o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
          }
           {Fl_Button* o = new Fl_Button(304, 65, 80, 25, _("B&rowse..."));
            o->callback((Fl_Callback*)cb_B);
          }
           {Fl_Input* o = vcProgram = new Fl_Input(9, 110, 285, 23, _("&Volume control program:"));
            o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
          }
           {Fl_Button* o = new Fl_Button(304, 108, 80, 25, _("Br&owse..."));
            o->callback((Fl_Callback*)cb_Br);
          }
           {Fl_Input* o = tdProgram = new Fl_Input(9, 155, 285, 23, _("Time and &date program:"));
            o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
          }
           {Fl_Button* o = new Fl_Button(304, 153, 80, 25, _("Brow&se..."));
            o->callback((Fl_Callback*)cb_Brow);
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
