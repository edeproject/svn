#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <edelib/Color.h>
#include <edelib/Missing.h>
#include "Applet.h"
#include "MemMonitor.h"

EDELIB_NS_USING(color_rgb_to_fltk)

#define UPDATE_INTERVAL 1.0f
#define STR_CMP(first, second, n) (strncmp(first, second, n) == 0)

static int height_from_perc(int perc, int h) {
	return (perc * h) / 100;
}

static long get_number(const char *ln) {
	char *s = edelib_strndup(ln, 128);
	int i = 1;

	for(char *p = strtok(s, " "); p; p = strtok(NULL, " "), i++) {
		if(i == 2) {
			long ret = atol(p);
			free(s);
			return ret;
		}
	}

	free(s);
	return 0;
}
static void mem_timeout_cb(void *d) {
	((MemMonitor*)d)->update_status();
	Fl::repeat_timeout(UPDATE_INTERVAL, mem_timeout_cb, d);
}

MemMonitor::MemMonitor() : Fl_Box(0, 0, 45, 25), mem_usedp(0), swap_usedp(0) {
	box(FL_THIN_DOWN_BOX);
}

void MemMonitor::update_status(void) {
	FILE *fd = fopen("/proc/meminfo", "r");
	if(!fd) return;

	long mem_total, mem_free, swap_total, swap_free;
	mem_total = mem_free = swap_total = swap_free = 0;

	char buf[128];
	while(fgets(buf, 128, fd) != 0) {
		if(STR_CMP(buf, "MemTotal:", 9))
			mem_total = get_number(buf);
		else if(STR_CMP(buf, "MemFree:", 8))
			mem_free = get_number(buf);
		else if(STR_CMP(buf, "SwapTotal:", 10))
			swap_total = get_number(buf);
		else if(STR_CMP(buf, "SwapFree:", 9))
			swap_free = get_number(buf);
	}

	fclose(fd);

	mem_usedp  = 100 - (int)(((float)mem_free / (float)mem_total) * 100);
	swap_usedp = 100 - (int)(((float)swap_total / (float)swap_free) * 100);

	static char tip[100];
	snprintf(tip, sizeof(tip), "Memory used: %i%%\nSwap used: %i%%", mem_usedp, swap_usedp);
	tooltip(tip);

	redraw();
}

void MemMonitor::draw(void) {
	draw_box();
	int W, W2, H, X, Y, mh, sh;

	W = w() - Fl::box_dw(box());
	H = h() - Fl::box_dh(box());
	X = x() + Fl::box_dx(box());
	Y = y() + Fl::box_dy(box());
	W2 = W / 2;

	mh = height_from_perc(mem_usedp, H);
	sh = height_from_perc(swap_usedp, H);

	fl_rectf(X, Y + H - mh, W2, mh, (Fl_Color)color_rgb_to_fltk(166, 48, 48));
	fl_rectf(X + W2, Y + H - sh, W2, sh, (Fl_Color)color_rgb_to_fltk(54, 136, 79));
}

int MemMonitor::handle(int e) {
	switch(e) {
		case FL_SHOW: {
			int ret = Fl_Box::handle(e);
			Fl::add_timeout(UPDATE_INTERVAL, mem_timeout_cb, this);
			return ret;
		}

		case FL_HIDE:
			Fl::remove_timeout(mem_timeout_cb);
			/* fallthrough */
	}

	return Fl_Box::handle(e);
}

EDE_PANEL_APPLET_EXPORT (
 MemMonitor, 
 EDE_PANEL_APPLET_OPTION_ALIGN_RIGHT,
 "Memory monitor",
 "0.1",
 "empty",
 "Sanel Zukan"
)
