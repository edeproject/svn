#ifndef __TITLEBAR_H__
#define __TITLEBAR_H__

#include "WMWindow.h"
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Image.h>
#include <efltk/Fl_Menu_.h>
#include <efltk/Fl_Value_Input.h>
#include <efltk/x.h>

class Frame;

// small window for setting size of target window
class SetSizeWindow : public WMWindow
{
	private:
		Frame* curr_frame;
		Fl_Value_Input* w_width;
		Fl_Value_Input* w_height;

	public:
		SetSizeWindow(Frame* f);
		~SetSizeWindow();
		Frame* frame() { return curr_frame; }
		double frame_w() { return w_width->value(); }
		double frame_h() { return w_height->value(); }
		void run();
};

class TitlebarButton : public Fl_Button
{
	private:
		int m_type;
		Frame* curr_frame;
	public:
		TitlebarButton(int type);
		~TitlebarButton();
		void draw();
		void frame(Frame* f) { curr_frame = f; }
		// used to see what is button in maximize or restore state
		void type(int t) { m_type = t; }
		int type() { return m_type; }
};

class Titlebar : public Fl_Window
{
	private:
		TitlebarButton minb, maxb, closeb;
		Frame* curr_frame;
		Fl_Image* title_icon;
		int text_w;
		Fl_Menu_* title_menu;

	public:
		Titlebar(int x, int y, int w, int h, const char* l = 0);
		~Titlebar();
		void setting_changed();
		void show();
		void hide();
		void popup_menu();
		int handle(int event);
		void layout();
		void draw();
		void handle_double_click();
		void update_desktops(Fl_Group* g);
		void frame(Frame* f) { curr_frame = f; }
		Frame* frame() { return curr_frame; }

		// called from Frame::maximize()
		void handle_maximize(bool is_max);

		// These three options are set from Windowmanager.cpp, but defaults are
		// in Titlebar.cpp
		static int default_height;
		static int label_align;
		// type of box for titlebar (excluding buttons) when theme is not used
		static int box_type;

		// here is to access Frame's private data, since is it's friend
		static void cb_change_desktop(Fl_Widget* w, void* data);
};

#endif
