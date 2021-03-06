//
// "$Id$"
//
// Fl_Pack test program for the Fast Light Tool Kit (FLTK).
//
// Rather crude test of the Fl_Pack object.
// Changing the type() of an Fl_Pack after it is displayed is not supported
// so I have to do a lot of resizing of things before that.
//
// Copyright 1998-1999 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <efltk/Fl.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Light_Button.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Scroll.h>
#include <efltk/Fl_Value_Slider.h>
#include <efltk/Fl_Pack.h>

Fl_Pack *pack;
Fl_Scroll *scroll;

// Callback for when horizontal/vertical are changed:
void type_cb(Fl_Light_Button*, long v)
{
    pack->type(uchar(v));
    // Resize the widgets to small squares so the pack will resize them
    // to strips that are 25 wide:
    for (int i = 0; i < pack->children(); i++) {
        Fl_Widget* o = pack->child(i);
        o->resize(0,0,25,25);
    }
    // Resize the pack to fit into the scroll box:
    pack->resize(0, 0,
                 scroll->w()-scroll->scrollbar.w()-scroll->edge_offset()*2,
                 scroll->h()-scroll->hscrollbar.h()-scroll->edge_offset()*2
                );
}

// Callback for when the spacing is changed:
void spacing_cb(Fl_Value_Slider*o, long) {
    pack->spacing(int(o->value()));
    pack->relayout();
    pack->redraw();
}

int main(int argc, char **argv)
{
    Fl_Window *w;
    {
        Fl_Window* o = new Fl_Window(365, 525);
        w = o;
        scroll = new Fl_Scroll(10,10,345,285);
        w->resizable(scroll);

        {
            Fl_Pack* o = new Fl_Pack(10, 10, 345, 285);
            pack = o;
            o->box(FL_DOWN_BOX);
            //o->box(FL_ENGRAVED_FRAME);

            (void) new Fl_Button(35, 35, 25, 25, "b1");
            (void) new Fl_Button(45, 45, 25, 25, "b2");
            (void) new Fl_Button(55, 55, 25, 25, "b3");
            (void) new Fl_Button(65, 65, 25, 25, "b4");
            (void) new Fl_Button(75, 75, 25, 25, "b5");
            (void) new Fl_Button(85, 85, 25, 25, "b6");
            (void) new Fl_Button(95, 95, 25, 25, "b7");
            (void) new Fl_Button(105, 105, 25, 25, "b8");
            (void) new Fl_Button(115, 115, 25, 25, "b9");
            (void) new Fl_Button(125, 125, 25, 25, "b10");
            (void) new Fl_Button(135, 135, 25, 25, "b11");
            (void) new Fl_Button(145, 145, 25, 25, "b12");
            (void) new Fl_Button(155, 155, 25, 25, "b13");
            (void) new Fl_Button(165, 165, 25, 25, "b14");
            (void) new Fl_Button(175, 175, 25, 25, "b15");
            (void) new Fl_Button(185, 185, 25, 25, "b16");
            (void) new Fl_Button(195, 195, 25, 25, "b17");
            (void) new Fl_Button(205, 205, 25, 25, "b18");
            (void) new Fl_Button(215, 215, 25, 25, "b19");
            (void) new Fl_Button(225, 225, 25, 25, "b20");
            (void) new Fl_Button(235, 235, 25, 25, "b21");
            (void) new Fl_Button(245, 245, 25, 25, "b22");
            (void) new Fl_Button(255, 255, 25, 25, "b23");
            (void) new Fl_Button(265, 265, 25, 25, "b24");
            o->end();
        }
        scroll->end();

        {
            Fl_Light_Button* o = new Fl_Light_Button(10, 325, 175, 25, "HORIZONTAL");
            o->type(Fl_Button::RADIO);
            o->callback((Fl_Callback*)type_cb, (void*)(Fl_Pack::HORIZONTAL));
        }
        {
            Fl_Light_Button* o = new Fl_Light_Button(10, 350, 175, 25, "VERTICAL");
            o->type(Fl_Button::RADIO);
            o->value(1);
            o->callback((Fl_Callback*)type_cb, (void*)(Fl_Pack::VERTICAL));
        }
        {
            Fl_Value_Slider* o = new Fl_Value_Slider(50,375, 295,25,"spacing:");
            o->clear_flag(FL_ALIGN_MASK);
            o->set_flag(FL_ALIGN_LEFT);
            o->type(Fl_Slider::HORIZONTAL);
            o->range(0,30);
            o->step(1);
            o->callback((Fl_Callback*)spacing_cb);
        }
        w->end();
    }
    w->show(argc, argv);
    return Fl::run();
}

//
// End of "$Id$".
//
