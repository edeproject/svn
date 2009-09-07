#include "Applet.h"

#include <FL/Fl_Box.H>
#include <FL/Fl.H>
#include <time.h>

#include <edelib/Debug.h>

static void clock_refresh(void *o);

class Clock : public Fl_Box {
private:
	char buf[64];
public:
	Clock() : Fl_Box(450, 0, 90, 25, NULL) { 
		box(FL_DOWN_BOX);
	}

	~Clock() {
		Fl::remove_timeout(clock_refresh);
	}

	int handle(int e);
	void update_time(void);
};

static void clock_refresh(void *o) {
	Clock *c = (Clock *)o;
	c->update_time();

	Fl::repeat_timeout(1.0, clock_refresh, o);
}

void Clock::update_time(void) {
	time_t    t;
	struct tm *tmp;

	t = time(NULL);
	tmp = localtime(&t);
	if(!tmp)
		return;

	strftime(buf, sizeof(buf), "%H:%M:%S", tmp);
	label(buf);
}

int Clock::handle(int e) {
	switch(e) {
		case FL_SHOW:
			Fl::add_timeout(0, clock_refresh, this);
			break;
	}

	return Fl_Box::handle(e);
}

EDE_PANEL_APPLET_EXPORT (
 Clock, 
 "Clock applet",
 "0.1",
 "empty",
 "Sanel Zukan"
)
