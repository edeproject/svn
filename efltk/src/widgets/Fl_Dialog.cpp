/***************************************************************************
                          Fl_Dialog.cpp  -  description
                             -------------------
    begin                : Tue Nov 12 2002
    copyright            : (C) 2002 by Alexey Parshin
    email                : alexeyp@m7.tts-sf.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "../core/fl_internal.h"

#include <efltk/Fl.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Input.h>
#include <efltk/Fl_Tabs.h>
#include <efltk/Fl_Pixmap.h>
#include <efltk/fl_ask.h>
#include <efltk/fl_draw.h>

#include <efltk/Fl_Dialog.h>

/* XPM */
static const char * cancel_xpm[] = {
    "20 20 67 1",
    "   c None",
    ".  c #F90D04",
    "+  c #F70C04",
    "@  c #F30C04",
    "#  c #F40C04",
    "$  c #F60C04",
    "%  c #E30C04",
    "&  c #F10C04",
    "*  c #DB0C04",
    "=  c #F80C04",
    "-  c #ED0C04",
    ";  c #B10B04",
    ">  c #4B0806",
    ",  c #130706",
    "'  c #E40C04",
    ")  c #BF0B04",
    "!  c #9E0A05",
    "~  c #BA0B04",
    "{  c #5E0905",
    "]  c #200706",
    "^  c #090706",
    "/  c #EE0C04",
    "(  c #DF0C04",
    "_  c #C00B04",
    ":  c #C20B04",
    "<  c #600905",
    "[  c #220706",
    "}  c #0C0706",
    "|  c #070707",
    "1  c #E00C04",
    "2  c #F00C04",
    "3  c #BB0B04",
    "4  c #E50C04",
    "5  c #F50C04",
    "6  c #BE0B04",
    "7  c #630905",
    "8  c #230706",
    "9  c #E10C04",
    "0  c #DA0C04",
    "a  c #8F0A05",
    "b  c #3A0806",
    "c  c #110706",
    "d  c #AD0B04",
    "e  c #610905",
    "f  c #2D0706",
    "g  c #EC0C04",
    "h  c #D90C04",
    "i  c #E20C04",
    "j  c #DD0C04",
    "k  c #AE0B04",
    "l  c #7D0905",
    "m  c #BD0B04",
    "n  c #AC0B04",
    "o  c #DE0C04",
    "p  c #B70B04",
    "q  c #940A05",
    "r  c #3B0806",
    "s  c #620905",
    "t  c #B80B04",
    "u  c #DC0C04",
    "v  c #AF0B04",
    "w  c #5A0905",
    "x  c #860A05",
    "y  c #3C0806",
    "z  c #120706",
    "A  c #3D0806",
    "B  c #1A0706",
    "                    ",
    "                    ",
    "              .+@   ",
    "   .+#       .+$%   ",
    "   +$&*     .+=-;>, ",
    "   #&@')!  .+=-~{]^ ",
    "    *'/(_:-+=-~<[}| ",
    "     _(/'12+-3<[}|  ",
    "     !_4@#5-678}|   ",
    "      :95=#0abc|    ",
    "      -25#@'def     ",
    "     .++ghi-jkl     ",
    "    .+=-manj-opq    ",
    "   .+=-~7rsdo/jn    ",
    "  .+=-~<8cfltu9vw   ",
    "  .$-~<[}|  qnvxyz  ",
    "  .%;{[}|     wAB^  ",
    "    >]}|       z^|  ",
    "    ,^|             ",
    "                    "};
/* XPM */
static const char * help_xpm[] = {
    "20 20 5 1",
    "   c None",
    ".  c #BEB8ED",
    "+  c #786BE5",
    "@  c #000080",
    "#  c #261E66",
    "                    ",
    "                    ",
    "        .....       ",
    "      ..+++++@      ",
    "     .++++++++@     ",
    "    .+++@@@+++@#    ",
    "    .++@   .++@#    ",
    "     @@   .+++@#    ",
    "         .+++@##    ",
    "        .+++@##     ",
    "       .+++@##      ",
    "       .++@##       ",
    "       .++@#        ",
    "        @@##        ",
    "         ##         ",
    "        ..          ",
    "       .++@         ",
    "        @@##        ",
    "         ##         ",
    "                    "};
/* XPM */
static const char * no_xpm[] = {
    "20 20 5 1",
    "   c None",
    ".  c #FF0000",
    "+  c #800000",
    "@  c #542929",
    "#  c #808080",
    "                    ",
    "                    ",
    "       ......       ",
    "     ..++++++..     ",
    "    .+++@@@@+++@    ",
    "   .+++@    #+++@   ",
    "   .++++.     .+@   ",
    "  .++@+++.    .++@  ",
    "  .+@ #+++.    .+@  ",
    "  .+@  #+++.   .+@  ",
    "  .+@   #+++.  .+@  ",
    "  .+@    #+++. .+@  ",
    "  .++#    #+++.++@  ",
    "   .++     #++++@   ",
    "   .++..    ++++@   ",
    "    @+++....+++@    ",
    "     @@++++++@@     ",
    "       @@@@@@       ",
    "                    ",
    "                    "};
/* XPM */
static const char * ok_xpm[] = {
    "20 20 7 1",
    "   c None",
    ".  c #ADF4AD",
    "+  c #FFFF00",
    "@  c #00FF00",
    "#  c #156303",
    "$  c #ACF298",
    "%  c #5AEF34",
    "                    ",
    "                    ",
    "                ... ",
    "               .+@# ",
    "              .+@@# ",
    "             .+@@@# ",
    "            .+@@@## ",
    "           .+@@@##  ",
    "          .+@@@##   ",
    "  $.     .+@@@##    ",
    " $%+.   .+@@@##     ",
    " #%@+. .+@@@##      ",
    "  #@@+.+@@@##       ",
    "   #@@+@@@##        ",
    "    #@@@@##         ",
    "     #@@##          ",
    "      ###           ",
    "                    ",
    "                    ",
    "                    "};
/* XPM */
static const char * refresh_xpm[] = {
    "20 20 10 1",
    "   c None",
    ".  c #262615",
    "+  c #000000",
    "@  c #27591D",
    "#  c #379324",
    "$  c #33DB15",
    "%  c #141309",
    "&  c #79EA62",
    "*  c #161010",
    "=  c #111108",
    "                    ",
    "                    ",
    "        .+++++      ",
    "       .@+###++     ",
    "      +@@@+$$$+     ",
    "      +@@@@+$$$+    ",
    "     +@@+%%%$$$++++ ",
    "     +@@@%$$$$$$&%  ",
    "     +@@@@+$$$$&%   ",
    "     +@@@@+%$$&%    ",
    "     +@@@@+ *&%     ",
    "     +@@@@+  *      ",
    "     +@@@@++++++    ",
    "     +@@@=$$$$$+    ",
    "      +@=+$$$$+     ",
    "      +=+####+      ",
    "       =+++++       ",
    "                    ",
    "                    ",
    "                    "};
/* XPM */
static const char * retry_xpm[] = {
    "20 20 10 1",
    "   c None",
    ".  c #262615",
    "+  c #000000",
    "@  c #494912",
    "#  c #808000",
    "$  c #FFFF00",
    "%  c #141309",
    "&  c #CCCC0E",
    "*  c #161010",
    "=  c #111108",
    "                    ",
    "                    ",
    "        .+++++      ",
    "       .@+###++     ",
    "      +@@@+$$$+     ",
    "      +@@@@+$$$+    ",
    "     +@@+%%%$$$++++ ",
    "     +@@@%$$$$$$&%  ",
    "     +@@@@+$$$$&%   ",
    "     +@@@@+%$$&%    ",
    "     +@@@@+ *&%     ",
    "     +@@@@+  *      ",
    "     +@@@@++++++    ",
    "     +@@@=$$$$$+    ",
    "      +@=+$$$$+     ",
    "      +=+&&&&+      ",
    "       =+++++       ",
    "                    ",
    "                    ",
    "                    "};

// THESE ARE REPLACED BY Fl_Stock_Button?
struct Fl_Dialog_Button_Template {
    int         id;
    const char *label;
    Fl_Pixmap  *pixmap;
};

Fl_Pixmap   pixmap_ok(ok_xpm),
pixmap_cancel(cancel_xpm),
pixmap_no(no_xpm),
pixmap_retry(retry_xpm),
pixmap_refresh(refresh_xpm),
pixmap_help(help_xpm);

// THESE ARE REPLACED BY Fl_Stock_Button?
static const Fl_Dialog_Button_Template buttonTemplates[] = {
    { Fl_Dialog::BTN_OK,     N_("Ok"),     &pixmap_ok },
    { Fl_Dialog::BTN_CANCEL, N_("Cancel"), &pixmap_cancel },
    { Fl_Dialog::BTN_YES,    N_("Yes"),    &pixmap_ok },
    { Fl_Dialog::BTN_NO,     N_("No"),     &pixmap_no },
    { Fl_Dialog::BTN_RETRY,  N_("Retry"),  &pixmap_retry },
    { Fl_Dialog::BTN_REFRESH,N_("Refresh"),&pixmap_refresh },
    { Fl_Dialog::BTN_CONFIRM,N_("Confirm"),&pixmap_ok },
    { Fl_Dialog::BTN_IGNORE, N_("Ignore"), &pixmap_no },
    { Fl_Dialog::BTN_HELP,   N_("Help"),   &pixmap_help },
    { 0,             "",       &pixmap_help }
};

void Fl_Dialog::escape_callback(Fl_Dialog *dialog, void *) {
    if (dialog->m_buttons & Fl_Dialog::BTN_CANCEL) {
        Fl::exit_modal();
        dialog->m_modalResult = Fl_Dialog::BTN_CANCEL;
    }
}

void Fl_Dialog::buttons_callback(Fl_Button *btn, long id)
{
    // HELP doesn't close window
    if(id==BTN_HELP) return;

    Fl::exit_modal();

    Fl_Dialog *dialog = (Fl_Dialog *)btn->window();
    dialog->m_modalResult = (int)id;
}

Fl_Dialog::Fl_Dialog(int ww, int hh, const char *label, Fl_Data_Source *ds) 
: Fl_Window(ww,hh,label) 
{
    m_defaultButton = 0;

    m_buttonPanel = new Fl_Group(0,0,10,10);
    Fl_Box *resize = new Fl_Box(0,-1,10,1);
    resize->hide();
    m_buttonPanel->resizable(resize);
    m_buttonPanel->layout_align(FL_ALIGN_BOTTOM);
    m_buttonPanel->layout_spacing(3);
    m_buttonPanel->end();

    m_tabs = new Fl_Tabs(0,0,10,10);
    m_tabs->show_tabs(false);
    m_tabs->layout_align(FL_ALIGN_CLIENT);
    m_modalResult = 0;
    m_externalDataSource = (ds != NULL);

    if (ds) m_dataSource = ds;
    else    m_dataSource = new Fl_Dialog_DS(m_tabs);

    callback((Fl_Callback*)escape_callback);
}

Fl_Dialog::~Fl_Dialog() {
    if (!m_externalDataSource)
        delete m_dataSource;
    clear_buttons();
}

Fl_Button *Fl_Dialog::button(int button_mask) const
{
    for(unsigned i = 0; i < m_buttonList.size(); i++) {
        Fl_Widget *btn = m_buttonList[i];
        if(button_mask & btn->argument())
            return (Fl_Button*)btn;
    }
    return (Fl_Button*)0;
}

Fl_Widget *Fl_Dialog::find_widget(const char *field_name) const 
{
    unsigned n_pages = m_tabs->children();
    for (unsigned i = 0; i < n_pages; i++) {
        Fl_Widget *widget = m_tabs->child(i);
        if(widget->is_group()) {
            Fl_Group *page = (Fl_Group *)widget;
            unsigned  n_widgets = page->children();
            for (unsigned j = 0; j < n_widgets; j++) {
                widget = page->child(j);
                if(widget->field_name()==field_name)
                    return widget;
            }
        } else {
            if(widget->field_name()==field_name)
                return widget;
        }
    }
    return (Fl_Widget*)0;
}

const Fl_Variant& Fl_Dialog::operator [] (const char *field_name) const {
    return (*m_dataSource)[field_name];
}

Fl_Variant& Fl_Dialog::operator [] (const char *field_name) {
    return (*m_dataSource)[field_name];
}

void Fl_Dialog::clear_buttons() 
{
    unsigned cnt = m_buttonList.count();
    for (unsigned i = 0; i < cnt; i++) {
        Fl_Widget *btn = m_buttonList[i];
        m_buttonPanel->remove(btn);
        delete btn;
    }
}

void Fl_Dialog::buttons(int buttons_mask,int default_button) 
{
    Fl_Group *saved = Fl_Group::current();

    Fl_Button *btn;
    unsigned i;
    m_defaultButton = 0L;
    m_buttons = buttons_mask;
    clear_buttons();

    int maxh = 25;

    m_buttonPanel->begin();
    for(i = 0; buttonTemplates[i].id; i++) 
    {
        const Fl_Dialog_Button_Template& buttonTemplate = buttonTemplates[i];
        long id = buttonTemplate.id;
        if (buttons_mask & id) 
        {
            if (id == default_button) {
                Fl_Group *default_box = new Fl_Group(0,0,10,10);
                default_box->color(FL_BLACK);
                default_box->box(FL_THIN_DOWN_BOX);
                btn = new Fl_Button(0,0,10,10, _(buttonTemplate.label));
                default_box->end();
                default_box->user_data((void *)id);
                m_defaultButton = btn;
            } else {
                btn = new Fl_Button(0,0,10,10, _(buttonTemplate.label));
            }

            btn->callback((Fl_Callback1*)Fl_Dialog::buttons_callback, id);
            btn->image(buttonTemplate.pixmap);

            m_buttonList.append(btn);

            fl_font(btn->label_font(), btn->label_size());
            int hh = int(fl_height());
            if (btn->image()) {
                int ih = btn->image()->height();
                if (ih > hh) hh = ih;
            }
            hh += btn->box()->dh();// * 2;
            if (hh > maxh) maxh = hh;
        }
    }
    m_buttonPanel->end();

    // resize buttons
    int bx = (w()-layout_spacing()*2) + 3;
    for (i = 0; i < m_buttonList.size(); i++) 
    {
        Fl_Widget *btn = m_buttonList[i];
        fl_font(btn->label_font(), btn->label_size());
        int ww = int(fl_width(btn->label()));

        if (btn->image())
            ww += btn->image()->width() + 3;
        ww += btn->box()->dw() * 2 + 4;
        bx -= ww + 6;
        if (btn == m_defaultButton) {
            Fl_Widget *default_box = btn->parent();
            default_box->resize(bx-2,3,ww+4,maxh+4);
            btn->resize(2,2,ww,maxh);
        } else {
            btn->resize(bx,5,ww,maxh);
        }
    }

    // resize button panel
    m_buttonPanel->h(maxh + 8);

    relayout();
    Fl_Group::current(saved);
}

int Fl_Dialog::handle(int event) 
{
    int rc = inherited::handle(event);
    if (rc)
        return rc;
    if (event == FL_KEY) {
        switch(Fl::event_key()) {
            case FL_Escape:
                clear_value();
                m_modalResult = (int)Fl_Dialog::BTN_CANCEL;
                Fl::exit_modal();
                return 1;
            case FL_Enter:
                if (m_defaultButton) {
                    m_defaultButton->do_callback(FL_DIALOG_BTN);
                    return 1;
                }
        }
    }
    return 0;
}

int Fl_Dialog::show_modal() {
    load_data();
    exec(0,false);
    if (m_modalResult & (Fl_Dialog::BTN_OK|Fl_Dialog::BTN_YES)) 
        save_data();
    return m_modalResult;
}

Fl_Scroll *Fl_Dialog::new_scroll(const char *lbl,bool autoColor) 
{
    if(!m_tabs) return 0;
    if (!m_tabs->children()) {
        m_tabs->show_tabs(false);
    } else {
        m_tabs->show_tabs(true);
    }
    return m_tabs->new_scroll(lbl, autoColor);
}

Fl_Group *Fl_Dialog::new_group(const char *lbl,bool autoColor) 
{
    if(!m_tabs) return 0;
    if (!m_tabs->children()) {
        m_tabs->show_tabs(false);
    } else {
        m_tabs->show_tabs(true);
    }
    return m_tabs->new_group(lbl, autoColor);
}

bool Fl_Dialog::load_data(Fl_Data_Source *ds) 
{
    if (ds) return m_tabs->load_data(ds);
    else    return m_tabs->load_data(m_dataSource);
}

bool Fl_Dialog::save_data(Fl_Data_Source *ds) const {
    if (ds) return m_tabs->save_data(ds);
    else    return m_tabs->save_data(m_dataSource);
}
