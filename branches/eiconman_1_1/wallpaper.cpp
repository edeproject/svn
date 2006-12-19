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

#include "wallpaper.h"
#include <efltk/Fl_Renderer.h>
#include <efltk/fl_draw.h> // fl_transform
#include <efltk/x.h>
#include <stdio.h>
#include <assert.h>

Wallpaper::Wallpaper() : desktop_w(0), desktop_h(0)
{
	loaded = false;
}

Wallpaper::~Wallpaper()
{
	if(loaded && img)
	{
		delete img;
		puts("Clearing image...");
	}
}

bool Wallpaper::load(Fl_String& path)
{
	img = Fl_Image::read(path, 0);
	if(img)
	{
		Fl_Renderer::system_init();
		img->system_convert();
		loaded = true;
		return true;
	}
	else
	{
		loaded = false;
		return false;
	}
}

/* Probably not the best thing to do, but will
 * save us from messy code. After image is loaded
 * apply needed modes (stretch,tile...) and if needed
 * blit it against the background.
 * This is also needed to keep same look for _XROOTPMAP_ID.
 */
void Wallpaper::apply(void)
{
	if(!loaded)
		return;

    Fl_Renderer::system_init();

	Fl_Image* newim = img;
	int iw=img->width(), ih=img->height();

	// TODO: pickup these from Desktop x,y
	int ix=0, iy=0;
	int xoff=0, yoff=0;

	switch(img_mode)
	{
		case WCENTER:
			ix=(desktop_w/2)-(iw/2);
			iy=(desktop_h/2)-(ih/2);
			if(ix<0) xoff=-ix; if(iy<0) yoff=-iy;
			if(ix<0) ix=0; if(iy<0) iy=0;
			break;
		case WSTRETCH:
			ix=0, iy=0, iw=desktop_w, ih=desktop_h;
			if(desktop_w != img->width() || desktop_h != img->height())
				newim = img->scale(desktop_w,desktop_h);
 
		case WSTRETCH_ASPECT:
		{
			int pbw = desktop_w, pbh = desktop_h;
			iw = pbw;
			ih = iw * img->height() / img->width();
			if(ih > pbh) 
			{
				ih = pbh;
				iw = ih * img->width() / img->height();
			}
			ix=(desktop_w/2)-(iw/2), iy=(desktop_h/2)-(ih/2);
			if(ix<0) ix=0; if(iy<0) iy=0;
			if(iw>desktop_w) iw=desktop_w; if(ih>desktop_h) ih=desktop_h;
			if(iw!=img->width()||ih!=img->height())
				newim = img->scale(iw,ih);
			break;
		}
		default:
			// should not be here
			assert(0);
	}

	newim->format()->alpha = img_opacity;
    if(iw > desktop_w) 
		iw=desktop_w; 
	if(ih > desktop_h) 
		ih=desktop_h;

	Fl_Rect r1(xoff, yoff, iw, ih);
	Fl_Rect r2(ix,iy, iw, ih);

	Fl_Image* bg_image = new Fl_Image(desktop_w, desktop_h, Fl_Renderer::system_format(), 0);
 	if(newim->format()->alpha > 0) 
	{
		// Blit image data to our bg_image
		bg_image->check_map(newim->format());
		Fl_Renderer::alpha_blit(newim->data(), &r1, newim->format(), newim->pitch(),
								bg_image->data(), &r2, bg_image->format(), bg_image->pitch(), 0);
    }

	if(newim != img)
		delete newim;

	delete img;
	img = bg_image;
}

Fl_Image* Wallpaper::wimage(void) const
{
	return img;
}

Fl_Image* Wallpaper::wimage_preview(void) const
{
	return img_preview;
}

void Wallpaper::set_opacity(int o, Fl_Color bg)
{
	// 255 means no opacity will be applied
	if(o <= 255)
		img_opacity = o;
	else
		img_opacity = 255;
}

void Wallpaper::set_mode(int m, int w, int h)
{
	img_mode = m;
	desktop_w = w;
	desktop_h = h;
}

int Wallpaper::opacity(void)
{
	return img_opacity;
}

int Wallpaper::mode(void)
{
	return img_mode;
}

bool Wallpaper::is_loaded(void)
{
	return loaded;
}
