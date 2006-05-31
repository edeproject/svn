#ifndef __TITLEBAR_H__
#define __TITLEBAR_H__

#include <efltk/Fl_Button.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Image.h>
#include <efltk/Fl_Menu_.h>
#include <efltk/x.h>

class Frame;

class TitlebarButton : public Fl_Button
{
	private:
		int m_type;
	public:
		TitlebarButton(int type);
		~TitlebarButton();
		void draw();
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
		void popup_menu(Frame*);
		int handle(int event);
		void layout();
		void draw();
		void handle_double_click();

		// called from Frame::maximize()
		void handle_maximize(bool is_max);

		// These three options are set from Windowmanager.cpp, but defaults are
		// in Titlebar.cpp
		static int default_height;
		static int label_align;
		// type of box for titlebar (excluding buttons) when theme is not used
		static int box_type;
};

#endif
