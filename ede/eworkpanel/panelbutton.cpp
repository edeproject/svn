// Copyright (c) 2000. - 2005. EDE Authors
// This program is licenced under terms of the
// GNU General Public Licence version 2 or newer.
// See COPYING for details.

#include <stdio.h>
#include <efltk/Fl.h>
#include <efltk/Fl_Menu_Button.h>
#include <efltk/fl_draw.h>
#include <efltk/Fl_Group.h>
#include <efltk/Fl_Item.h>

#include "panelbutton.h"

extern Fl_Widget* fl_did_clipping;

// PanelMenu
PanelMenu::PanelMenu(int x, int y, int w, int h, Fl_Boxtype up_c, Fl_Boxtype down_c, const char *label)
    : Fl_Menu_Button(x, y, w, h, label)
{
    m_open = false;
    Height = 0;
    up = up_c;
    down = down_c;

    anim_speed(2);
    anim_flags(BOTTOM_TO_TOP);
    accept_focus(false);
}

void PanelMenu::draw()
{
    Fl_Boxtype box = up;
    Fl_Flags flags;
    Fl_Color color;

    if (!active_r()) {
        flags = FL_INACTIVE;
        color = this->color();
    } else if (belowmouse()) {
        flags = FL_HIGHLIGHT;
        color = highlight_color();
        if (!color) color = this->color();
    } else if (m_open) {
        flags = FL_HIGHLIGHT;
        color = highlight_color();
        if (!color) color = this->color();
        box = down;
    } else {
        flags = 0;
        color = this->color();
    }

    if(!box->fills_rectangle()) {
        fl_push_clip(0, 0, this->w(), this->h());
        parent()->draw_group_box();
        fl_pop_clip();
    }

    box->draw(0, 0, this->w(), this->h(), color, flags);

    int x,y,w,h;
    x = y = 0;
    w = this->w(); h = this->h();
    box->inset(x,y,w,h);
    draw_inside_label(x,y,w,h,flags);
}

void PanelMenu::calculate_height()
{
    Fl_Style *s = Fl_Style::find("Menu");
    Height = s->box->dh();
    for(int n=0; n<children(); n++)
    {
        Fl_Widget *i = child(n);
        if(!i) break;
        if(!i->visible()) continue;
        fl_font(i->label_font(), i->label_size());
        Height += i->height()+s->leading;
    }
}

int PanelMenu::popup()
{
    m_open = true;
    calculate_height();
    int retval = Fl_Menu_::popup(0, 0-Height);//, w(), h());
    m_open = false;
    redraw();
    return retval;
}


// PanelButton - as opposed to PanelMenu - by Vedran

PanelButton::PanelButton(int x, int y, int w, int h, Fl_Boxtype up_c, Fl_Boxtype down_c, const char *label)
    : Fl_Button(x, y, w, h, label)
{
//    Height = 0;
    up = up_c;
    down = down_c;
    accept_focus(false);
}

void PanelButton::draw()
{
    Fl_Boxtype box = up;
    Fl_Flags flags;
    Fl_Color color;

    if (belowmouse())
    {
        flags = FL_HIGHLIGHT;
        color = highlight_color();
        if (!color) color = this->color();
//        box = down;
    } else {
        flags = 0;
        color = this->color();
    }

    if(!box->fills_rectangle()) {
        fl_push_clip(0, 0, this->w(), this->h());
        parent()->draw_group_box();
        fl_pop_clip();
    }

    box->draw(0, 0, this->w(), this->h(), color, flags);

    int x,y,w,h;
    x = y = 0;
    w = this->w(); h = this->h();
    box->inset(x,y,w,h);
    draw_inside_label(x,y,w,h,flags);
}
