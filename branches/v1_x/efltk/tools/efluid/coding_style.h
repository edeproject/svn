// generated by Extended Fast Light User Interface Designer (eFluid) version 2.0003

#ifndef _CODING_STYLE_H_
#define _CODING_STYLE_H_

#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Round_Button.h>
#include <efltk/Fl_Check_Button.h>
#include <efltk/Fl_Value_Input.h>
#include <efltk/Fl_Button.h>

extern int gbrace_style;
extern int gno_space_parens;
extern int gbrace_functions;
extern int gindent_tabs;
extern int gtab_size;
extern int galways_return_parens;
extern int gindent_code;
extern void save_coding_style();
extern void write_code_block(char *pBlock);
extern void load_coding_style();
extern const char *get_opening_brace(int is_function);
extern const char *get_indent_string(int level);
extern void show_coding_style_cb(Fl_Widget *, void *);
extern void write_includes_from_code(char*);
Fl_Window* make_codingstyle_window();
void load_codingstyle_window();

#endif
