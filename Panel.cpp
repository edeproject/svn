#include <stdio.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include "Panel.h"

#undef MIN
#define MIN(x,y)  ((x) < (y) ? (x) : (y))
#undef MAX
#define MAX(x,y)  ((x) > (y) ? (x) : (y))

inline bool intersects(Fl_Widget *o1, Fl_Widget *o2) {
	    return (MAX(o1->x(), o2->x()) <= MIN(o1->x() + o1->w(), o2->x() + o2->w()) &&
	            MAX(o1->y(), o2->y()) <= MIN(o1->y() + o1->h(), o2->y() + o2->h()));
}

static void move_widget(Panel *self, Fl_Widget *o, int &sx, int &sy) {
	int tx, ty, px, py;
	Fl_Widget *const *a;

	tx = Fl::event_x() - sx;
	ty = Fl::event_y() - sy;

	px = o->x() + tx;
	py = o->y() + ty;

	/* check the bounds */
	if(px < 0)
		px = 0;
	if(px + o->w() > self->w())
		px = self->w() - o->w();
	if(py < 0)
		py = 0;
	if(py + o->h() > self->y() + self->h())
		py = self->y() + self->h() - o->h();
	if(py + o->h() > self->h())
		py = self->h() - o->h();

	o->position(px, py);

	/* find possible collision and move others */
	a = self->array();
	for(int i = self->children(); i--; ) {
		if(o == a[i])
			continue;

		if(intersects(a[i], o)) {
			px = a[i]->x() + tx;
			py = a[i]->y() + ty;

			/* check the bounds */
			if(px < 0)
				px = 0;
			if(px + o->w() > self->w())
				px = self->w() - o->w();
			if(py < 0)
				py = 0;
			if(py + o->h() > self->y() + self->h())
				py = self->y() + self->h() - o->h();
			if(py + o->h() > self->h())
				py = self->h() - o->h();

			a[i]->position(px, py);
		}
	}

	/* update current position */
	sx = Fl::event_x();
	sy = Fl::event_y();

	o->parent()->redraw();
}

int Panel::handle(int e) {
	switch(e) {
		case FL_PUSH:
			sx = Fl::event_x();
			sy = Fl::event_y();

			clicked = Fl::belowmouse();
			if(clicked == this)
				clicked = 0;
			else
				clicked->handle(e);

			sx = Fl::event_x();
			sy = Fl::event_y();
			return 1;

		case FL_DRAG:
			if(clicked) {
				fl_cursor(FL_CURSOR_MOVE);
				move_widget(this, clicked, sx, sy);
				clicked->handle(e);
			}
			return 1;

		case FL_RELEASE:
			fl_cursor(FL_CURSOR_DEFAULT);

			if(clicked) {
				clicked->handle(e);
				clicked = 0;
			}
			return 1;
	}

	return Fl_Window::handle(e);
}
