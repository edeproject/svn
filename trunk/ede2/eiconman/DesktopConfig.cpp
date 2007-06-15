/*
 * $Id$
 *
 * Eiconman, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2007 EDE Authors.
 *
 * This program is licensed under terms of the 
 * GNU General Public License version 2 or newer.
 * See COPYING for details.
 */

#include "DesktopConfig.h"
#include "Utils.h"
#include "eiconman.h"

#include <edelib/Nls.h>
#include <edelib/Debug.h>
#include <fltk/Item.h>
#include <fltk/ColorChooser.h>
#include <fltk/file_chooser.h>
#include <fltk/SharedImage.h>
#include <fltk/run.h>
#include <fltk/draw.h>
#include <fltk/x11.h>

void close_cb(fltk::Widget*, void* w) {
	DesktopConfig* dc = (DesktopConfig*)w;
	dc->hide();
}

void ok_cb(fltk::Widget*, void* w) { }

void apply_cb(fltk::Widget*, void* w) { 
	DesktopConfig* dc = (DesktopConfig*)w;

	//Desktop::instance()->set_bg_color(dc->bg_color());
}

void wp_use_cb(fltk::Widget*, void* w) {
	DesktopConfig* dc = (DesktopConfig*)w;
	dc->wp_disable();
}

void color_box_cb(fltk::Widget*, void* w) {
	DesktopConfig* dc = (DesktopConfig*)w;

	fltk::Color col = dc->bg_color();
	if(fltk::color_chooser(_("Background color"), col))
		dc->set_color(col);
}

void img_browse_cb(fltk::Widget*, void* w) {
	DesktopConfig* dc = (DesktopConfig*)w;
	
	const char* f = fltk::file_chooser(_("Choose image"), "Image files (*.{jpg,png,xpm})", 0);
	if(f)
		dc->set_preview_image(f);
};

PreviewBox::PreviewBox(int x, int y, int w, int h, const char* l) : fltk::InvisibleBox(x,y,w,h)
{
}

PreviewBox::~PreviewBox()
{
}

void PreviewBox::draw(void) {
	draw_box();

	if(image()) {
		fltk::Rectangle ir(0,0,w(),h());
		box()->inset(ir);
		fltk::Image* im = (fltk::Image*)image();
		im->draw(ir);
	}
}

DesktopConfig::DesktopConfig() : fltk::Window(540, 265, _("Background settings")), img_enable(false), wp_img(NULL) {
	begin();
		// monitor
		fltk::InvisibleBox* m1 = new fltk::InvisibleBox(75, 175, 100, 15);
		m1->box(fltk::BORDER_BOX);
		fltk::InvisibleBox* m2 = new fltk::InvisibleBox(20, 20, 210, 160);
		m2->box(fltk::THIN_UP_BOX);

		preview = new PreviewBox(30, 30, 190, 140);
		preview->box(fltk::DOWN_BOX);
		preview->color(56);

		fltk::InvisibleBox* m4 = new fltk::InvisibleBox(50, 185, 145, 15);
		m4->box(fltk::THIN_UP_BOX);

		// fetch workspace names
		fltk::Choice* ws_names = new fltk::Choice(310, 20, 220, 25, _("Desktop:"));
		ws_names->begin();

		new fltk::Item(_("All desktops"));

		ws_names->end();

		// rest
		use_wp = new fltk::CheckButton(310, 60, 20, 20, _("Use wallpaper"));
		use_wp->align(fltk::ALIGN_RIGHT);
		use_wp->callback(wp_use_cb, this);
		use_wp->value(1);

		img_path = new fltk::Input(310, 95, 190, 25, _("Image:"));
		img_browse = new fltk::Button(505, 95, 25, 25, "...");
		img_browse->callback(img_browse_cb, this);

		img_choice = new fltk::Choice(310, 130, 220, 25, _("Mode:"));
		img_choice->begin();
			new fltk::Item(_("Normal"));
			new fltk::Item(_("Center"));
			new fltk::Item(_("Stretch"));
			new fltk::Item(_("Aspect stretch"));
			new fltk::Item(_("Tile"));
		img_choice->end();
   
		color_box = new fltk::Button(310, 175, 25, 25, _("Background color"));
		color_box->color((fltk::Color)0x3ca700);
		color_box->align(fltk::ALIGN_RIGHT);
		color_box->callback(color_box_cb, this);

		fltk::Button* ok = new fltk::Button(250, 230, 90, 25, _("&OK"));
		ok->callback(ok_cb, this);

		fltk::Button* apply = new fltk::Button(345, 230, 90, 25, _("&Apply"));
		apply->callback(apply_cb, this);

		fltk::Button* close = new fltk::Button(440, 230, 90, 25, _("&Cancel"));
		close->callback(close_cb, this);
	end();
}

DesktopConfig::~DesktopConfig()
{
}

void DesktopConfig::run(void) {
	if(!shown())
		show();

	while(visible())
		fltk::wait();
}

void DesktopConfig::wp_disable(void) {
	if(!use_wp->value()) {
		img_path->deactivate();
		img_browse->deactivate();
		img_choice->deactivate();
	} else {
		img_path->activate();
		img_browse->activate();
		img_choice->activate();
	}

	img_enable = false;
	preview->image(NULL);
	set_color(color_box->color());
}

void DesktopConfig::set_preview_color(unsigned int c) {
	if(c == preview->color())
		return;

	preview->color(c);
	preview->redraw();
}

void DesktopConfig::set_color(unsigned int c) {
	if(c != color_box->color()) {
		color_box->color(c);
		color_box->redraw();
	}

	set_preview_color(c);
}

void DesktopConfig::set_preview_image(const char* path) {
	EASSERT(path != NULL);

	if(wp_path == path)
		return;
	wp_path = path;
	wp_img  = fltk::SharedImage::get(wp_path.c_str());

	preview->image(wp_img);
	preview->redraw();
	img_path->value(wp_path.c_str());
}