/*
 * $Id: eiconman.h 1653 2006-06-09 13:08:58Z karijes $
 *
 * Eiconman, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef __WALLPAPER_H__
#define __WALLPAPER_H__

#include <efltk/Fl_Image.h>
#include <efltk/Fl_String.h>

// wallpaper display modes
#define WCENTER         0
#define WSTRETCH        1
#define WSTRETCH_ASPECT 2

/* redefined Fl_Image because of stupid design
 * I can't extract updated Pixmap part
 */
#if 0
class WallpaperImage : public Fl_Image
{
	public:
		WallpaperImage(int W, int H, Fl_PixelFormat* fmt);
		~WallpaperImage();
		void _draw(int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, Fl_Flags f);
};
#endif

class Wallpaper
{
	private:
		int desktop_w, desktop_h;
		Fl_Image* img;
		Fl_Image* img_preview;
		int img_opacity;
		short img_mode;
		bool loaded;

	public:
		Wallpaper();
		~Wallpaper();
		bool load(Fl_String& path);
		bool is_loaded(void);

		Fl_Image* wimage(void) const;
		Fl_Image* wimage_preview(void) const;

		void set_opacity(int o, Fl_Color bg);
		void set_mode(int m, int w, int h);
		int opacity(void);
		int mode(void);

		void apply(void);
};

#endif
