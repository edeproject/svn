#ifndef __ICONDIALOG_H__
#define __ICONDIALOG_H__

#include <efltk/Fl_Window.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Check_Button.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_String.h>
#include <efltk/Fl_Input.h>

class IconSettings;

class IconDialog : public Fl_Window
{
	private:
		IconSettings* settings;

		Fl_Button* close;
		Fl_Button* apply;

		Fl_Button* img_path_button;
		Fl_Button* cmd_path_button;
		Fl_Button* link_path_button;
		Fl_Button* term_path_button;

		Fl_Box* img_box;

		Fl_Input* name;
		Fl_Input* img_path;
		Fl_Input* cmd_path;
		Fl_Input* link_path;
		Fl_Input* term_path;

	public:
		IconDialog(IconSettings* is, Fl_Image* img);
		~IconDialog();
};
#endif
