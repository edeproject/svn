// generated by Fast Light User Interface Designer (fluid) version 2,0003

#ifndef logoutdialog_h
#define logoutdialog_h


/*#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Round_Button.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Divider.h>*/

#include <fltk/Fl.h>
#include "NLS.h"
#include <fltk/Window.h>
#include <fltk/Group.h>
#include <fltk/Round_Button.h>
#include <fltk/Button.h>
#include <fltk/Box.h>
#include <fltk/Divider.h>

#include "icons/penguin.xpm"


// which of these can be safely removed? what should be extern and
// what shouldn't?

extern fltk::Window* windowLogoutDialog;
extern fltk::Round_Button* logoutRadioItemLogoutDialog;
extern fltk::Round_Button* restartRadioItemLogoutDialog;
extern fltk::Round_Button* shutdownRadioItemLogoutDialog;
void LogoutFunction(fltk::Widget *, void *);
extern void LogoutFunction(fltk::Button*, void*);
void LogoutDialog(fltk::Widget*, void *);


#endif
