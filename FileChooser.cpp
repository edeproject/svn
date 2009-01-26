#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_File_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_File_Browser.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>

#include <edelib/TableBase.h>
#include <edelib/Nls.h>

#include <stdio.h>

#include "FileBrowser.h"

extern int FL_NORMAL_SIZE;

EDELIB_NS_USE

static void list_cell_cb(Fl_Widget*, void*);

static Fl_Menu_Item chooser_menu[] = {
	{" Detail view", 0, 0},
	{" Simple view", 0, 0, 0, FL_MENU_DIVIDER},
	{" Show bookmarks", 0, 0, 0, FL_MENU_TOGGLE},
	{0}
};

class ChooserMenuButton : public Fl_Menu_Button {
	public:
		ChooserMenuButton(int X, int Y, int W, int H, const char *l = 0) :
		Fl_Menu_Button(X, Y, W, H, l) { }
		~ChooserMenuButton() { }
		void draw(void);
};

void ChooserMenuButton::draw(void) {
	Fl_Menu_Button::draw();
}

class Bookmarks : public TableBase { 
	public:
		Bookmarks(int X, int Y, int W, int H, const char *l = 0) : TableBase(X, Y, W, H, l) {
			box(FL_DOWN_FRAME);
			color(FL_WHITE);
			col_header(1);
			col_header_height(20);
			cols(1);

			// initial width
			int boxw = Fl::box_dw(box());
			col_width(0, W - boxw);
		}

		~Bookmarks() { }
		void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H);
		void resize(int X, int Y, int W, int H);
};

void Bookmarks::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) {
	switch(context) {
		case CONTEXT_ROW_HEADER:
		case CONTEXT_RC_RESIZE:
			break;

		// draw column header
		case CONTEXT_COL_HEADER:
			fl_font(FL_HELVETICA, FL_NORMAL_SIZE);
			fl_push_clip(X, Y, W, H);

			fl_draw_box(FL_UP_BOX, X, Y, W, H, row_header_color());
			fl_color(FL_BLACK);
			fl_draw("Places", X + 5, Y, W, H, FL_ALIGN_LEFT);
			fl_pop_clip();
			break;
		// draw cells
		case CONTEXT_CELL:
			// background
			fl_push_clip(X, Y, W, H);
			fl_draw_box(FL_FLAT_BOX, X, Y, W, H, FL_WHITE);
			fl_pop_clip();

			// text
			fl_font(FL_HELVETICA, 12);
			fl_push_clip(X + 5, Y + 3, W - 6, H - 6);
			fl_color(FL_BLACK);
			fl_draw("xxx", X + 5, Y + 3, W - 6, H - 6, FL_ALIGN_LEFT);
			fl_pop_clip();
			break;
		default:
			break;
	}
}

void Bookmarks::resize(int X, int Y, int W, int H) {
	TableBase::resize(X, Y, W, H);

	int boxw = Fl::box_dw(box());

	if(vscrollbar->visible())
		col_width(0, W - vscrollbar->w() - boxw);
	else
		col_width(0, W - boxw);
}

int main(int argc, char **argv) {
	Fl_Window *win = new Fl_Window(480, 350);
	//Fl_Window *win = new Fl_Window(670, 350);
	win->begin();
		Fl_Button *b1 = new Fl_Button(10, 10, 30, 25);
		b1->tooltip(_("Add to bookmarks"));
		//b1->image(image_list1);

		Fl_Button *b2 = new Fl_Button(45, 10, 30, 25);
		b2->tooltip(_("Remove from bookmarks"));
		//b2->image(image_list);

		Fl_Button *b3 = new Fl_Button(365, 10, 30, 25);
		b3->tooltip(_("Create directory"));
		//b3->image(image_folder);

		Fl_Button *b4 = new Fl_Button(400, 10, 30, 25);
		b4->tooltip(_("Refresh"));
		//b4->image(image_view);

		ChooserMenuButton *b5 = new ChooserMenuButton(440, 10, 30, 25);
		b5->menu(chooser_menu);
	
		Fl_Tile *tile_group = new Fl_Tile(10, 40, 460, 200);
		tile_group->begin();
			Bookmarks *bb = new Bookmarks(10, 40, 145, 200);
			bb->end();

			//new Fl_Browser(10, 40, 145, 200);
			//new Fl_File_Browser(155, 40, 315, 200);

			//ChooserListView *lv = new ChooserListView(155, 40, 315, 200);
			FileBrowser *lv = new FileBrowser(155, 40, 315, 200);
			lv->box(FL_DOWN_FRAME);
			lv->color(FL_WHITE);
			lv->col_header(1);
			lv->col_header_height(20);
			lv->col_resize(1);
			lv->col_resize_min(50);
			lv->cols(4);
			lv->load(".");
			lv->col_width_all(100);
			lv->end();
			//lv->rows(20);
			//lv->resize_headers();

		tile_group->end();

		Fl_Group *g1 = new Fl_Group(10, 240, 460, 105);
		g1->begin();
			Fl_File_Input *loc = new Fl_File_Input(110, 246, 260, 35, _("Location:"));
			Fl_Group::current()->resizable(loc);

			Fl_Choice *fi = new Fl_Choice(110, 285, 260, 25, _("Filter:"));
			fi->down_box(FL_BORDER_BOX);

			Fl_Check_Button *cbtn = new Fl_Check_Button(110, 315, 260, 25, _("Preview"));
			cbtn->down_box(FL_DOWN_BOX);

			new Fl_Button(380, 255, 90, 25, _("&Open"));
			new Fl_Button(380, 285, 90, 25, _("&Cancel"));
		g1->end();


		//Fl_Box *preview_box = new Fl_Box(475, 40, 185, 200);
		//preview_box->box(FL_DOWN_BOX);
		//preview_box->color(FL_WHITE);

		/* invisible resizable box */
		Fl_Box* ibox = new Fl_Box(175, 142, 170, 81);
		Fl_Group::current()->resizable(ibox);
	win->end();
	win->show(argc, argv);
	return Fl::run();
}
