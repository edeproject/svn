//
// "$Id$"
//
// Menu bar widget for the Fast Light Tool Kit (FLTK).
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
#include <efltk/Fl_Util.h>
#include <efltk/Fl_Menu_Bar.h>
#include <efltk/fl_draw.h>
#include <config.h>

static void revert_menubar(Fl_Style* s) {
    s->leading = 4;
    s->color = FL_GRAY;
    s->box = FL_FLAT_BOX;
#if 0
    // NT 4.0 style
    s->button_box = FL_FLAT_BOX;
#else
    // Windows98 style:
    s->button_box = FL_HIGHLIGHT_BOX;
#endif
}
static Fl_Named_Style style_menubar("Menu_Bar", revert_menubar, &Fl_Menu_Bar::default_style);
Fl_Named_Style* Fl_Menu_Bar::default_style = &::style_menubar;

Fl_Menu_Bar::Fl_Menu_Bar(int x,int y,int w,int h,const char *l)
    : Fl_Menu_(x,y,w,h,l)
{
    style(default_style);
    shortcut(FL_Alt_L);
    lines=1;

    level=0;

    highlight_ = last_highlight_ = -1;
    selected_ = last_selected_ = -1;

    first_menu=0;
    executed_=0;
    right_=0;
    do_layout=true;

    value(-1);
}

void Fl_Menu_Bar::draw()
{
    if(damage()&(~FL_DAMAGE_HIGHLIGHT))
        draw_box();

    int X=0, Y=0, W=w(), H=h();
    box()->inset(X,Y,W,H);
    fl_push_clip(X,Y,W,H);

    if(!children()) { last_highlight_ = last_selected_ = -1; return; }
    for (int i = 0; i < children(); i++)
    {
        Fl_Widget* widget = child(i);
        if (!widget->visible()) continue;

        if(( damage()&(~FL_DAMAGE_HIGHLIGHT) || selected_==i || last_selected_==i) && (selected_!=-1||last_selected_!=-1))
        {
            widget->clear_flag(FL_HIGHLIGHT);
            if(selected_==i)
                button_box()->draw(widget->x(), widget->y(), widget->w(), widget->h(), button_color(), FL_VALUE);
            else
                button_box()->draw(widget->x(), widget->y(), widget->w(), widget->h(), button_color(), widget->flags()&~FL_VALUE);

            fl_push_matrix();
            fl_translate(widget->x(), widget->y());
            widget->draw();
            fl_pop_matrix();
        }

        if((damage()&(~FL_DAMAGE_HIGHLIGHT) || highlight_ == i || last_highlight_ == i) && selected_==-1)
        {
            // If you change how the items are drawn, you probably need to
            // change MenuTitle::draw and the functions find_selected and
            // titlex in Fl_Menu.cxx.
            if(i == highlight_ && takesevents() && widget->active_r())
                widget->set_flag(FL_HIGHLIGHT);
            else
                widget->clear_flag(FL_HIGHLIGHT);
            widget->clear_flag(FL_SELECTED);

            button_box()->draw(widget->x(), widget->y(), widget->w(), widget->h(), button_color(), widget->flags()&~FL_VALUE);
            fl_push_matrix();
            fl_translate(widget->x(), widget->y());
            widget->draw();
            fl_pop_matrix();
        }
    }

    fl_pop_clip();

    last_highlight_ = highlight_;
    last_selected_ = selected_;
}

void Fl_Menu_Bar::layout()
{
    lines=1;
    if(do_layout) h(0);
    int X=box()->dx()+leading()/2;
    int Y=box()->dy()+leading()/2;
    int W = w()-box()->dw();
    int H = h()-box()->dh();

    for(int i = 0; i < children(); i++) {
        Fl_Widget *w = child(i);

        if(!w->visible()) continue;
        if(w==right_) continue;

        w->w(0); w->layout(); //Get original size
        w->w(w->w()+leading());
        w->h(w->h()+leading());

        if(w->w()+X > W && do_layout) {
            Y+=w->h()+leading()/2;
            X=box()->dx()+leading()/2;
            lines++;
        }

        if(Y+w->h() > H && do_layout) {
            h(Y+w->h()+leading());
        }

        w->position(X,Y);
        X += w->w();
    }

    if(right_) {
        Fl_Widget *w = right_;

        w->w(0); w->layout(); //Get original size
        w->w(w->w()+leading());
        w->h(w->h()+leading());

        if(w->w()+X > W && do_layout) {
            Y+=w->h()+leading()/2;
            X=box()->dx()+leading()/2;
            lines++;
        }

        if(Y+w->h() > H && do_layout) {
            h(Y+w->h()+leading());
        }

        if(lines==1)
            X=W-w->w();

        w->position(X,Y);
    }

    Fl_Widget::layout();
}

int Fl_Menu_Bar::handle(int event)
{
    switch(event) {
    case FL_PUSH: {
        popup(0,0,0,0);
        return 1;
    }

    case FL_LEAVE:
        highlight_ = -1;
        selected_ = -1;
        redraw(FL_DAMAGE_HIGHLIGHT);
        return 1;

    case FL_ENTER:
    case FL_MOVE: {
        int index = -1;
        for(int i = 0; i < children(); i++) {
            Fl_Widget *widget = child(i);
            if(!widget->visible()) continue;
            if(Fl::event_inside(widget->x(), widget->y(), widget->w(), widget->h())) {
                index = i;
                break;
            }
        }
        if(index!=last_highlight_) {
            highlight_ = index;
            redraw(FL_DAMAGE_HIGHLIGHT);
        }
        return 1;
    }
    case FL_SHORTCUT: {
        key_event = true;
        for(int i = 0; i < children(); i++) {
            Fl_Widget* w = child(i);
            if(w->is_group() && w->active() && w->test_shortcut()) {
                value(i);
                popup(0,0,0,0);
            }
        }
        if(handle_shortcut()) return 1;
        return 0;
    }
    case FL_KEYUP: {
        key_event = true;
        // In the future maybe any shortcut() will work, but for now
        // only the Alt key does. Setting the shortcut to zero will disable
        // the alt key shortcut.
        if (shortcut() != FL_Alt_L && shortcut() != FL_Alt_R) break;
        if (Fl::event_key() != FL_Alt_L && Fl::event_key() != FL_Alt_R) break;
        // checking for event_clicks insures that the keyup matches the
        // keydown that preceeded it, so Alt was pressed & released without
        // any intermediate values.  On X it is false if Alt is held down
        // for a long time, too:
        if (!Fl::event_is_click()) break;

        // okay we got the shortcut, find first menu and pop it up:
        for(int i = 0; i < children(); i++) {
            Fl_Widget* w = child(i);
            if(w->active()) {
                value(i);
                popup(0,0,0,0);
            }
        }
        break;
    }

    default:
        break;

    }

    return Fl_Menu_::handle(event);
}

//
// End of "$Id$".
//
