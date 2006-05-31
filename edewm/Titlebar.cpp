#include "Titlebar.h"
#include "Windowmanager.h"
#include "Theme.h"
#include "Frame.h"
#include "Icon.h"
#include "debug.h"
#include <efltk/fl_draw.h>

int Titlebar::box_type = 0;
int Titlebar::default_height = 20;
int Titlebar::label_align = FL_ALIGN_LEFT;

extern bool grab();
extern void grab_cursor(bool grab);

// TODO: used all over the files
// make this more flexible
Frame *menu_frame = 0; //This is set to frame,where menu were popped up

// For menu
Fl_Widget* menu_max = 0;
Fl_Widget* menu_min = 0;
Fl_Widget* menu_set_size = 0;
Fl_Widget* menu_desktop = 0;

// drawing stuff
#define MAX_OF .6f
#define MIN_OF .5f
#define vv(x,y) fl_vertex(x,y)

void draw_close(Fl_Color col)
{
	fl_rotate(45);
	fl_color(col);

	vv(-0.9f,-0.12f); vv(-0.9f,0.12f); vv(0.9f,0.12f); vv(0.9f,-0.12f); fl_fill_stroke(FL_DARK3);
	vv(-0.12f,-0.9f); vv(-0.12f,0.9f); vv(0.12f,0.9f); vv(0.12f,-0.9f); fl_fill_stroke(FL_DARK3);
}

void draw_max(Fl_Color col)
{			
	fl_color(col);

	vv(-MAX_OF, -MAX_OF); vv(MAX_OF, -MAX_OF);
	vv(MAX_OF,-MAX_OF+0.4); vv(-MAX_OF,-MAX_OF+0.4);
	fl_fill();

	vv(MAX_OF,-MAX_OF); vv(MAX_OF,MAX_OF);
	vv(-MAX_OF,MAX_OF); vv(-MAX_OF,-MAX_OF);
	fl_stroke();
}

// TODO: finish this
void draw_maximized(Fl_Color col)
{
	fl_color(FL_RED);

	vv(-MAX_OF, -MAX_OF); vv(MAX_OF, -MAX_OF);
	vv(MAX_OF,-MAX_OF+0.4); vv(-MAX_OF,-MAX_OF+0.4);
	fl_fill();

	vv(MAX_OF,-MAX_OF); vv(MAX_OF,MAX_OF);
	vv(-MAX_OF,MAX_OF); vv(-MAX_OF,-MAX_OF);
	fl_stroke();
}

void draw_min(Fl_Color col)
{
	fl_color(col);

	vv(-MIN_OF, MIN_OF); vv(MIN_OF, MIN_OF);
	vv(MIN_OF, MIN_OF+.2f); vv(-MIN_OF, MIN_OF+.2f);
	fl_fill();
}

// callbacks for buttons in titlebar
void button_cb_close(Fl_Widget*, void* f)
{
	Frame* frame = (Frame*)f;
	frame->close();
}

void button_cb_kill(Fl_Widget*, void* f)
{
	Frame* frame = (Frame*)f;
	frame->kill();
}

void button_cb_max(Fl_Widget* w, void* f)
{
	Frame* frame = (Frame*)f;
	if(frame->maximized)
		frame->restore();
	else
		frame->maximize();
}

void button_cb_min(Fl_Widget*, void* f)
{
	Frame* frame = (Frame*)f;
	frame->iconize();
}

// Set size window
// TODO: shouldn't this we palce in another file ???

void set_size_cb(Fl_Widget*, void* f)
{
}

TitlebarButton::TitlebarButton(int type) : Fl_Button(0,0,0,0), m_type(type)
{
	focus_box(FL_NO_BOX);
	label_type(FL_SYMBOL_LABEL);

	switch(m_type) 
	{
		case TITLEBAR_MAX_UP: label("@mx"); break;
		case TITLEBAR_MAXIMIZED_UP: label("@mz"); break;
		case TITLEBAR_MIN_UP: label("@ii"); break;
		case TITLEBAR_CLOSE_UP: label("@xx"); break;
	};
}

TitlebarButton::~TitlebarButton()
{
}

void TitlebarButton::draw()
{
	int idx = m_type;
	if(flags() & FL_VALUE) idx++;

	Fl_Image *i = Theme::image(idx);
	if(i) 
	{
		Fl_Flags scale = 0;
		if(i->height()!=h()) scale = FL_ALIGN_SCALE;
		i->draw(0,0,w(),h(), scale);
	} 
	else 
		Fl_Button::draw();
}

Titlebar::Titlebar(int x, int y, int w, int h, const char* l) : 
Fl_Window(x, y, w, h, l), minb(TITLEBAR_MIN_UP), maxb(TITLEBAR_MAX_UP), closeb(TITLEBAR_CLOSE_UP), text_w(0)
{
	curr_frame = (Frame*)parent();

	// TODO: make this more flexible
	static bool init = false;
	if(!init)
	{
		fl_add_symbol("xx", draw_close, 1);
		fl_add_symbol("mx", draw_max, 1);
		fl_add_symbol("mz", draw_maximized, 1);
		fl_add_symbol("ii", draw_min,1);
	}

	setting_changed();
	
	title_menu = new Fl_Menu_();
	menu_max = title_menu->add(_("Maximize"), 0, button_cb_max, curr_frame);
	menu_min = title_menu->add(_("Minimize"), 0, button_cb_min, curr_frame);
	menu_set_size = title_menu->add(_("Set size"), 0, set_size_cb, curr_frame, FL_MENU_DIVIDER);
	menu_desktop = title_menu->add(_("To desktop"), 0, 0, 0, FL_SUBMENU|FL_MENU_DIVIDER);
	title_menu->add(_("Kill"), 0, button_cb_kill, curr_frame);
	title_menu->add(_("Close"), 0, button_cb_close, curr_frame);
	title_menu->end();

	end();
}

Titlebar::~Titlebar()
{
}

void Titlebar::setting_changed()
{
	minb.callback(button_cb_min, curr_frame);
	maxb.callback(button_cb_max, curr_frame);
	closeb.callback(button_cb_close, curr_frame);

	if(Titlebar::default_height != h())
	{
		h(Titlebar::default_height);
		curr_frame->updateBorder();
	}
	layout();
	redraw();
}

void Titlebar::show()
{
	if(!shown())
		create();

	XMapWindow(fl_display, fl_xid(this));
	XRaiseWindow(fl_display, fl_xid(this));
}

void Titlebar::hide()
{
	if(shown())
		XUnmapWindow(fl_display, fl_xid(this));
}

#define set_box(b) if(box() != b) box(b)

void Titlebar::layout()
{
	if(Theme::use_theme())
	{
		if(box() != FL_FLAT_BOX) 
			box(FL_FLAT_BOX);
	}
	else
	{
		switch(Titlebar::box_type)
		{
			default:
			case 0: set_box(FL_FLAT_BOX); break;
			case 1: set_box(FL_HOR_SHADE_FLAT_BOX); break;
			case 2: set_box(FL_THIN_DOWN_BOX); break;
			case 3: set_box(FL_UP_BOX); break;
			case 4: set_box(FL_DOWN_BOX); break;
			case 5: set_box(FL_PLASTIC_UP_BOX); break;
		}
	}

	int W = w()-box()->dx();

	int lsize = h()/2+2;
	label_size(lsize);

	// Try to detect what buttons are showed
	if(!curr_frame->func_flag(MINIMIZE)) 
		minb.deactivate();
	else
		minb.activate();

	if(!curr_frame->func_flag(MAXIMIZE))
		maxb.deactivate();
	else
		maxb.activate();

	// TODO: stinky
	if(!curr_frame->decor_flag(MINIMIZE) || curr_frame->frame_flag(KEEP_ASPECT))
		minb.hide();

	// TODO: stinky
	if(!curr_frame->decor_flag(MAXIMIZE) || curr_frame->frame_flag(KEEP_ASPECT))
		maxb.hide();

	if(curr_frame->size_hints->min_width == curr_frame->size_hints->max_width || 
		curr_frame->transient_for_xid)
	{
		minb.hide();
		maxb.hide();
	}
	else
	{
		minb.show();
		maxb.show();
	}

	int offset = 0;
	int s = h();
	int mid = 0;

	// for themes, buttons are little bit bigger
	if(!Theme::use_theme())
	{
		s -= 4;
		mid = 2;
		offset = 2;
	}

	int bx = W - s - offset;

	closeb.resize(bx, mid, s, s);
	if(closeb.visible())
		bx -= s + offset;

	maxb.resize(bx, mid, s, s);
	if(maxb.visible())
		bx -= s + offset;

	minb.resize(bx, mid, s, s);

	text_w = bx - (curr_frame->decor_flag(SYSMENU) ? h() : 0) - 10;
	fl_font(label_font(), label_size());
	if(!curr_frame->label().empty())
	{
		// TODO: label is not very well cutted when window width
		// is too small
		Fl_Widget::label(fl_cut_line(curr_frame->label(), text_w));
		if(strcmp(label(), curr_frame->label()))
			tooltip(curr_frame->label());
		else
			tooltip("");
	}
	else
	{
		label("");
		tooltip("");
	}

	// Reset layout flags
	Fl_Widget::layout();
}

void Titlebar::draw()
{
	DBG("Titlebar::draw(): %s", label().c_str());
	if(Theme::use_theme() && Theme::image(TITLEBAR_BG))
		Theme::image(TITLEBAR_BG)->draw(0, 0, w(), h(), FL_ALIGN_SCALE);
	else
		draw_box();

	int s=h()-4;

	// Resize and set image & mask
	if(curr_frame->icon() && curr_frame->decor_flag(SYSMENU))
		title_icon = curr_frame->icon()->get_icon(s, s);
	else
		title_icon = 0;

	int tx = box()->dx()+1;

	if(title_icon)
	{
		title_icon->draw(2,2, s, s);
		tx += s + 5;	// Separate text a few pixels from icon
	}

	// FIXME:
	// When window does not containt max or min or close buttons
	// we should not draw them. Also this will enable correct title
	// text position when is choosed FL_ALIGN_RIGHT
	draw_label(tx, 0, text_w, h(), Titlebar::label_align);

	draw_child(closeb);
	draw_child(maxb);
	draw_child(minb);
}

void Titlebar::popup_menu(Frame* frm)
{
	// must use it here, since it will
	// be initialized for other files where is
	// used
	menu_frame = curr_frame;

	if(curr_frame->maximized)
		menu_max->label(_("Restore"));
	else
		menu_max->label(_("Maximize"));

	// we don't want animation for dialogs and utils frames
	// MWM hints can set MAXIMIZE and MINIMIZE options separately
	// so we must check them each
	if(curr_frame->window_type() == TYPE_NORMAL)
	{
		if(curr_frame->decor_flag(MAXIMIZE))
		{
			menu_max->activate();
			menu_set_size->activate();
		}
		if(curr_frame->decor_flag(MINIMIZE))
			menu_min->activate();
	}
	else
	{
		menu_max->deactivate();
		menu_set_size->deactivate();
		menu_min->deactivate();
	}

	title_menu->Fl_Group::focus(-1);
	title_menu->popup(Fl::event_x_root(), Fl::event_y_root());

	menu_frame = 0;
}

// handle double click on titlebar area
void Titlebar::handle_double_click()
{
	button_cb_max(0, curr_frame);
}

// do what should be done when window is 
// maximized or restored
void Titlebar::handle_maximize(bool is_max)
{
	if(is_max)
		maxb.label("@mz");
	else
		maxb.label("@mx");
}

int Titlebar::handle(int event)
{
	static bool grabbed = false;
	static int dx, dy, nx, ny;
	int mx, my;

	// Uh, oh... details are in Frame.cpp
	extern bool handle_title;

	switch(event)
	{
		case FL_PUSH:
		{
			if(grabbed) return 1;
			dx = Fl::event_x_root() - curr_frame->x();
			dy = Fl::event_y_root() - curr_frame->y();
			nx = Fl::event_x_root() - dx;
			ny = Fl::event_x_root() - ny;

			// TODO: needs revision
			// Send events to buttons...
			for(int i = children(); i--;)
			{
				Fl_Widget* o = child(i);
				if(!o) break;
				mx = Fl::event_x() - o->x() - 2;
				my = Fl::event_y() - o->y() - 2;
				if(mx >= 0 && my < o->w() && my >= 0 && mx < o->h() + 4)
				{
					if(o->send(event))
					{
						handle_title = false;
						return 1;
					}
				}
			}

			// allow double-click do maximize only normal windows
			if(Fl::event_clicks() && Fl::event_button() == 1)
			{
				if(curr_frame->window_type() == TYPE_NORMAL && curr_frame->decor_flag(MAXIMIZE))
					handle_double_click();

				Fl::event_clicks(0);
				return 1;
			}
			else if(Fl::event_button() == 3)
			{
				popup_menu(curr_frame);
				return 1;
			}
			
			return 1;
		}

		case FL_RELEASE:
		{
			if(Fl::event_state(FL_BUTTON1)) return 1;
			if(grabbed)
			{
				if(!Frame::do_opaque)
				{
					clear_overlay();
					curr_frame->set_size(nx, ny, curr_frame->w(), curr_frame->h());
					
					// NOTE: grabbed in FL_DRAG
					XUngrabServer(fl_display);
				}
				grab_cursor(false);
				grabbed = false;
			}

			if(root->get_cursor() == FL_CURSOR_MOVE)
				root->set_default_cursor();
			if(Fl::event_is_click())
			{
				curr_frame->activate();
				curr_frame->raise();
			}

			return 1;
		}

		case FL_DRAG:
		{
			if(Fl::event_is_click()) return 1; // don't drag yet
			if(!Fl::event_state(FL_BUTTON1)) return 0;

			// Change to MOVE cursor
			if(root->get_cursor() != FL_CURSOR_MOVE)
				root->set_cursor(FL_CURSOR_MOVE, FL_WHITE, FL_BLACK);

			// We need to grab server while moving,
			// since if underlying window redraws our overlay will fuck up...
			if(!grabbed) 
			{
				if(!Frame::do_opaque) XGrabServer(fl_display);
				grab_cursor(true);
				grabbed = true;
			}

			nx = Fl::event_x_root()-dx;
			ny = Fl::event_y_root()-dy;
			curr_frame->handle_move(nx, ny);

			return 1;
		}
	}

	return 0;
}
