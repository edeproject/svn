// generated by Fast Light User Interface Designer (fluid) version 2,0003

#ifndef widget_panel_h
#define widget_panel_h
#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_WM.h>
#include <efltk/Fl_Tabs.h>
extern void propagate_tabs(Fl_Tabs*, void*);
extern Fl_Tabs* panel_tabs;
#include <efltk/Fl_Group.h>
extern void propagate_group(Fl_Group*, void*);
#include <efltk/Fl_Input.h>
extern void user_class_cb(Fl_Input*, void*);
#include <efltk/Fl_Choice.h>
extern void subtype_cb(Fl_Choice*, void*);
extern void label_cb(Fl_Input*, void*);
#include <efltk/Fl_Box.h>
extern Fl_Box* image_label;
#include <efltk/Fl_Button.h>
extern void image_cb(Fl_Button*, void*);
#include <efltk/Fl_Check_Button.h>
extern void image_inlined_cb(Fl_Check_Button*, void*);
extern Fl_Check_Button* include_image_button;
extern void align_cb(Fl_Button*, void*);
#include <efltk/Fl_Value_Input.h>
extern void x_cb(Fl_Value_Input*, void*);
extern Fl_Value_Input* widget_x;
extern void y_cb(Fl_Value_Input*, void*);
extern Fl_Value_Input* widget_y;
extern void width_cb(Fl_Value_Input*, void*);
extern Fl_Value_Input* widget_width;
extern void height_cb(Fl_Value_Input*, void*);
extern Fl_Value_Input* widget_height;
extern void set_xy_cb(Fl_Check_Button*, void*);
extern Fl_Check_Button* set_xy;
extern void slider_size_cb(Fl_Value_Input*, void*);
extern void border_cb(Fl_Check_Button*, void*);
extern void modal_cb(Fl_Check_Button*, void*);
extern void non_modal_cb(Fl_Check_Button*, void*);
extern void value_cb(Fl_Value_Input*, void*);
extern void min_cb(Fl_Value_Input*, void*);
extern void max_cb(Fl_Value_Input*, void*);
extern void step_cb(Fl_Value_Input*, void*);
extern void line_cb(Fl_Value_Input*, void*);
extern void visible_cb(Fl_Check_Button*, void*);
extern void active_cb(Fl_Check_Button*, void*);
extern void resizable_cb(Fl_Check_Button*, void*);
extern void hotspot_cb(Fl_Check_Button*, void*);
#include "Shortcut_Button.h"
extern void shortcut_in_cb(Shortcut_Button*, void*);
#include <efltk/Fl_Wordwrap_Input.h>
extern void tooltip_cb(Fl_Wordwrap_Input*, void*);
extern void box_cb(Fl_Choice*, void*);
#include <efltk/Fl_Light_Button.h>
extern void color_cb(Fl_Light_Button*, void*);
extern void highlight_color_cb(Fl_Light_Button*, void*);
extern void button_box_cb(Fl_Choice*, void*);
extern void button_color_cb(Fl_Light_Button*, void*);
extern void selection_color_cb(Fl_Light_Button*, void*);
extern void focus_box_cb(Fl_Choice*, void*);
extern void label_type_cb(Fl_Choice*, void*);
extern void label_font_cb(Fl_Choice*, void*);
extern void label_size_cb(Fl_Value_Input*, void*);
extern void label_color_cb(Fl_Light_Button*, void*);
extern void highlight_label_color_cb(Fl_Light_Button*, void*);
extern void text_font_cb(Fl_Choice*, void*);
extern void text_size_cb(Fl_Value_Input*, void*);
extern void text_color_cb(Fl_Light_Button*, void*);
extern void selected_text_color_cb(Fl_Light_Button*, void*);
extern void default_cb(Fl_Button*, void*);
extern void name_cb(Fl_Input*, void*);
extern void name_public_cb(Fl_Check_Button*, void*);
#include <efltk/Fl_Text_Editor.h>
extern void extra_code_input_cb(Fl_Text_Editor*, void*);
extern Fl_Text_Editor* extra_code_input;
extern void callback_cb(Fl_Text_Editor*, void*);
extern void user_data_cb(Fl_Input*, void*);
extern void when_cb(Fl_Choice*, void*);
extern void user_data_type_cb(Fl_Input*, void*);
extern int overlays_invisible;
extern void overlay_cb(Fl_Check_Button*, void*);
extern Fl_Check_Button* overlaybutton;
extern void cancel_cb(Fl_Button*, void*);
extern void ok_cb(Fl_Button*, void*);
Fl_Window* make_widget_panel();
#endif
