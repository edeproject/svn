// generated by Extended Fast Light User Interface Designer (eFluid) version 2.0003

#ifndef _FUNCTION_PANEL_H_
#define _FUNCTION_PANEL_H_

#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Check_Button.h>
#include <efltk/Fl_Input.h>
#include <efltk/Fl_Return_Button.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Text_Editor.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Box.h>


extern Fl_Window* function_panel;
extern Fl_Check_Button* f_public_button;
extern Fl_Check_Button* f_c_button;
extern Fl_Input* f_name_input;
extern Fl_Input* f_attributes_input;
extern Fl_Input* f_return_type_input;
extern Fl_Return_Button* f_panel_ok;
extern Fl_Button* f_panel_cancel;
extern Fl_Window* code_panel;
extern Fl_Text_Editor* code_input;
extern Fl_Button* code_panel_cancel;
extern Fl_Button* code_panel_ok;
extern Fl_Window* codeblock_panel;
extern Fl_Input* code_before_input;
extern Fl_Input* code_after_input;
extern Fl_Return_Button* codeblock_panel_ok;
extern Fl_Button* codeblock_panel_cancel;
extern Fl_Window* declblock_panel;
extern Fl_Input* decl_before_input;
extern Fl_Input* decl_after_input;
extern Fl_Return_Button* declblock_panel_ok;
extern Fl_Button* declblock_panel_cancel;
extern Fl_Window* decl_panel;
extern Fl_Check_Button* decl_public_button;
extern Fl_Input* decl_input;
extern Fl_Return_Button* decl_panel_ok;
extern Fl_Button* decl_panel_cancel;
extern Fl_Window* class_panel;
extern Fl_Check_Button* c_public_button;
extern Fl_Input* c_name_input;
extern Fl_Input* c_subclass_input;
extern Fl_Button* c_panel_cancel;
extern Fl_Button* c_panel_ok;

Fl_Window* make_function_panel();
Fl_Window* make_code_panel();
Fl_Window* make_codeblock_panel();
Fl_Window* make_declblock_panel();
Fl_Window* make_decl_panel();
Fl_Window* make_class_panel();

#endif
