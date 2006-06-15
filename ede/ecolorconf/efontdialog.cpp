/*
 * $Id$
 *
 * EControl applet for colors and fonts
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

/*
 * Based on:
 * Font demo program for the Fast Light Tool Kit (FLTK).
 * Copyright 1998-1999 by Bill Spitzak and others.
 */

#include <efltk/Fl.h>
#include <efltk/Fl_Double_Window.h>
#include <efltk/Fl_Browser.h>
#include <efltk/Fl_String.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Check_Button.h>
#include <efltk/fl_ask.h>
#include <efltk/fl_draw.h>
#include <efltk/Fl_Box.h>
#include <efltk/fl_utf8.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ecolorutils.h"

#define MAXSIZE 64

Fl_Window *form;


// class for font preview window

class FontDisplay : public Fl_Widget {
  void draw();
public:
    Fl_Font font;
    unsigned size;
    const char* encoding;
    FontDisplay(Fl_Boxtype B, int X, int Y, int W, int H, const char* L = 0) :
        Fl_Widget(X,Y,W,H,L) {box(B); font = 0; size = 14;}
};

void FontDisplay::draw() {
  draw_box();
  int ix = 0, iy = 0, iw = w(), ih = h();
  box()->inset(ix, iy, iw, ih);
  fl_push_clip(ix, iy, iw, ih);
  const char* saved_encoding = fl_encoding();
  fl_encoding(encoding);
  fl_font(font, size);
  fl_color(FL_BLACK);
  char buffer[32];
//#if HAVE_XUTF8
  char outbuf[64];
//#endif
  for (int Y = 1; Y < 8; Y++) {
      for (int X = 0; X < 32; X++) buffer[X] = (32*Y+X);
//#if HAVE_XUTF8
      int len = fl_latin12utf((uint8*)buffer, 32, outbuf);
      fl_draw(outbuf, len, 3, 3+fl_height()*Y);
//#else
//      fl_draw(buffer, 32, 3, 3+fl_height()*Y);
//#endif
  }
  fl_encoding(saved_encoding);
  fl_pop_clip();
}



// other variables

FontDisplay *textobj;

Fl_Browser *fontobj, *sizeobj, *encobj;

Fl_Font* all_fonts; // list returned by fltk

Fl_Group *button_group;
Fl_Check_Button* bold_button, *italic_button;
Fl_Box* id_box;
Fl_Button *ok_button, *cancel_button;

bool return_value = false;



// callback functions


// callback for list of fonts

void font_cb(Fl_Widget *, long) 
{
	int fn = fontobj->value();
	Fl_Font f = all_fonts[fn];

	// are bold and italic available?
	if (f->bold() == f) 
		bold_button->deactivate();
	else 
		bold_button->activate();
	if (f->italic() == f) 
		italic_button->deactivate();
	else 
		italic_button->activate();
	if (bold_button->value()) f = f->bold();
	if (italic_button->value()) f = f->italic();

	textobj->font = f;

	// Populate the encobj (browser for font encodings)
	char saved[30];
	strncpy(saved, textobj->encoding, 29);
	encobj->clear();

	const char** encodings;
	int ne = f->encodings(encodings);
	int picked = -1;
	int iso8859 = 0;
	for (int i = 0; i < ne; i++) {
		encobj->add(encodings[i]);
		if (!strcmp(encodings[i], saved)) picked = i;
		if (!strcmp(encodings[i], fl_encoding())) iso8859 = i;
	}
	if (picked < 0) picked = iso8859;
	textobj->encoding = encodings[picked];
	encobj->value(picked);

	// Populate the sizeobj (browser for font sizes)
	int pickedsize;
	if (sizeobj->value() > 0) {
		pickedsize = atoi(sizeobj->text(sizeobj->value()));
	} else {
		pickedsize = 14;
	}
	sizeobj->clear();

	int *s;
	int n = f->sizes(s);
	if(!n) {
		// no sizes (this only happens on X)
		for (int i = 1; i<MAXSIZE; i++) {
			char buf[20];
			sprintf(buf,"%d",i);
			sizeobj->add(buf);
		}
		sizeobj->value(pickedsize-1);
		textobj->size = pickedsize;
		
		// fl_font(f, pickedsize); lets fix this...
	} else if (s[0] == 0) {
		// many sizes;
		int j = 1;
		for (int i = 1; i<MAXSIZE || i<s[n-1]; i++) {
			char buf[20];
			sprintf(buf,"%d",i);
			Fl_Widget *w = sizeobj->add(buf);
			if (j < n && i==s[j]) {
				w->label_font(w->label_font()->bold());
				w->label_color(FL_RED);
				j++;
			}
		}
		sizeobj->value(pickedsize-1);
		textobj->size = pickedsize;
	} else {
		// some sizes -- when is this used?
		int w = 0;
		for (int i = 0; i < n; i++) {
			if (s[i]<=pickedsize) w = i;
			char buf[20];
			sprintf(buf,"%d",s[i]);
			Fl_Widget *w = sizeobj->add(buf);
			w->label_font(w->label_font()->bold());
		}
		sizeobj->value(w);
		textobj->size = s[w];
	}

	encobj->redraw();
	sizeobj->redraw();
	textobj->redraw();
	encobj->relayout();
	sizeobj->relayout();
	textobj->relayout();

//  id_box->label(textobj->font->system_name());
//  id_box->redraw();
	button_group->redraw();	// needed?
}

void encoding_cb(Fl_Widget *, long) {
  int i = encobj->value();
  textobj->encoding = encobj->text(i);
  textobj->redraw();
  id_box->redraw();
}

void size_cb(Fl_Widget *, long) {
  int i = sizeobj->value();
  const char *c = sizeobj->text(i);
  while (*c < '0' || *c > '9') c++;
  textobj->size = atoi(c);
  textobj->redraw();
  id_box->redraw();
}

void return_cb(Fl_Widget *, long ret) {
    return_value = ret;
    form->hide();
}


// TODO: rewrite this in eflud...
void create_the_forms() 
{
  form = new Fl_Window(550, 420, _("Select font..."));
  
  textobj = new FontDisplay(FL_ENGRAVED_BOX,10,10,530,160);
  textobj->clear_flag(FL_ALIGN_MASK);
  textobj->set_flag(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_CLIP);
  id_box = new Fl_Box(10, 172, 530, 15);
  id_box->box(FL_ENGRAVED_BOX);
  id_box->label_size(10);
  id_box->set_flag(FL_ALIGN_INSIDE|FL_ALIGN_CLIP);
  button_group = new Fl_Group(10, 190, 140, 20);
  bold_button = new Fl_Check_Button(0, 0, 70, 20, "Bold");
  bold_button->callback(font_cb, 1);
  italic_button = new Fl_Check_Button(70, 0, 70, 20, "Italic");
  italic_button->callback(font_cb, 1);
  button_group->end();
  fontobj = new Fl_Browser(10, 210, 280, 170);
  fontobj->when(FL_WHEN_CHANGED);
  fontobj->callback(font_cb);
  form->resizable(fontobj);
  encobj = new Fl_Browser(300, 210, 100, 170);
  encobj->when(FL_WHEN_CHANGED);
  encobj->callback(encoding_cb, 1);
  sizeobj = new Fl_Browser(410, 210, 130, 170);
  sizeobj->when(FL_WHEN_CHANGED);
  sizeobj->callback(size_cb);
  
  ok_button = new Fl_Button(380, 390, 80, 25, _("&OK"));
  ok_button->callback(return_cb, 1);

  cancel_button = new Fl_Button(465, 390, 80, 25, _("&Cancel"));
  cancel_button->callback(return_cb, 0);
  
  form->end();
}


// search for largest <= selected size:
int find_best_size(Fl_Font font, int selected)
{
	int *allsizes;
	int numsizes = font->sizes(allsizes);

//	This is a bug in efltk
	if (numsizes <= 1) return selected;

	for (int i=1; i<numsizes; i++) {
		if (allsizes[i] > selected)
			return allsizes[i-1];
	}

	return allsizes[numsizes-1];
}


EDEFont fl_font_dialog(EDEFont current_font) 
{
	EDEFont return_font;

	create_the_forms();
	int numfonts = fl_list_fonts(all_fonts);

	// populate list of fonts
	Fl_String currentname = current_font.font->name();
	for(int i = 0; i < numfonts; i++) {
		Fl_String fontname = all_fonts[i]->name();
		fontobj->add(fontname);
 		if (currentname.lower_case().pos(fontname.lower_case())==0) // it's a substring
			fontobj->value(i);
	}

	// set bold, italic
	if (currentname.pos(" bold italic") == currentname.length()-12) {
		bold_button->value(true);
		italic_button->value(true);
	} else if (currentname.pos(" italic") == currentname.length()-7) {
		italic_button->value(true);
	} else if (currentname.pos(" bold") == currentname.length()-5) {
		bold_button->value(true);
	}

	// populate other lists
	textobj->encoding = current_font.encoding; // TODO: what if we're using XFT?
	font_cb(fontobj,0);
	for (int i=0; i < sizeobj->children(); i++) {
		if (atoi(sizeobj->text(i)) == current_font.size) {
			sizeobj->value(i);
			size_cb(sizeobj,0);
		}
	}

	form->exec();
	form->show();

	// we have to construct a proper EDEFont to return
	return_font.defined = false;
	if (return_value)
	{
		return_font.font = fl_find_font(fontobj->text(fontobj->value()));
		if (bold_button->value()) return_font.font = return_font.font->bold();
		if (italic_button->value()) return_font.font = return_font.font->italic();

		int size = atoi(sizeobj->text(sizeobj->value()));
		return_font.size = find_best_size(return_font.font, size);

		// on XFT encoding is always Unicode, so this field can be blank
		if (encobj->children() > 0) 
			return_font.encoding = encobj->text(encobj->value());
		else
			return_font.encoding = "";
		
		return_font.defined = true;
	}
	return return_font;
}

//
// End of "$Id$".
//
