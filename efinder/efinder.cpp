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

#include <efltk/Fl_Image.h>

#include "efinder.h"
#include "eglob.h"
#include "../common/aboutdialog.h"
#include "VerticalTabs.h"

#include "icons/play.xpm"
#include "icons/pause.xpm"
#include "icons/stop.xpm"

Fl_Window* propertiesWindow;
Fl_Value_Input* fileLimitValue;
Fl_Check_Button* stayOnSingleCheck;
Fl_Check_Button* outputCountCheck;

static void cb_Close2(Fl_Button*, void*) {
  propertiesWindow->hide();
}

Fl_Window* createPropertiesWindow() {
  Fl_Window* w;
   {Fl_Window* o = propertiesWindow = new Fl_Window(290, 199, _("Preferences"));
    w = o;
    o->shortcut(0xff1b);
     {Fl_Tabs* o = new Fl_Tabs(0, 0, 379, 269);
      o->color((Fl_Color)0xfffffffe);
       {Fl_Group* o = new Fl_Group(1, 23, 377, 245, _("Preferences"));
o->layout_align(FL_ALIGN_CLIENT);
         {Fl_Value_Input* o = fileLimitValue = new Fl_Value_Input(159, 17, 45, 20, _("Limit results to first n files:"));
          o->step(1);
          o->value(100);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_CLIP|FL_ALIGN_WRAP);
        }
         {Fl_Check_Button* o = stayOnSingleCheck = new Fl_Check_Button(9, 42, 230, 25, _("Stay on single filesystem"));
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_CLIP|FL_ALIGN_WRAP);
        }
         //TODO: this feature needs to be implemented
         /*{Fl_Check_Button* o = outputCountCheck = new Fl_Check_Button(9, 74, 270, 23, _("Output count of matching lines"));
          o->value(1);*/
        }
         {Fl_Button* o = new Fl_Button(204, 142, 75, 25, _("&Close"));
          o->callback((Fl_Callback*)cb_Close2);
        }
        o->end();
      }
      o->end();
    }
    o->end();
  }
  propertiesWindow->end();
  propertiesWindow->set_modal();
  propertiesWindow->hide();
//  propertiesWindow->clear_visible();
  return  w;
}


Fl_Window* permWindow;

static Fl_Image play_pix((const char **)play_xpm);
static Fl_Image pause_pix((const char **)pause_xpm);
static Fl_Image stop_pix((const char **)stop_xpm);

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
  permWindow->set_modal();
  permWindow->show();
}
Fl_Window* findWindow;
Fl_Input* filterInput;
Fl_Input* pathInput;
Fl_Check_Button* recursiveCheck;
Fl_Check_Button* findHidden;

static void cb_Browse(Fl_Button*, void*) {
  const char *f = fl_select_dir(0,  _("Select directory"));
      if (f) {
  	pathInput->value(f);
      }
;}
Fl_Check_Button* caseSensitiveCheck;
Fl_Input* containsInput;
Fl_Check_Button* useRegexpCheck;
Fl_Check_Button* useLocate;
Fl_Check_Button* wholeWordsOnly;
Fl_Input_Browser* fileTypeBrowser;
Fl_Check_Button* doNotLookIntoBinaryCheck;
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
Fl_ListView* searchList;
Fl_Box* statusLine;
Fl_Button* searchButton;
Fl_Button* pauseButton;

static void cb_searchButton(Fl_Button*, void*) {
  findCB();
}


Fl_Button* stopButton;

static void cb_stopButton(Fl_Button*, void*) {
  stopSearch();
}


static void cb_pauseButton(Fl_Button*, void*) {
  pauseSearch();
}
  

static void cb_Quit(Fl_Item*, void*) {
  exit(0);
}

static void cb_Preferences(Fl_Item*, void*) {
  propertiesWindow->show();
  propertiesWindow->set_modal();
}

static void cb_About(Fl_Item*, void*) {
  AboutDialog("Find Files","2.0","");
}


void cb_searchResult(Fl_ListView *list, void*)
{
	if (Fl::event_clicks() && (list->selected() == 1)) {
		Fl_ListView_Item *item = list->get_selection()[0];
		Fl_String filename ("elauncher \"file:" + Fl_String (item->label(0)) + "\"");
		fl_start_child_process((char *)filename, false);
	}
}


void createFindWindow() {
  createPropertiesWindow();
  Fl_Window* w;
   {Fl_Window* o = findWindow = new Fl_Window(640, 490, _("Find"));
    w = o;
    o->shortcut(0xff1b);
     {VerticalTabs* o = new VerticalTabs(3, 26, 200, 420);
//      o->color((Fl_Color)0xfffffffe);
//      o->color(fl_darker(FL_GRAY));

       {Fl_Group* o = new Fl_Group(1, 45, 198, 418, _("Filenames"));
        o->hide();
         {Fl_Box* o = new Fl_Box(11, 40, 170, 35, _("Look for files whose name contains:"));
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
        }
         {Fl_Input* o = filterInput = new Fl_Input(11, 80, 175, 25);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
          //o->value("*");
          //o->parent()->resizable(o);
        }
         {Fl_Box* o = new Fl_Box(11, 135, 175, 20, _("Start in directory:"));
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
        }
         {Fl_Input* o = pathInput = new Fl_Input(11, 157, 175, 25);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
          o->parent()->resizable(o);
          char *tmp = fl_get_homedir();
          o->value(tmp);
          delete []tmp;
        }
         {Fl_Button* o = new Fl_Button(101, 192, 80, 25, _("&Browse..."));
          o->callback((Fl_Callback*)cb_Browse);
          o->tooltip(_("Find path."));
        }
         {Fl_Check_Button* o = recursiveCheck = new Fl_Check_Button(21, 225, 155, 25, _("Search subdirectories"));
          o->value(1);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_CLIP|FL_ALIGN_WRAP);
        }
         {Fl_Check_Button* o = findHidden = new Fl_Check_Button(21, 255, 140, 25, _("Show hidden files"));
          o->align(FL_ALIGN_LEFT|FL_ALIGN_TOP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
        }
        // TODO: This should be in preferences:
        /* {Fl_Check_Button* o = useLocate = new Fl_Check_Button(21, 285, 140, 25, _("Use \"locate\""));
          o->align(FL_ALIGN_LEFT|FL_ALIGN_TOP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
          o->deactivate();
        }*/
         {Fl_Box* o = new Fl_Box(11, 305, 175, 20);
          o->parent()->resizable(o);
        }
        o->parent()->resizable(o);
        o->end();
      }
       {Fl_Group* o = new Fl_Group(1, 45, 198, 418, _("Content"));
        o->hide();
         {Fl_Box* o = new Fl_Box(11, 20, 170, 40, _("Look for files that contain the following text:"));
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
        }
         {Fl_Input* o = containsInput = new Fl_Input(11, 65, 175, 25);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
          o->parent()->resizable(o);
        }
         {Fl_Check_Button* o = useRegexpCheck = new Fl_Check_Button(11, 95, 160, 25, _("Perl compatible regexp"));
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
        }
         {Fl_Check_Button* o = caseSensitiveCheck = new Fl_Check_Button(11, 135, 140, 25, _("Case sensitive"));
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
        }
         {Fl_Check_Button* o = wholeWordsOnly = new Fl_Check_Button(11, 165, 140, 25, _("Whole words only"));
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
         {Fl_Box* o = new Fl_Box(11, 285, 175, 20);
          o->parent()->resizable(o);
        }
        }
        o->end();
      }
       {Fl_Group* o = new Fl_Group(1, 45, 198, 418, _("File types"));
         {Fl_Box* o = new Fl_Box(11, 30, 180, 20, _("Look for files of type:"));
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
        }
         {Fl_Box* o = new Fl_Box(11, 65, 115, 20, _("Known types:"));
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
        }
         {Fl_Input_Browser* o = fileTypeBrowser = new Fl_Input_Browser(11, 87, 145, 25); o->begin();
          o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
          for (int i=0; i<FILE_TYPES_NR-1; i++) 
              new Fl_Item(ftypes[i]);
          o->parent()->resizable(o);
          
          o->value("Any kind");
          o->end();
          o->type(Fl_Input_Browser::NONEDITABLE);
        }
         {Fl_Check_Button* o = doNotLookIntoBinaryCheck = new Fl_Check_Button(11, 142, 180, 25, _("Do not look into binary files"));
          o->value(1);
        }
         {Fl_Box* o = new Fl_Box(11, 285, 175, 20);
          o->parent()->resizable(o);
        }
        o->end();
      }
       {Fl_Group* o = new Fl_Group(1, 45, 198, 418, _("File properties"));
        o->hide();
         {Fl_Box* o = new Fl_Box(6, 15, 190, 30, _("Find only files with the following:"));
          o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
        }
         {Fl_Check_Button* o = considerPermValue = new Fl_Check_Button(6, 50, 115, 25, _("Permissions"));
          o->callback((Fl_Callback*)cb_considerPermValue);
        }
         {Fl_Check_Button* o = considerUserValue = new Fl_Check_Button(6, 75, 115, 25, _("File owner:"));
          o->callback((Fl_Callback*)cb_considerUserValue);
        }
         {Fl_Group* o = userGroup = new Fl_Group(6, 105, 180, 60);
          o->deactivate();
           {Fl_Input_Browser* o = userIdChoice = new Fl_Input_Browser(70, 0, 72, 20, _("User id:")); o->begin();
            o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
            o->value("Anyone");
            jam("/etc/passwd", userIdChoice);
            o->end();
            o->type(Fl_Input_Browser::NONEDITABLE);
          }
           {Fl_Input_Browser* o = groupIdChoice = new Fl_Input_Browser(70, 25, 72, 20, _("Group id:")); o->begin();
            o->align(FL_ALIGN_LEFT|FL_ALIGN_WRAP);
            o->value("Anyone");
            jam("/etc/group", groupIdChoice);
            o->end();
            o->type(Fl_Input_Browser::NONEDITABLE);
          }
          o->end();
        }
         {Fl_Check_Button* o = considerSizeValue = new Fl_Check_Button(6, 167, 80, 25, _("Size:"));
          o->callback((Fl_Callback*)cb_considerSizeValue);
          o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
        }
         {Fl_Group* o = sizeGroup = new Fl_Group(6, 190, 190, 50);
          o->box(FL_FLAT_BOX);
          o->deactivate();
           {Fl_Value_Input* o = sizeGValue = new Fl_Value_Input(110, 2, 70, 20, _("greater than:"));
            o->step(1);
          }
           {Fl_Value_Input* o = sizeMValue = new Fl_Value_Input(110, 27, 70, 20, _("and less than (KB):"));
            o->step(1);
          }
          o->end();
        }
         {Fl_Check_Button* o = considerTimeValue = new Fl_Check_Button(6, 240, 75, 25, _("Time:"));
          o->callback((Fl_Callback*)cb_considerTimeValue);
        }
         {Fl_Group* o = timeGroup1 = new Fl_Group(86, 250, 110, 60);
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
         {Fl_Group* o = new Fl_Group(6, 310, 110, 60);
           {Fl_Value_Input* o = timeValue = new Fl_Value_Input(5, 18, 65, 20, _("in last:"));
            o->maximum(100);
            o->step(1);
            o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_CLIP|FL_ALIGN_WRAP);
            o->deactivate();
          }
          o->end();
        }
         {Fl_Group* o = timeGroup2 = new Fl_Group(86, 320, 105, 60);
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
             {Fl_Round_Button* o = mounthsRadio = new Fl_Round_Button(0, 45, 105, 15, _("Months"));
              o->type(Fl_Round_Button::RADIO);
            }
            o->end();
          }
          o->end();
        }
        o->end();
      }
      o->end();
    }
     {Fl_ListView* o = searchList = new Fl_ListView(205, 26, 430, 407); o->begin();
      o->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
      searchList->type(searchList->type() | Fl_ListView::MULTI_SELECTION);
      searchList->add_column(_("Filename"), 100);
      searchList->add_column(_("Location"), 100);
      searchList->add_column(_("Size"), 79);
      searchList->add_column(_("Last modified"), 140);
      searchList->add_column(_("Rights"), 65);
      searchList->column_flags(0, FL_ALIGN_LEFT);
      searchList->column_flags(1, FL_ALIGN_LEFT);
      searchList->column_flags(2, FL_ALIGN_LEFT);
      searchList->column_flags(3, FL_ALIGN_LEFT);
      searchList->column_flags(4, FL_ALIGN_RIGHT);
      searchList->callback((Fl_Callback*)cb_searchResult);
      o->end();
      Fl_Group::current()->resizable(o);
    }
     {Fl_Box* o = statusLine = new Fl_Box(205, 435, 430, 20, _("Click on Start to begin search"));
// TODO: make this box clipping (can't remember how)
      o->box(FL_THIN_DOWN_BOX);
      o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
      o->when(FL_WHEN_CHANGED);
    }
     {Fl_Group* o = new Fl_Group(205, 455, 453, 40);
         {Fl_Box* o = new Fl_Box(140, 5, 10, 25);
          o->parent()->resizable(o);
        }
       {Fl_Button* o = searchButton = new Fl_Button(150, 5, 90, 25, _("&Start"));
        o->image(play_pix);
        o->shortcut(0xff0d);
        o->callback((Fl_Callback*)cb_searchButton);
        o->tooltip(_("Start searching."));
      }
       {Fl_Button* o = pauseButton = new Fl_Button(245, 5, 88, 25, _("&Pause"));
        o->image(pause_pix);
        o->tooltip(_("Pause current search. Click on Start to continue."));
        o->callback((Fl_Callback*)cb_pauseButton);
        o->deactivate();
      }
       {Fl_Button* o = stopButton = new Fl_Button(340, 5, 90, 25, _("S&top"));
        o->image(stop_pix);
        o->callback((Fl_Callback*)cb_stopButton);
        o->tooltip(_("Stop searching."));
        o->deactivate();
      }
      o->end();
    }
     {Fl_Menu_Bar* mainmenu = new Fl_Menu_Bar(5, 0, 630, 23); mainmenu->begin();
      mainmenu->shortcut(0xffe9);
       {Fl_Item_Group* o = new Fl_Item_Group(_("&File"));
         {Fl_Item* o = new Fl_Item(_("&New window"));
          o->shortcut(0x4006e);
        }
        new Fl_Divider();
         {Fl_Item* o = new Fl_Item(_("&Quit"));
          o->shortcut(0x40071);
          o->callback((Fl_Callback*)cb_Quit);
        }
        o->end();
      }
       {Fl_Item_Group* o = new Fl_Item_Group(_("&Options"));
         {Fl_Item* o = new Fl_Item(_("&Preferences"));
          o->shortcut(0x40070);
          o->callback((Fl_Callback*)cb_Preferences);
        }
        o->end();
      }
       {Fl_Item_Group* o = new Fl_Item_Group(_("&Help"));
         {Fl_Item* o = new Fl_Item(_("&About"));
          o->callback((Fl_Callback*)cb_About);
        }
        mainmenu->right_layout(o);
        o->end();
      }
      mainmenu->end();
    }
    o->size_range(o->w(), o->h());
    o->end();
  }
  findWindow->end();
  findWindow->show();
}


