#ifndef __ICONTOOLTIP_H__
#define __ICONTOOLTIP_H__

#include <efltk/Fl_Window.h>
#include <efltk/Fl_Image.h>
#include <efltk/Fl_String.h>
#include <efltk/Fl_Box.h>

/* Tooltip manager for icons on desktop.
 * It can be as ordinary efltk tooltip (PLAIN)
 * or EXTENDED, with icon and some details.
 */

#define PLAIN    1
#define EXTENDED 2

class IconTooltip : public Fl_Window
{
	private:
		Fl_Box* img_box;
		Fl_Box* title_box;
		Fl_Box* cmd_box;

		bool timeout_added;
		bool cshow;

	public:
		IconTooltip();
		~IconTooltip();
		void set_data(Fl_Image* img, const char* title, const char* cmd);
		void show_tooltip(int x, int y, int type);
		void hide_tooltip(void);
		bool can_show(void) { return cshow; }
};

#endif
