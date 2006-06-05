#ifndef _THEME_H_
#define _THEME_H_

#include <efltk/Fl_Image.h>
#include <efltk/Fl_Image_List.h>
#include <efltk/Fl_Multi_Image.h>
#include <efltk/Fl_Config.h>
#include <efltk/Fl_String.h>
/*
enum {
    TITLEBAR_BG = 0,     TITLEBAR_BG_UNFOCUSED,
    TITLEBAR_CLOSE_UP,   TITLEBAR_CLOSE_DOWN,
    TITLEBAR_MAX_UP,     TITLEBAR_MAX_DOWN,
    TITLEBAR_RESTORE_UP, TITLEBAR_RESTORE_DOWN,
    TITLEBAR_MIN_UP,     TITLEBAR_MIN_DOWN,
    IMAGES_LAST
};
*/

enum 
{
	TITLEBAR_BG = 0,     TITLEBAR_BG_UNFOCUSED,
    TITLEBAR_CLOSE_UP,   TITLEBAR_CLOSE_DOWN,   TITLEBAR_CLOSE_UNFOCUSED,
    TITLEBAR_MAX_UP,     TITLEBAR_MAX_DOWN,     TITLEBAR_MAX_UNFOCUSED,
    TITLEBAR_RESTORE_UP, TITLEBAR_RESTORE_DOWN, TITLEBAR_RESTORE_UNFOCUSED,
    TITLEBAR_MIN_UP,     TITLEBAR_MIN_DOWN,     TITLEBAR_MIN_UNFOCUSED,
    IMAGES_LAST
};

class Theme
{
	private:
		bool use_theme;
		int tbheight;
		Fl_Image* images[IMAGES_LAST];
		Fl_Image_List imglist;
		Fl_Color framecol;
		Fl_Color framecol_unfocus;
		void load_image(const char* name, int type, const Fl_String &path, Fl_Config &cfg);
		Theme();
		~Theme();
		Theme(const Theme&);
		Theme& operator=(const Theme&);
		static Theme* pinstance;

	public:
		static Theme* instance();
		void shutdown();
		void use(bool val) { use_theme = val; }
		bool use(void) { return use_theme; }
		bool load(const Fl_String &file);
		void unload();
		int titlebar_h() { return tbheight; }
		Fl_Image* image(int which);
		Fl_Color frame_color() { return framecol; }
		Fl_Color frame_color_unfocused() { return framecol_unfocus; }
};
	

/*
namespace Theme {

extern bool use_theme();
extern void use_theme(bool val);

extern bool load_theme(const Fl_String &file);
extern void unload_theme();
extern int titlebar_height();

extern Fl_Image *image(int which);
extern Fl_Color frame_color();

};
*/

#endif
