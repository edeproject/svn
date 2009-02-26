/*
 * $Id$
 *
 * Efinder, file search tool
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */


#include "efinder.h"
#include "eglob.h"
Fl_Window* permWindow;

static void cb_Read(Fl_Check_Button*, void*) {
  toggle_permission(S_IRUSR);
}

static void cb_Write(Fl_Check_Button*, void*) {
  toggle_permission(S_IWUSR);
}

static void cb_Execute(Fl_Check_Button*, void*) {
  toggle_permission(S_IXUSR);
}

static void cb_Set(Fl_Check_Button*, void*) {
  toggle_permission(S_ISUID);
}

static void cb_Read1(Fl_Check_Button*, void*) {
  toggle_permission(S_IRGRP);
}

static void cb_Write1(Fl_Check_Button*, void*) {
  toggle_permission(S_IWGRP);
}

static void cb_Execute1(Fl_Check_Button*, void*) {
  toggle_permission(S_IXGRP);
}

static void cb_Set1(Fl_Check_Button*, void*) {
  toggle_permission(S_ISGID);
}

static void cb_Read2(Fl_Check_Button*, void*) {
  toggle_permission(S_IROTH);
}

static void cb_Write2(Fl_Check_Button*, void*) {
  toggle_permission(S_IWOTH);
}

static void cb_Execute2(Fl_Check_Button*, void*) {
  toggle_permission(S_IXOTH);
}

static void cb_Sticky(Fl_Check_Button*, void*) {
  toggle_permission(S_ISVTX);
}

static void cb_Close(Fl_Button*, void*) {
  permWindow->hide();
}

void createPermWindow() {
  Fl_Window* w;
   {Fl_Window* o = permWindow = new Fl_Window(355, 175, _("Permissions"));
    w = o;
    o->shortcut(0xff1b);
     {Fl_Group* o = new Fl_Group(10, 30, 110, 90, _("Owner"));
      o->box(FL_ENGRAVED_BOX);
      o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
       {Fl_Check_Button* o = new Fl_Check_Button(5, 5, 85, 25, _("Read"));
        o->callback((Fl_Callback*)cb_Read);
      }
       {Fl_Check_Button* o = new Fl_Check_Button(5, 25, 85, 25, _("Write"));
        o->callback((Fl_Callback*)cb_Write);
      }
       {Fl_Check_Button* o = new Fl_Check_Button(5, 45, 85, 25, _("Execute"));
        o->callback((Fl_Callback*)cb_Execute);
      }
       {Fl_Check_Button* o = new Fl_Check_Button(5, 65, 85, 25, _("Set UID"));
        o->callback((Fl_Callback*)cb_Set);
      }
      o->end();
    }
     {Fl_Group* o = new Fl_Group(125, 30, 105, 90, _("Group"));
      o->box(FL_ENGRAVED_BOX);
      o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
       {Fl_Check_Button* o = new Fl_Check_Button(5, 5, 85, 25, _("Read"));
        o->callback((Fl_Callback*)cb_Read1);
      }
       {Fl_Check_Button* o = new Fl_Check_Button(5, 25, 85, 25, _("Write"));
        o->callback((Fl_Callback*)cb_Write1);
      }
       {Fl_Check_Button* o = new Fl_Check_Button(5, 45, 85, 25, _("Execute"));
        o->callback((Fl_Callback*)cb_Execute1);
      }
       {Fl_Check_Button* o = new Fl_Check_Button(5, 65, 85, 25, _("Set GID"));
        o->callback((Fl_Callback*)cb_Set1);
      }
      o->end();
    }
     {Fl_Group* o = new Fl_Group(235, 30, 110, 90, _("Other"));
      o->box(FL_ENGRAVED_BOX);
      o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
       {Fl_Check_Button* o = new Fl_Check_Button(5, 5, 85, 25, _("Read"));
        o->callback((Fl_Callback*)cb_Read2);
      }
       {Fl_Check_Button* o = new Fl_Check_Button(5, 25, 85, 25, _("Write"));
        o->callback((Fl_Callback*)cb_Write2);
      }
       {Fl_Check_Button* o = new Fl_Check_Button(5, 45, 85, 25, _("Execute"));
        o->callback((Fl_Callback*)cb_Execute2);
      }
       {Fl_Check_Button* o = new Fl_Check_Button(5, 65, 95, 25, _("Sticky"));
        o->callback((Fl_Callback*)cb_Sticky);
      }
      o->end();
    }
     {Fl_Button* o = new Fl_Button(140, 140, 85, 25, _("&Close"));
      o->callback((Fl_Callback*)cb_Close);
    }
    o->end();
    o->resizable(o);
  }
  permWindow->end();
  permWindow->show();
}
Fl_Window* findWindow;
Fl_Input* pathInput;

static void cb_Browse(Fl_Button*, void*) {
  const char *f = fl_select_dir(0,  _("Select directory"));
      if (f) {
  	pathInput->value(f);
      }
;}
Fl_Input* filterInput;
Fl_Check_Button* recursiveCheck;
Fl_Value_Input* fileLimitValue;
Fl_Input* containsInput;
Fl_Check_Button* caseSensitiveCheck;
Fl_Check_Button* useRegexpCheck;
Fl_Input_Browser* fileTypeBrowser;
Fl_Check_Button* stayOnSingleCheck;
Fl_Check_Button* considerPermValue;

static void cb_considerPermValue(Fl_Check_Button*, void*) {
  if (considerPermValue->value())
  	createPermWindow();
}
Fl_Check_Button* considerUserValue;

static void cb_considerUserValue(Fl_Check_Button*, void*) {
  if (considerUserValue->value()) {
  	userGroup->activate();
  }
  else {
  	userGroup->deactivate();
  }
;}
Fl_Group* userGroup;
Fl_Input_Browser* userIdChoice;
Fl_Input_Browser* groupIdChoice;
Fl_Check_Button* considerSizeValue;

static void cb_considerSizeValue(Fl_Check_Button*, void*) {
  if (considerSizeValue->value()) {
  	sizeGroup->activate();
  }
  else {
  	sizeGroup->deactivate();
  }
;}
Fl_Group* sizeGroup;
Fl_Value_Input* sizeGValue;
Fl_Value_Input* sizeMValue;
Fl_Check_Button* considerTimeValue;

static void cb_considerTimeValue(Fl_Check_Button*, void*) {
  if (considerTimeValue->value()) {
  	timeGroup1->activate();
  	timeValue->activate();
  	timeGroup2->activate();
  }
  else {
  	timeGroup1->deactivate();
  	timeValue->deactivate();
  	timeGroup2->deactivate();
  }
;}
Fl_Group* timeGroup1;
Fl_Round_Button* modifiedRadio;
Fl_Round_Button* changedRadio;
Fl_Round_Button* accessedRadio;
Fl_Value_Input* timeValue;
Fl_Group* timeGroup2;
Fl_Round_Button* minutesRadio;
Fl_Round_Button* hoursRadio;
Fl_Round_Button* daysRadio;
Fl_Round_Button* mounthsRadio;
Fl_Check_Button* doNotLookIntoBinaryCheck;
Fl_Check_Button* outputCountCheck;
Fl_Round_Button* matchAnywhereRadio;
Fl_Round_Button* matchWordsRadio;
Fl_Round_Button* matchLinesRadio;
Fl_Round_Button* invertMatchRadio;
Fl_ListView* searchList;
Fl_Box* statusLine;
Fl_Button* searchButton;

static void cb_searchButton(Fl_Button*, void*) {
  findCB();
}

static void cb_Cancel(Fl_Button*, void*) {
  exit(0);
}
Fl_Button* stopButton;

static void cb_stopButton(Fl_Button*, void*) {
  stopSearch();
}

void createFindWindow() {
  Fl_Window* w;
   {Fl_Window* o = findWindow = new Fl_Window(480, 385, _("Find"));
    w = o;
    o->shortcut(0xff1b);
     {Fl_Tabs* o = new Fl_Tabs(3, 5, 473, 235);
      o->color((Fl_Color)0xfffffffe);
       {Fl_Group* o = new Fl_Group(1, 23, 471, 211, _("Find"));
         {Fl_Group* o = new Fl_Group(7, 20, 460, 100, _("Find"));
          o->box(FL_ENGRAVED_BOX);
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
           {Fl_Group* o = new Fl_Group(5, 5, 450, 30);
             {Fl_Input* o = pathInput = new Fl_Input(70, 5, 285, 23, _("Path:"));
              o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
              o->parent()->resizable(o);
              char *tmp = fl_get_homedir();
              o->value(tmp);
              delete []tmp;
            }
             {Fl_Button* o = new Fl_Button(360, 5, 80, 23, _("&Browse..."));
              o->callback((Fl_Callback*)cb_Browse);
              o->tooltip(_("Find path."));
            }
            o->end();
          }
           {Fl_Input* o = filterInput = new Fl_Input(75, 40, 370, 23, _("File filter:"));
            o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
            o->value("*");
            o->parent()->resizable(o);
          }
           {Fl_Group* o = new Fl_Group(25, 65, 430, 32);
             {Fl_Check_Button* o = recursiveCheck = new Fl_Check_Button(50, 0, 155, 30, _("Search subdirectories"));
              o->value(1);
              o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_CLIP|FL_ALIGN_WRAP);
            }
             {Fl_Group* o = new Fl_Group(200, 0, 220, 32);
               {Fl_Value_Input* o = fileLimitValue = new Fl_Value_Input(175, 5, 45, 20, _("Limit results to first n files:"));
                o->step(1);
                o->value(64);
                o->align(FL_ALIGN_LEFT|FL_ALIGN_CLIP|FL_ALIGN_WRAP);
              }
              o->end();
            }
            o->end();
          }
          o->end();
        }
         {Fl_Group* o = new Fl_Group(7, 140, 459, 65, _("Content"));
          o->box(FL_ENGRAVED_BOX);
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
           {Fl_Input* o = containsInput = new Fl_Input(75, 10, 370, 23, _("Containing:"));
            o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
            o->parent()->resizable(o);
          }
           {Fl_Group* o = new Fl_Group(75, 35, 370, 25);
             {Fl_Check_Button* o = caseSensitiveCheck = new Fl_Check_Button(0, 0, 140, 25, _("Case sensitive"));
              o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
            }
             {Fl_Check_Button* o = useRegexpCheck = new Fl_Check_Button(140, 0, 230, 25, _("Extended regexp"));
              o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
            }
            o->end();
          }
          o->end();
        }
         {Fl_Box* o = new Fl_Box(7, 206, 455, 2);
          o->parent()->resizable(o);
        }
        o->parent()->resizable(o);
        o->end();
      }
       {Fl_Group* o = new Fl_Group(1, 23, 471, 211, _("Filter options"));
        o->hide();
         {Fl_Group* o = new Fl_Group(7, 20, 459, 32, _("Considerations"));
          o->box(FL_ENGRAVED_BOX);
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
           {Fl_Input_Browser* o = fileTypeBrowser = new Fl_Input_Browser(70, 5, 145, 23, _("File type:")); o->begin();
            o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
            new Fl_Item(_("Any kind"));
            new Fl_Item(_("Regular"));
            new Fl_Item(_("Directory"));
            new Fl_Item(_("Symlink"));
            new Fl_Item(_("Socket"));
            new Fl_Item(_("Block device"));
            new Fl_Item(_("Character device"));
            new Fl_Item(_("FIFO"));
            o->parent()->resizable(o);
            
            o->value("Any kind");
            o->end();
          }
           {Fl_Check_Button* o = stayOnSingleCheck = new Fl_Check_Button(225, 4, 230, 23, _("Stay on single filesystem"));
            o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_CLIP|FL_ALIGN_WRAP);
          }
          o->end();
        }
         {Fl_Group* o = new Fl_Group(7, 50, 459, 155);
          o->box(FL_ENGRAVED_BOX);
           {Fl_Check_Button* o = considerPermValue = new Fl_Check_Button(5, 5, 115, 25, _("Permissions"));
            o->callback((Fl_Callback*)cb_considerPermValue);
          }
           {Fl_Check_Button* o = considerUserValue = new Fl_Check_Button(5, 30, 115, 25, _("User/group:"));
            o->callback((Fl_Callback*)cb_considerUserValue);
          }
           {Fl_Group* o = userGroup = new Fl_Group(120, 30, 325, 25);
            o->deactivate();
             {Fl_Input_Browser* o = userIdChoice = new Fl_Input_Browser(70, 2, 72, 20, _("User id:")); o->begin();
              o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
              o->value("Anyone");
              jam("/etc/passwd", userIdChoice);
              o->end();
            }
             {Fl_Input_Browser* o = groupIdChoice = new Fl_Input_Browser(240, 2, 72, 20, _("Group id:")); o->begin();
              o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
              o->value("Anyone");
              jam("/etc/group", groupIdChoice);
              o->end();
            }
            o->end();
          }
           {Fl_Check_Button* o = considerSizeValue = new Fl_Check_Button(5, 55, 80, 23, _("Size:"));
            o->callback((Fl_Callback*)cb_considerSizeValue);
            o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
          }
           {Fl_Group* o = sizeGroup = new Fl_Group(85, 55, 360, 25);
            o->box(FL_FLAT_BOX);
            o->deactivate();
             {Fl_Value_Input* o = sizeGValue = new Fl_Value_Input(90, 2, 70, 20, _("greater than:"));
              o->step(1);
            }
             {Fl_Value_Input* o = sizeMValue = new Fl_Value_Input(275, 2, 70, 20, _("and less than(KB):"));
              o->step(1);
            }
            o->end();
          }
           {Fl_Check_Button* o = considerTimeValue = new Fl_Check_Button(5, 80, 135, 25, _("Time:"));
            o->callback((Fl_Callback*)cb_considerTimeValue);
          }
           {Fl_Group* o = timeGroup1 = new Fl_Group(140, 85, 110, 60);
            o->box(FL_FLAT_BOX);
            o->deactivate();
             {Fl_Round_Button* o = modifiedRadio = new Fl_Round_Button(0, 0, 105, 20, _("Modified"));
              o->type(Fl_Round_Button::RADIO);
            }
             {Fl_Round_Button* o = changedRadio = new Fl_Round_Button(0, 20, 105, 20, _("Changed"));
              o->type(Fl_Round_Button::RADIO);
            }
             {Fl_Round_Button* o = accessedRadio = new Fl_Round_Button(0, 40, 105, 20, _("Accessed"));
              o->type(Fl_Round_Button::RADIO);
            }
            o->end();
          }
           {Fl_Group* o = new Fl_Group(255, 90, 80, 60);
             {Fl_Value_Input* o = timeValue = new Fl_Value_Input(5, 33, 65, 20, _("in previous:"));
              o->maximum(100);
              o->step(1);
              o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_CLIP|FL_ALIGN_WRAP);
              o->deactivate();
            }
            o->end();
          }
           {Fl_Group* o = timeGroup2 = new Fl_Group(335, 85, 110, 60);
            o->box(FL_FLAT_BOX);
            o->deactivate();
             {Fl_Group* o = new Fl_Group(0, 0, 105, 60);
               {Fl_Round_Button* o = minutesRadio = new Fl_Round_Button(0, 0, 105, 15, _("Minutes"));
                o->type(Fl_Round_Button::RADIO);
              }
               {Fl_Round_Button* o = hoursRadio = new Fl_Round_Button(0, 15, 105, 15, _("Hours"));
                o->type(Fl_Round_Button::RADIO);
              }
               {Fl_Round_Button* o = daysRadio = new Fl_Round_Button(0, 30, 105, 15, _("Days"));
                o->type(Fl_Round_Button::RADIO);
              }
               {Fl_Round_Button* o = mounthsRadio = new Fl_Round_Button(0, 45, 105, 15, _("Mounths"));
                o->type(Fl_Round_Button::RADIO);
              }
              o->end();
            }
            o->end();
          }
           {Fl_Box* o = new Fl_Box(440, 5, 10, 145);
            o->parent()->resizable(o);
          }
          o->parent()->resizable(o);
          o->end();
        }
        o->end();
      }
       {Fl_Group* o = new Fl_Group(1, 23, 471, 211, _("Content options"));
        o->hide();
         {Fl_Group* o = new Fl_Group(7, 20, 459, 185, _("General considerations"));
          o->box(FL_ENGRAVED_BOX);
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
           {Fl_Check_Button* o = doNotLookIntoBinaryCheck = new Fl_Check_Button(15, 10, 270, 23, _("Do not look into binary files"));
            o->value(1);
          }
           {Fl_Check_Button* o = outputCountCheck = new Fl_Check_Button(15, 37, 270, 23, _("Output count of matching lines"));
            o->value(1);
          }
           {Fl_Round_Button* o = matchAnywhereRadio = new Fl_Round_Button(15, 72, 255, 20, _("Match anywhere"));
            o->type(Fl_Round_Button::RADIO);
            o->value(1);
          }
           {Fl_Round_Button* o = matchWordsRadio = new Fl_Round_Button(15, 95, 255, 20, _("Match whole words only"));
            o->type(Fl_Round_Button::RADIO);
          }
           {Fl_Round_Button* o = matchLinesRadio = new Fl_Round_Button(15, 118, 255, 20, _("Match whole lines only"));
            o->type(Fl_Round_Button::RADIO);
          }
           {Fl_Round_Button* o = invertMatchRadio = new Fl_Round_Button(15, 141, 255, 20, _("Output files where no match is found"));
            o->type(Fl_Round_Button::RADIO);
          }
           {Fl_Box* o = new Fl_Box(285, 10, 160, 165);
            o->parent()->resizable(o);
          }
          o->parent()->resizable(o);
          o->end();
        }
        o->end();
      }
       {Fl_Group* o = new Fl_Group(1, 23, 471, 211, _("About"));
        o->hide();
         {Fl_Box* o = new Fl_Box(21, 32, 424, 135, _("(C) Copyright 2000-2005. EDE Authors.\n\n Efinder is using the code from xfglob whic\
h is\n (C) Copyright 2000-2001 by Edscott Wilson Garcia."));
          o->align(FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
        }
        o->end();
      }
      o->end();
    }
     {Fl_ListView* o = searchList = new Fl_ListView(3, 245, 473, 85); o->begin();
      o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
      searchList->type(searchList->type() | Fl_ListView::MULTI_SELECTION);
      searchList->add_column(_("Filename"), 200);
      searchList->add_column(_("Size"), 79);
      searchList->add_column(_("Last modified"), 125);
      searchList->add_column(_("Rights"), 65);
      searchList->column_flags(0, FL_ALIGN_LEFT);
      searchList->column_flags(1, FL_ALIGN_CENTER);
      searchList->column_flags(2, FL_ALIGN_CENTER);
      searchList->column_flags(3, FL_ALIGN_RIGHT);
      o->end();
      Fl_Group::current()->resizable(o);
    }
     {Fl_Box* o = statusLine = new Fl_Box(3, 362, 473, 20);
      o->box(FL_THIN_DOWN_BOX);
      o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
      o->when(FL_WHEN_CHANGED);
    }
     {Fl_Group* o = new Fl_Group(3, 330, 473, 30);
       {Fl_Button* o = searchButton = new Fl_Button(222, 3, 80, 25, _("&Find"));
        o->shortcut(0xff0d);
        o->callback((Fl_Callback*)cb_searchButton);
        o->tooltip(_("Start the searching."));
      }
       {Fl_Button* o = new Fl_Button(392, 3, 80, 25, _("&Cancel"));
        o->callback((Fl_Callback*)cb_Cancel);
      }
       {Fl_Button* o = stopButton = new Fl_Button(307, 3, 80, 25, _("&Stop"));
        o->callback((Fl_Callback*)cb_stopButton);
        o->tooltip(_("Stop the searching."));
      }
       {Fl_Box* o = new Fl_Box(7, 5, 210, 20);
        o->parent()->resizable(o);
      }
      o->end();
    }
    o->size_range(o->w(), o->h());
    o->end();
  }
  findWindow->end();
  findWindow->show();
}