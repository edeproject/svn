// Equinox Desktop Environment - desktop icons manager
// Copyright (C) 2000-2005 EDE Authors
// This program is licenced under terms of the
// GNU General Public Licence version 2 or newer.
// See COPYING for details.

#include "edeskicon.h"
#include "propdialog.h"
#include "eiconman.h"
#include <edeconf.h>

#include <efltk/fl_draw.h>

#include <efltk/Fl_Image.h>
#include <efltk/Fl_Image_Filter.h>

#define ICONSIZE 48

Fl_Menu_Button *popup;
Icon *menu_item=0;

extern Desktop *desktop;

void menu_cb(Fl_Widget *w, long a)
{
    if(menu_item) {
        switch(a) {
        case 1:
            Icon::cb_execute((Fl_Item*)w, menu_item);
            break;
        case 3:
            delete_icon(w, menu_item);
            break;
        case 4:
            property_dialog(w, menu_item, false);
            break;
        }
    }
}

Fl_String get_localized_string()
{
    Fl_String locale = setlocale(LC_MESSAGES, NULL);
//    int pos = locale.rpos('_');
//    if(pos>0) locale.sub_delete(pos, locale.length()-pos);
    if(locale=="C" || locale=="POSIX") locale.clear();

    Fl_String localName;
    if(locale.empty()) localName = "Name";
    else localName.printf("Name[%s]", locale.c_str());

    return localName;
}

Fl_String get_localized_name(Fl_Config &iconConfig)
{
    Fl_String localName(get_localized_string());
    Fl_String icon_name;
    if(iconConfig.get("Desktop Entry", localName, icon_name, "")) {
        iconConfig.get("Desktop Entry", "Name", icon_name, "None");
    }
    return icon_name;
}

Icon::Icon(char *icon_config) : Fl_Widget(0, 0, ICONSIZE, ICONSIZE)
{
    if(!popup) {
        popup = new Fl_Menu_Button(0, 0, 0, 0);
        if(popup->parent())
            popup->parent()->remove(popup);
        popup->parent(0);
        popup->type(Fl_Menu_Button::POPUP3);
        popup->begin();

        Fl_Item *open_item = new Fl_Item(_("&Open"));
        open_item->callback(menu_cb, 1);
        open_item->x_offset(12);

        Fl_Item *delete_item = new Fl_Item(_("&Delete"));
        delete_item->callback(menu_cb, 3);
        delete_item->x_offset(12);

	   new Fl_Divider();

        Fl_Item *property_item = new Fl_Item(_("&Properties"));
        property_item->callback(menu_cb, 4);
        property_item->x_offset(12);

        popup->end();
    }

    cfg = new Fl_Config(icon_config);

    icon_im = 0;
    micon = 0;

    cfg->set_section("Desktop Entry");
    cfg->read("X", x_position, 100);
    cfg->read("Y", y_position, 100);
    position(x_position, y_position);

//    label_font(FL_HELVETICA);
    label(icon_name);
    align(FL_ALIGN_BOTTOM|FL_ALIGN_WRAP);
    tooltip(icon_name);
    box(FL_NO_BOX);

    update_all();
    desktop->begin();
}

Icon::~Icon()
{
    if (icon_im) delete icon_im;
    if (cfg) delete cfg;
}

void Icon::cb_execute_i()
{
    Fl_Config &iconfig = *cfg;
    iconfig.set_section("Desktop Entry");

    char *cmd=0;
    if(!iconfig.read("Exec", cmd, 0) && cmd)
    {
        char pRun[256];
        char browser[256];
        Fl_Config pGlobalConfig(fl_find_config_file("ede.conf", 0));
        pGlobalConfig.get("Web", "Browser", browser, 0, sizeof(browser));
        if(pGlobalConfig.error() && !browser) {
            strncpy(browser, "netscape", sizeof(browser));
        }

        char *location = cmd;
        char *prefix = strstr(location, ":");
        if(prefix) // it is internet resource
        {
            *prefix = '\0';
            if (!strcasecmp(location, "http") || !strcasecmp(location, "ftp") || !strcasecmp(location, "file"))
            {	snprintf(pRun, sizeof(pRun)-1, "%s %s &", browser, cmd);
            }
	    else if (!strcasecmp(location, "gg"))
	    {	snprintf(pRun, sizeof(pRun)-1, "%s http://www.google.com/search?q=\"%s\" &", browser, ++prefix);
	    }
	    else if (!strcasecmp(location, "leo"))
	    {	snprintf(pRun, sizeof(pRun)-1, "%s http://dict.leo.org/?search=\"%s\" &", browser, ++prefix);
	    }
	    else if (!strcasecmp(location, "av"))
	    {	snprintf(pRun, sizeof(pRun)-1, "%s http://www.altavista.com/sites/search/web?q=\"%s\" &", browser, ++prefix);
	    }
	    else  {
                snprintf(pRun, sizeof(pRun)-1, "%s %s &", browser, cmd);
            }
        }
	else // local executable
        {   snprintf(pRun, sizeof(pRun)-1, "%s &", cmd);
        }
        fl_start_child_process(pRun);

        free((char*)cmd);
    }
}

int Icon::handle(int e)
{
    static int bx, by;
    static int x_icon, y_icon;
    static int X, Y;
    static bool button1 = false;
    int dx, dy;
    

    if (e==FL_PUSH) {
        button1 = (Fl::event_button()==1);
    }

    // Left mouse button
    if(button1) {
        switch(e) {
        case FL_DRAG:

            if(!micon) {
                micon = new MovableIcon(this);
                micon->show();
            }

            dx = ((Fl::event_x_root()-bx)/label_gridspacing) * label_gridspacing;
            dy = ((Fl::event_y_root()-by)/label_gridspacing) * label_gridspacing;
            X=x_icon+dx;
            Y=y_icon+dy;

            if(X<desktop->x()) X=desktop->x();
            if(Y<desktop->y()) Y=desktop->y();
            if(X+w()>desktop->x()+desktop->w()) X=desktop->x()+desktop->w()-w();
            if(Y+h()>desktop->y()+desktop->h()) Y=desktop->y()+desktop->h()-h();

            micon->position(X, Y);

            return 1;

        case FL_RELEASE:

            // This happens only when there was no drag
            if(Fl::event_is_click()) {
                if (one_click_exec)
                    cb_execute_i();
                return 1;
            }

            // We will update config only on FL_RELEASE, when 
            // dragging is over
            if(micon) {
                delete micon;
                micon = 0;
            }

            position(X-desktop->x(), Y-desktop->y());
            desktop->redraw();

            cfg->set_section("Desktop Entry");
            cfg->write("X", x());
            cfg->write("Y", y());
            cfg->flush();

            return 1;
	    
        case FL_PUSH:

            take_focus();

            bx = (Fl::event_x_root()/label_gridspacing)*label_gridspacing;
            by = (Fl::event_y_root()/label_gridspacing)*label_gridspacing;
            x_icon = ((desktop->x()+x())/label_gridspacing)*label_gridspacing;
            y_icon = ((desktop->y()+y())/label_gridspacing)*label_gridspacing;

            // Double click
            if ((!one_click_exec) && (Fl::event_clicks() > 0)) {
                Fl::event_clicks(0);
                cb_execute_i();
            }

            desktop->redraw();

            return 1;
        }
    }

    switch (e) {
    case FL_SHORTCUT:
    case FL_KEY:
        if(Fl::event_key()==FL_Enter||Fl::event_key()==FL_Space) {
            cb_execute_i();
        }
        break;

    case FL_FOCUS:
    case FL_ENTER:
        return 1;

    case FL_PUSH:
        take_focus();
        desktop->redraw();
        if(Fl::event_button()==3) {
            menu_item = this;
            popup->popup();
            menu_item = 0;
            return 1;
        }
        break;

    default:
        break;
    }

    return Fl_Widget::handle(e);
}

void Icon::draw()
{
    Fl_Flags f=0;
    Fl_Image *im = icon_im;
    if(focused()) {
        f=FL_SELECTED;
    }

    if(im)
        im->draw(0, 0, w(), h(),f);
    else {
        fl_color(FL_RED);
        fl_rect(0,0,w(),h());
        fl_color(FL_BLACK);
        fl_rectf(1,1,w()-2,h()-2);
        fl_color(FL_WHITE);
        fl_font(label_font()->bold(), 10);
        fl_draw("NO ICON FOUND!", 1, 1, w()-2, h()-2, FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_WRAP);
    }

    int X = w()-(w()/2)-(lwidth/2);
    int Y = h()+2;

    if(!label_trans) {
        fl_color(label_background);
        fl_rectf(X,Y,lwidth,lheight);
    }

    if(focused()) {
        focus_box()->draw(X, Y, lwidth, lheight, color(), 0);
    }

    fl_font(label_font(), label_size());

    // A little shadow, from Dejan's request :)
    // SUCKS!
    /*fl_color(fl_darker(label_color()));
    fl_draw(label(), X-1, Y+1, lwidth, lheight, flags());
    fl_draw(label(), X, Y+1, lwidth, lheight, flags());
    */

    fl_color(label_color());
    fl_draw(label(), X, Y, lwidth, lheight, flags());
}

void Icon::update_icon()
{
    if(icon_im) delete icon_im;

    Fl_String path(PREFIX"/share/ede/icons/48x48/");
    path += icon_file;

    if(!fl_file_exists(path)) path = icon_file;

    if(fl_file_exists(path))
    {
        icon_im = Fl_Image::read(path, 0);
    } else {
        icon_im = 0;
    }

    if(!icon_im) {
        icon_im = Fl_Image::read(PREFIX"/share/ede/icons/48x48/folder.png", 0);
    }

    if(icon_im) {
        if(icon_im->width()!=48 || icon_im->height()!=48) {
            Fl_Image *old = icon_im;
            icon_im = old->scale(48,48);
            delete old;
        }
        icon_im->mask_type(MASK_ALPHA);
        icon_im->threshold(128);
    }
}

void Icon::layout()
{
    if(layout_damage()&FL_LAYOUT_XYWH && icon_im)
    {
#if 0
        // Alpha blends image to bg!
        // This sucks, cause if icon overlaps another, it will
        // draw bg top of overlapped icon...
        if(icon_im->format()->Amask)
        {
            if(desktop->bg_image) {
                int pitch = icon_im->pitch();

                uint8 *data = new uint8[h()*pitch];

                int X=x(),Y=y(),W=w(),H=h();
                if(X<0) X=0;
                if(Y<0) Y=0;
                if(X+W>desktop->w()) X=desktop->w()-W;
                if(Y+H>desktop->h()) Y=desktop->h()-H;

                Fl_Rect r(X,Y,W,H);
                Fl_Rect r2(0,0,W,H);
                Fl_Renderer::blit(desktop->bg_image->data(), &r, desktop->bg_image->format(), desktop->bg_image->pitch(),
                                  data, &r2, icon_im->format(), pitch, 0);

                if(im) delete im;
                im = new Fl_Image(W, H, icon_im->format(), data);

                // Blit image data to our bg_image
                Fl_Renderer::alpha_blit(icon_im->data(), &r2, icon_im->format(), icon_im->pitch(),
                                        im->data(), &r2, im->format(), im->pitch(),
                                        0);
            } else {
                //blend to color
                im = icon_im->back_blend(desktop->bg_color);
            }
        }
        else
#endif
        {
            if(icon_im) {
                icon_im->mask_type(MASK_ALPHA);
                icon_im->threshold(128);
            }
        }
    }

    Fl_Widget::layout();
}

void Icon::update_all()
{
    Fl_Config &iconConfig = *cfg;
    iconConfig.read_file(false);
    iconConfig.set_section("Desktop Entry");

    // Icon Label:
    icon_name = get_localized_name(iconConfig);
    tooltip(icon_name);
    label(icon_name);

    label_color(label_foreground);
    label_size(label_fontsize);

    lwidth = label_maxwidth; // This is a bit strange, but otherwise we get mysterious crashes...
    lheight= 0;
    fl_font(label_font(), label_size());
    fl_measure(icon_name, lwidth, lheight, FL_ALIGN_WRAP);
    lwidth += 4; //  height+= 4;

    // Icon file:
    iconConfig.read("Icon", icon_file, "folder.png");

    update_icon();

    redraw();
    //desktop->redraw();
}

void save_icon(Icon *i_window)
{
    if(i_name->size()==0) {
        fl_alert(_("Name of the icon must be filled."));
    }
    else
    {
        const char *icon_file = i_filename->value();
        const char *icons_path = PREFIX"/share/ede/icons/48x48";
        if(!strncmp(icons_path, i_filename->value(), strlen(icons_path))) {
            // Only relative path, if icon in default location
            icon_file = fl_file_filename(i_filename->value());
        }

        Fl_Config i_config(i_link->value());
        i_config.set_section("Desktop Entry");

        i_config.write(get_localized_string(), i_name->value());
        i_config.write("Name", i_name->value());	// fallback
        i_config.write("Exec", i_location->value());
        i_config.write("Icon", icon_file);
        i_config.flush();
        i_window->update_all();
    }
}

void delete_icon(Fl_Widget*, Icon *icon)
{
    if (fl_ask(_("Delete this icon?"))) 
    {
        icon->hide();
        Fl_String fname(icon->get_cfg()->filename());
        delete icon;
        if(remove(fname) < 0)
            fl_alert(_("Remove of the icon %s failed. You probably do not have write access to this file."), fname.c_str());
    }
}

int create_new_icon()
{
    int ix=Fl::event_x_root();
    int iy=Fl::event_y_root();
    Icon *icon=0;
    const char *i = fl_input(_("Enter the name of the new icon:"), "The Gimp");
    if (i)
    {
        char config[FL_PATH_MAX];
        snprintf(config, sizeof(config)-1, "%s/.ede/desktop/%s.desktop", getenv("HOME"), i);

        if(!fl_file_exists(config))
        {
            Fl_Config cfg(config);
            cfg.set_section("Desktop Entry");
            cfg.write("Icon", "no icon");
            cfg.write("X", ix);
            cfg.write("Y", iy);
            cfg.write(get_localized_string(), i);
            cfg.write("Exec", "Executable Here");
            //const char *u = fl_input(_("Enter the program name or the location to open:"), "gimp");
            cfg.flush();

            desktop->begin();
            icon = new Icon(config);
            desktop->end();
        }
        else {
            fl_alert(_("The icon with the same name already exists."));
        }
    }
    if(icon) {
        property_dialog(0, icon, true);
        icon->position(ix,iy);
        icon->show();

        desktop->redraw();
        desktop->relayout();
    }
    return 0;
}

void update_iconeditdialog(Icon *i)
{ 
    i_link->value(i->get_cfg()->filename());

    Fl_Config &i_config = *i->get_cfg();

    Fl_String val;
    i_config.set_section("Desktop Entry");

    val = get_localized_name(i_config);
    if(!val.empty()) {
        i_name->value(val);
    }

    if(!i_config.read("Exec", val, 0)) {
        i_location->value(val);
    }

    if(!i_config.read("Icon", val, 0)) {
        i_filename->value(val);
    }
}

void update_property_dialog(Icon *i)
{
    Fl_String val;

    Fl_Config i_config(i->get_cfg()->filename());
    i_config.set_section("Desktop Entry");

    val = get_localized_name(i_config);
    if(!val.empty()) {
        pr_name->label(val);
    }

    if(!i_config.read("Exec", val, 0)) {
        pr_exec->label(val);
    }

    Fl_FileAttr *attr = fl_file_attr(i->get_cfg()->filename());
    if(attr)
    {
        char size[32];
        snprintf(size, 32, _("%d bytes, %s"), (int) attr->size, attr->time);
        pr_size->label(size);
        delete attr;
    }

    pr_icon->image(i->icon_im);
}

MovableIcon::MovableIcon(Icon *i)
: Fl_Window(desktop->x()+i->x(), desktop->y()+i->y(), i->w(), i->h())
{
    icon = i;
    set_override();
    create();

    Fl_Image *im = i->icon_im;
    if(im)
    {
        Pixmap mask = im->create_mask(im->width(), im->height());
        XShapeCombineMask(fl_display, fl_xid(this), ShapeBounding, 0, 0, mask, ShapeSet);

        align(FL_ALIGN_INSIDE);
        image(im);
    }
}

MovableIcon::~MovableIcon()
{
}
