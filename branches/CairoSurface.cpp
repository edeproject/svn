/*
 * Cairo integration with FLTK
 * See README for more info.
 */

#include <FL/Fl_Window.h>
#include <FL/Fl_Double_Window.h>
#include <FL/x.h>
#include <FL/Fl.h>
#include <FL/fl_draw.h>
#include <cairo/cairo-xlib.h>

#define FLTK_WINDOW Fl_Window

class CairoSurface : public FLTK_WINDOW {
	private:
		cairo_surface_t* surface;
		cairo_t* ctx;

	public:
		CairoSurface(int W, int H, const char* l = 0) : FLTK_WINDOW(W, H, l), surface(0), ctx(0) { }
		~CairoSurface() { }
		virtual void show(void);
		virtual void hide(void);
		virtual void draw(void);
		virtual void flush(void);
		virtual void resize(int, int, int, int);
		void flush(int);
		void cairo_draw(void);
};

void CairoSurface::resize(int X, int Y, int W, int H) {
		cairo_xlib_surface_set_size(surface, W, H);
}

void CairoSurface::show(void) {
	if(shown())
		return;

	fl_open_display();
	Fl_X::make_xid(this);

	surface = cairo_xlib_surface_create(fl_display, fl_xid(this), fl_visual->visual, w(), h());
	ctx = cairo_create(surface);
}

void CairoSurface::flush(void) {
	FLTK_WINDOW::flush();
	cairo_surface_flush(surface);
#if 0
	surface = cairo_xlib_surface_create(fl_display, fl_xid(this), fl_visual->visual, w(), h());
	ctx = cairo_create(surface);

	cairo_draw();

	cairo_destroy(ctx);
	cairo_surface_destroy(surface);
#endif
}


#define M_PI 3.1415

void CairoSurface::cairo_draw(void) {
	//surface = cairo_xlib_surface_create(fl_display, Fl_X::i(this)->other_xid, fl_visual->visual, w(), h());
	//ctx = cairo_create(surface);

	/* paint background to white */
	cairo_set_source_rgb (ctx, 1.0, 1.0, 1.0);
	cairo_paint (ctx);

	double xc = 128.0;
	double yc = 128.0;
	double radius = 100.0;
	double angle1 = 45.0  * (M_PI/180.0);  /* angles are specified */
	double angle2 = 180.0 * (M_PI/180.0);  /* in radians           */

	/* paint arc to black */
	cairo_set_source_rgb (ctx, 0, 0, 0);
	cairo_set_line_width (ctx, 10.0);
	cairo_arc (ctx, xc, yc, radius, angle1, angle2);
	cairo_stroke (ctx);

	/* draw helping lines */
	cairo_set_source_rgba (ctx, 1, 0.2, 0.2, 0.6);
	cairo_set_line_width (ctx, 6.0);

	cairo_arc (ctx, xc, yc, 10.0, 0, 2*M_PI);
	cairo_fill (ctx);

	cairo_arc (ctx, xc, yc, radius, angle1, angle1);
	cairo_line_to (ctx, xc, yc);
	cairo_arc (ctx, xc, yc, radius, angle2, angle2);
	cairo_line_to (ctx, xc, yc);
	cairo_stroke (ctx);

	//cairo_destroy(ctx);
	//cairo_surface_destroy(surface);


	
#if 0
	cairo_set_source_rgb (ctx, 1.0, 1.0, 1.0); /* white */
	cairo_paint (ctx);

	cairo_set_source_rgba (ctx, 0, 0.0, 0.0, 0.50); /* half-alpha-black */

	for (int i = 1; i <= 3; i++) {
		int inset = w() / 8 * i;

		cairo_rectangle (ctx, inset, inset, w() - 2 * inset, h() - 2 * inset);
		cairo_fill (ctx);
	}
#endif

#if 0
	cairo_set_source_rgb (ctx, 0, 0, 0);
	cairo_move_to (ctx, 0, 0);
	cairo_line_to (ctx, 1, 1);
	cairo_move_to (ctx, 1, 0);
	cairo_line_to (ctx, 0, 1);
	cairo_set_line_width (ctx, 0.2);
	cairo_stroke (ctx);

	cairo_rectangle (ctx, 0, 0, 0.5, 0.5);
	cairo_set_source_rgba (ctx, 1, 0, 0, 0.80);
	cairo_fill (ctx);

	cairo_rectangle (ctx, 0, 0.5, 0.5, 0.5);
	cairo_set_source_rgba (ctx, 0, 1, 0, 0.60);
	cairo_fill (ctx);

	cairo_rectangle (ctx, 0.5, 0, 0.5, 0.5);
	cairo_set_source_rgba (ctx, 0, 0, 1, 0.40);
	cairo_fill (ctx);
#endif
}

void CairoSurface::draw(void) {
	FLTK_WINDOW::draw();
	cairo_draw();
}

void CairoSurface::hide(void) {
	cairo_destroy(ctx);
	cairo_surface_destroy(surface);

	FLTK_WINDOW::hide();
}

int main() {
	CairoSurface cs(400, 400, "Cairo surface in FLTK window");
	cs.resizable(cs);
	cs.show();
	return Fl::run();
}
