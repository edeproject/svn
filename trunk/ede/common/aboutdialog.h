// generated by Extended Fast Light User Interface Designer (eFluid) version 2,0003

#ifndef _ABOUTDIALOG_H_
#define _ABOUTDIALOG_H_

#include <efltk/Fl.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Button.h>


class AboutDialog  {
public:
	AboutDialog(const char *progname, const char *progversion, const char *addcomment = 0);
private:
		inline void cb_Click_i(Fl_Button*, void*);
		static void cb_Click(Fl_Button*, void*);
		inline void cb_Close_i(Fl_Button*, void*);
		static void cb_Close(Fl_Button*, void*);
	void showCopyingInfo();
};

#endif