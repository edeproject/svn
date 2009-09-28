#include <stdlib.h>

#include <FL/Fl_Pixmap.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <edelib/Debug.h>

#include "TaskButton.h"
#include "Netwm.h"
#include "icons/window.xpm"

static Fl_Pixmap image_window(window_xpm);

TaskButton::TaskButton(int X, int Y, int W, int H, const char *l) : Fl_Button(X, Y, W, H, l), xid(0) { 
	box(FL_UP_BOX);
	align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT | FL_ALIGN_CLIP);

	image(image_window);
}

void TaskButton::draw(void) {
	Fl_Color col = value() ? selection_color() : color();
	draw_box(value() ? (down_box() ? down_box() : fl_down(box())) : box(), col);

	if(image()) {
		int X, Y, lw, lh;

		X = x() + 5;
		Y = (y() + h() / 2) - (image()->h() / 2);
		image()->draw(X, Y);

		X += image()->w() + 5;

		if(label()) {
			fl_font(labelfont(), labelsize());
			fl_color(labelcolor());

			lw = lh = 0;
			fl_measure(label(), lw, lh, align());

			/* use clipping so long labels do not be drawn on the right border, which looks ugly */
			fl_push_clip(x() + Fl::box_dx(box()), 
						 y() + Fl::box_dy(box()), 
						 w() - Fl::box_dw(box()) - 5, 
						 h() - Fl::box_dh(box()));

				Y = (y() + h() / 2) - (lh / 2);
				fl_draw(label(), X, Y, lw, lh, align(), 0, 0);

			fl_pop_clip();
		}
	} else {
		draw_label();
	}

	if(Fl::focus() == this)
		draw_focus();
}

void TaskButton::update_title_from_xid(void) {
	E_RETURN_IF_FAIL(xid >= 0);

	char *title = netwm_get_window_title(xid);
	if(!title) {
		label("...");
		tooltip("...");
	} else {
		copy_label(title);
		tooltip(label());
		free(title);
	}
}
