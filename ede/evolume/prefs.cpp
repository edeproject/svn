// Copyright (c) 2000. - 2005. EDE Authors
// This program is licenced under terms of the
// GNU General Public Licence version 2 or newer.
// See COPYING for details.

#include "prefs.h"
#include <malloc.h>
#include <efltk/Fl_Item.h>
#include <efltk/filename.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Config.h>
extern char device[1024];
extern int mixer_device;

void choice_items(char *path) {
  Fl_Item         *new_Item;
      dirent          **files;
      int             num_Files = 0;
  
      num_Files = fl_filename_list(path, &files);
  
      if (num_Files > 0) {
  
          for (int i = 0; i < num_Files; i ++) {
              if (strcmp(files[i]->d_name, ".") != 0 &&
                  strcmp(files[i]->d_name, "..") != 0) {
  
                  char filename[FL_PATH_MAX];
                  snprintf(filename, sizeof(filename)-1, "%s/%s", path, files[i]->d_name);
  
                  if (!fl_is_dir(filename) && fl_file_match(files[i]->d_name, "mixer*")) {
                      new_Item = new Fl_Item();
  					new_Item->copy_label(filename);
                  }
              }
              free(files[i]);
          }
          free(files);
      }
}

Fl_Window* preferenciesWindow;

Fl_Input_Browser* deviceNameInput;

static void cb_OK(Fl_Button*, void*) {
  Fl_Config globalConfig("EDE Team", "evolume");
  globalConfig.set("Sound mixer", "Device", deviceNameInput->value());
  snprintf(device, sizeof(device)-1, "%s", (char*)deviceNameInput->value());
  
  mixer_device = open(device, O_RDWR);  
  update_info();
  
  preferenciesWindow->hide();
}

static void cb_Cancel(Fl_Button*, void*) {
  preferenciesWindow->hide();
}

void PreferencesDialog(Fl_Widget *, void *) {
  Fl_Window* w;
   {Fl_Window* o = preferenciesWindow = new Fl_Window(265, 289, _("Preferencies"));
    w = o;
     {Fl_Tabs* o = new Fl_Tabs(5, 5, 255, 245);
       {Fl_Group* o = new Fl_Group(0, 23, 255, 221, _("Sound device"));
        o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
         {Fl_Input_Browser* o = deviceNameInput = new Fl_Input_Browser(90, 27, 155, 23, _("Device name:")); o->begin();
          o->align(132);
           
          o->value(device);
          choice_items("/dev");
          choice_items("/dev/sound");
          o->end();
        }
        o->end();
      }
      o->end();
    }
     {Fl_Button* o = new Fl_Button(95, 260, 80, 25, _("&OK"));
      o->callback((Fl_Callback*)cb_OK);
       
    }
     {Fl_Button* o = new Fl_Button(180, 260, 80, 25, _("&Cancel"));
      o->callback((Fl_Callback*)cb_Cancel);
    }
    o->end();
  }
  preferenciesWindow->end();
  preferenciesWindow->show();
}
