#include "icondialog.h"
#include "eiconman.h"

#include <assert.h>

#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_File_Dialog.h>
#include <efltk/Fl_Locale.h>

void cmd_input_cb(Fl_Widget*, void* i)
{
	Fl_Input* in = (Fl_Input*)i;
	char *file_types = _("Executables (*.*), *, All files (*.*), *");
	const char *f = fl_select_file(in->value(), file_types, _("Open location..."));
	if(f)
		in->value(f);
}

IconDialog::IconDialog(IconSettings* isett, Fl_Image* img) : Fl_Window(310, 290, _("Icon settings")), settings(isett)
{
	assert(settings != NULL);

	Fl_Tabs* tt = new Fl_Tabs(5, 5, 300, 249);

	Fl_Group* gr1 = new Fl_Group(1, 23, 298, 225, _("Settings"));
		img_box = new Fl_Box(125, 10, 60, 60);
		if(img)
			img_box->image(img);

		name = new Fl_Input(9, 88, 250, 25, _("Name:"));
		name->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
		name->value(isett->name.c_str());

		img_path = new Fl_Input(9, 133, 250, 25, _("Image:"));
		img_path->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
		img_path->value(isett->icon_path.c_str());

		img_path_button = new Fl_Button(264, 133, 25, 25, _("..."));
		img_path_button->label_type(FL_ENGRAVED_LABEL);

		cmd_path = new Fl_Input(9, 188, 250, 25, _("Program to run:"));
		cmd_path->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
		cmd_path->value(settings->cmd.c_str());

		cmd_path_button = new Fl_Button(264, 188, 25, 25, _("..."));
		cmd_path_button->label_type(FL_ENGRAVED_LABEL);

	gr1->end();

	Fl_Group* gr2 = new Fl_Group(1, 23, 298, 225, _("Terminal"));
	gr2->hide();
		new Fl_Check_Button(9, 18, 250, 25, _("Run in terminal"));
		term_path = new Fl_Input(9, 68, 250, 25, _("Terminal program:"));
		term_path->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
		term_path_button = new Fl_Button(264, 68, 26, 25, _("..."));
		term_path_button->label_type(FL_ENGRAVED_LABEL);
	gr2->end();
	tt->end();

	apply = new Fl_Button(140, 260, 80, 25, _("&Apply"));
	close = new Fl_Button(225, 260, 80, 25, _("&Close"));
	
	end();
}

IconDialog::~IconDialog()
{
}
