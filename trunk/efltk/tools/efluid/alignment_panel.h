// generated by Extended Fast Light User Interface Designer (eFluid) version 2.0003

#ifndef _ALIGNMENT_PANEL_H_
#define _ALIGNMENT_PANEL_H_

#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Value_Input.h>
#include <efltk/Fl_Input.h>
#include <efltk/Fl_Check_Button.h>
#include <efltk/Fl_Divider.h>

extern void alignment_cb(Fl_Value_Input*, long);
extern void header_input_cb(Fl_Input*, void*);
extern void code_input_cb(Fl_Input*, void*);
extern void include_H_from_C_button_cb(Fl_Check_Button*, void*);
extern bool i18n;

extern Fl_Window* alignment_window;
extern Fl_Value_Input* horizontal_input;
extern Fl_Value_Input* vertical_input;
extern Fl_Value_Input* snap_input;
extern Fl_Input* header_file_input;
extern Fl_Input* code_file_input;
extern Fl_Check_Button* include_H_from_C_button;
extern Fl_Check_Button* locale;

Fl_Window* make_alignment_window();

#endif
