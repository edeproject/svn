#include <FL/Fl.H>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <edelib/Missing.h>
#include "Applet.h"
#include "MemMonitor.h"

#define UPDATE_INTERVAL .5f
#define STR_CMP(first, second, n) (strncmp(first, second, n) == 0)

static long get_number(const char *ln) {
	char *s = edelib_strndup(ln, 128);
	int i = 1;

	for(char *p = strtok(s, " "); p; p = strtok(NULL, " "), i++) {
		if(i == 2)
			return atol(p);
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
	Fl::add_timeout(UPDATE_INTERVAL, mem_timeout_cb, this);
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
}

EDE_PANEL_APPLET_EXPORT (
 MemMonitor, 
 EDE_PANEL_APPLET_OPTION_ALIGN_RIGHT,
 "Memory monitor",
 "0.1",
 "empty",
 "Sanel Zukan"
)
