#include "icontooltip.h"
#include <efltk/Fl_Tooltip.h>
#include <assert.h>
#include <stdio.h>

#define DEFAULT_W     195
#define DEFAULT_H     70
#define CLOSE_TIMEOUT 5.0

void tooltip_close_timeout(void* t)
{
	puts("tooltip_close_timeout");
	IconTooltip* ttip = (IconTooltip*)t;
	ttip->hide();
}

void tooltip_show_timeout(void* t)
{
	puts("tooltip_show_timeout");
	IconTooltip* ttip = (IconTooltip*)t;
	ttip->show();
}

IconTooltip::IconTooltip() : Fl_Window(DEFAULT_W, DEFAULT_H)
{
	title_box = new Fl_Box(5, 9, 120, 25);
	title_box->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	cmd_box = new Fl_Box(5, 35, 120, 25);
	cmd_box->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	img_box = new Fl_Box(130, 5, 60, 60);

	clear_border();
	end();
}

IconTooltip::~IconTooltip()
{
	puts("IconTooltip::~IconTooltip()");
}

void IconTooltip::set_data(Fl_Image* img, const char* title, const char* cmd)
{
	assert(img != NULL);
	assert(title != NULL);
	assert(cmd != NULL);

	img_box->image(img);

	title_box->label(title);
	title_box->label_font(FL_HELVETICA_BOLD);

	cmd_box->label_font(Fl_Tooltip::font());
	cmd_box->label_size(Fl_Tooltip::size());
	cmd_box->label(cmd);
}

void IconTooltip::show_tooltip(int x, int y, int type)
{
	Fl::remove_timeout(tooltip_show_timeout);
	Fl::remove_timeout(tooltip_close_timeout);

	box(Fl_Tooltip::boxtype());
	color(Fl_Tooltip::color());
	position(x, y);
	printf("position: %i %i\n", x, y);
	printf("ENTERED ----\n");

	Fl::add_timeout(Fl_Tooltip::delay(), tooltip_show_timeout, this);
	Fl::add_timeout(CLOSE_TIMEOUT, tooltip_close_timeout, this);
}

void IconTooltip::hide_tooltip(void)
{
	printf("LEAVED ----\n");
	Fl::remove_timeout(tooltip_show_timeout);
	Fl::remove_timeout(tooltip_close_timeout);

	if(shown())
		hide();
}

#if 0
int main()
{
	IconTooltip* it = new IconTooltip();
	it->show_tooltip(EXTENDED);
	while(1)
		Fl::wait();

	return 0;
}
#endif
