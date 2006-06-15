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

#ifndef _EFINDER_H_
#define _EFINDER_H_

#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Check_Button.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Input.h>
#include <efltk/Fl_File_Dialog.h>
#include <efltk/Fl_Value_Input.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Input_Browser.h>
#include <efltk/Fl_Item.h>
#include <efltk/Fl_Round_Button.h>
#include <efltk/Fl_ListView.h>
#include <efltk/Fl_ListView_Item.h>
#include <efltk/Fl_Util.h>

extern Fl_Window* permWindow;
extern Fl_Window* findWindow;
extern Fl_Input* pathInput;
extern Fl_Input* filterInput;
extern Fl_Check_Button* recursiveCheck;
extern Fl_Value_Input* fileLimitValue;
extern Fl_Input* containsInput;
extern Fl_Check_Button* caseSensitiveCheck;
extern Fl_Check_Button* useRegexpCheck;
extern Fl_Input_Browser* fileTypeBrowser;
extern Fl_Check_Button* stayOnSingleCheck;
extern Fl_Check_Button* considerPermValue;
extern Fl_Check_Button* considerUserValue;
extern Fl_Group* userGroup;
extern Fl_Input_Browser* userIdChoice;
extern Fl_Input_Browser* groupIdChoice;
extern Fl_Check_Button* considerSizeValue;
extern Fl_Group* sizeGroup;
extern Fl_Value_Input* sizeGValue;
extern Fl_Value_Input* sizeMValue;
extern Fl_Check_Button* considerTimeValue;
extern Fl_Group* timeGroup1;
extern Fl_Round_Button* modifiedRadio;
extern Fl_Round_Button* changedRadio;
extern Fl_Round_Button* accessedRadio;
extern Fl_Value_Input* timeValue;
extern Fl_Group* timeGroup2;
extern Fl_Round_Button* minutesRadio;
extern Fl_Round_Button* hoursRadio;
extern Fl_Round_Button* daysRadio;
extern Fl_Round_Button* mounthsRadio;
extern Fl_Check_Button* doNotLookIntoBinaryCheck;
extern Fl_Check_Button* outputCountCheck;
extern Fl_Round_Button* matchAnywhereRadio;
extern Fl_Round_Button* matchWordsRadio;
extern Fl_Round_Button* matchLinesRadio;
extern Fl_Round_Button* invertMatchRadio;
extern Fl_ListView* searchList;
extern Fl_Box* statusLine;
extern Fl_Button* searchButton;
extern Fl_Button* stopButton;

void createPermWindow();
void createFindWindow();

#endif
