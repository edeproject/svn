// Fl_Time.cxx
// Source file for the time widget class
//
// Copyright (C) 2000 Softfield Research Ltd.
//
// Changes 02/09/2001 by Martin Pekar
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include <efltk/Fl_Item.h>
#include <efltk/fl_ask.h>
#include <efltk/Fl_Util.h>
#include <efltk/Fl_Locale.h>
#include <efltk/filename.h>

#include "etimedate.h"
#include "fl_time.h"

Fl_Time::Fl_Time(int x, int y, int w, int h, char *l) : Fl_Group(x, y, w, h, l)
{
    int button_width = (int)(w/7);

    input_time = new Fl_Input(0, 0, w - button_width * 4, h, 0);
    input_time->callback(input_changed_cb, this);
    input_time->when(FL_WHEN_CHANGED);
//    input_time->textsize(12);

    button_decrease_hour = new Fl_Button(w - 4 * button_width, 2, button_width, h-4, "@-1<<");
    button_decrease_hour->callback(button_cb, this);
    button_decrease_hour->label_type(FL_SYMBOL_LABEL);
    button_decrease_hour->box(FL_THIN_UP_BOX);
    button_decrease_hour->label_color(button_color());
    button_decrease_hour->highlight_color(fl_color_average(button_color(), FL_GRAY, .5f));

    button_increase_hour = new Fl_Button(w - 3 * button_width, 2, button_width, h-4, "@-1>>");
    button_increase_hour->callback(button_cb, this);
    button_increase_hour->label_type(FL_SYMBOL_LABEL);
    button_increase_hour->box(FL_THIN_UP_BOX);
    button_increase_hour->label_color(button_color());
    button_increase_hour->highlight_color(fl_color_average(button_color(), FL_GRAY, .5f));
    
    button_decrease_minute = new Fl_Button(w - 2 * button_width, 2, button_width, h-4, "@-1<");
    button_decrease_minute->callback(button_cb, this);
    button_decrease_minute->label_type(FL_SYMBOL_LABEL);
    button_decrease_minute->box(FL_THIN_UP_BOX);
    button_decrease_minute->label_color(button_color());
    button_decrease_minute->highlight_color(fl_color_average(button_color(), FL_GRAY, .5f));

    button_increase_minute = new Fl_Button(w - button_width, 2, button_width, h-4, "@-1>");
    button_increase_minute->callback(button_cb, this);
    button_increase_minute->label_type(FL_SYMBOL_LABEL);
    button_increase_minute->box(FL_THIN_UP_BOX);
    button_increase_minute->label_color(button_color());
    button_increase_minute->highlight_color(fl_color_average(button_color(), FL_GRAY, .5f));

    end();

    //type(FL_TIME_12HOUR);
    type(FL_TIME_24HOUR);
    current_time();
}


void Fl_Time::current_time()
{
    struct tm* display_time_tm;

    gettimeofday(&current_tv, 0);
    display_tv.tv_sec = current_tv.tv_sec;
    display_tv.tv_usec = current_tv.tv_usec;
    display_time_tm = localtime(&current_tv.tv_sec);

    if(type() == FL_TIME_24HOUR) strftime(time_string, 19, "%2H:%2M", display_time_tm);
    else strftime(time_string, 19, "%2I:%2M %p", display_time_tm);

    input_time->value(time_string);
}


void Fl_Time::refresh()
{
    long different;
    struct tm *display_time_tm;

    if (valid())
    {
        different = - display_tv.tv_sec + current_tv.tv_sec;
        gettimeofday(&current_tv, 0);

        display_tv.tv_sec = current_tv.tv_sec - different;
        display_time_tm = localtime(&display_tv.tv_sec);

        if(type() == FL_TIME_24HOUR) strftime(time_string, 19, "%2H:%2M", display_time_tm);
        else strftime(time_string, 19, "%2I:%2M %p", display_time_tm);

        input_time->value(time_string);
    }
}


int Fl_Time::hour()
{
    struct tm *display_time_tm;

    display_time_tm = localtime(&display_tv.tv_sec);
    return display_time_tm->tm_hour;
}


int Fl_Time::minute()
{
    struct tm *display_time_tm;

    display_time_tm = localtime(&display_tv.tv_sec);
    return display_time_tm->tm_min;
}


void Fl_Time::redisplay()
{
    struct tm *display_time_tm;

    display_time_tm = localtime(&display_tv.tv_sec);

    if(type() == FL_TIME_24HOUR) strftime(time_string, 19, "%2H:%2M", display_time_tm);
    else strftime(time_string, 19, "%2I:%2M %p", display_time_tm);

    input_time->value(time_string);
}


void Fl_Time::hour(int value)
{
    struct tm *display_time_tm;

    display_time_tm = localtime(&display_tv.tv_sec);
    display_time_tm->tm_hour = value;
    display_tv.tv_sec = mktime(display_time_tm);
}


void Fl_Time::minute(int value)
{
    struct tm *display_time_tm;

    display_time_tm = localtime(&display_tv.tv_sec);
    if(value < 0)
    {
        display_time_tm->tm_min = 59;
    }
    else if(value >= 0 && value <= 59)
    {
        display_time_tm->tm_min = value;
    }
    else if (value > 59)
    {
        display_time_tm->tm_min = 0;
    }
    display_time_tm->tm_sec = 0;
    display_tv.tv_sec = mktime(display_time_tm);
}


void Fl_Time::settime()
{
    struct tm *display_time_tm;
    display_time_tm = localtime(&display_tv.tv_sec);
    //  return display_time_tm->tm_min;
    time_t ct = mktime (display_time_tm);
    if (stime(&ct)!=0)
        fl_alert(_("Error setting time. You are probably not superuser!"));
}


void Fl_Time::value(int h, int m)
{
    hour(h);
    minute(m);
}


bool Fl_Time::valid()
{
    int h, m;
    char a[5];

    if(type() == FL_TIME_12HOUR)
    {
        if (sscanf(input_time->value(), "%d:%d %s",&h, &m, a) == 3)
        {
            if (h >= 1 && h <= 12 && m >= 0 && m <= 59 && (strcasecmp(a, "am") == 0 || strcasecmp(a, "pm") == 0))
            {
                last_valid = true;
                return true;
            }
        }
    }
    else
    {
        if (sscanf(input_time->value(), "%d:%d",&h, &m) == 2)
        {
            if (h >= 0 && h <= 23 && m >= 0 && m <= 59)
            {
                last_valid = true;
                return true;
            }
        }
    }
    last_valid = false;
    return false;
}


void Fl_Time::input_changed_cb(Fl_Widget* widget, void* data)
{
    Fl_Time* t = (Fl_Time*) data;
    int h, m;
    char a[5];

    if (t->valid())
    {
        if(t->type() == FL_TIME_12HOUR)
        {
            sscanf(t->input_time->value(), "%d:%d %2s",&h, &m, a);
            if(strcasecmp(a, "am") == 0)
            {
                if(h < 12)
                {
                    t->hour(h);
                }
                else
                {
                    t->hour(0);
                }
            }
            else
            {
                if(h < 12)
                {
                    t->hour(h + 12);
                }
                else
                {
                    t->hour(12);
                }
            }
        }
        else
        {
            sscanf(t->input_time->value(), "%d:%d",&h, &m);
            t->hour(h);
        }
        t->minute(m);
    }
    t->do_callback();
}


void Fl_Time::button_cb(Fl_Widget* widget, void* data)
{
    Fl_Time* t = (Fl_Time*) data;

    if(widget == t->button_decrease_hour)
    {
        t->hour(t->hour()-1);
    }
    if (widget == t->button_decrease_minute)
    {
        t->minute(t->minute()-1);
    }
    if (widget == t->button_increase_minute)
    {
        t->minute(t->minute()+1);
    }
    if (widget == t->button_increase_hour)
    {
        t->hour(t->hour()+1);
    }
    t->redisplay();
    t->do_callback();
}


void Fl_Time::textsize(int size)
{
    input_time->text_size(size);
}


void Fl_Time::labelsize(int size)
{
    button_decrease_hour->label_size(size);
    button_decrease_minute->label_size(size);
    button_increase_minute->label_size(size);
    button_increase_hour->label_size(size);
    Fl_Group::label_size(size);
}


void Fl_Time::textfont(Fl_Font font)
{
    input_time->text_font(font);
}


void Fl_Time::labelfont(Fl_Font font)
{
    button_decrease_hour->label_font(font);
    button_decrease_minute->label_font(font);
    button_increase_minute->label_font(font);
    button_increase_hour->label_font(font);
    Fl_Group::label_font(font);
}


void Fl_Time::textcolor(Fl_Color color)
{
    input_time->text_color(color);
}


void Fl_Time::labelcolor(Fl_Color color)
{
    button_decrease_hour->label_color(color);
    button_decrease_minute->label_color(color);
    button_increase_minute->label_color(color);
    button_increase_hour->label_color(color);
    Fl_Group::label_color(color);
}


int Fl_Time::textsize()
{
    return input_time->text_size();
}


int Fl_Time::labelsize()
{
    return button_decrease_hour->label_size();
}


Fl_Font Fl_Time::labelfont()
{
    return button_decrease_hour->label_font();
}


Fl_Font Fl_Time::textfont()
{
    return input_time->text_font();
}


Fl_Color Fl_Time::labelcolor()
{
    return button_decrease_hour->label_color();
}


Fl_Color Fl_Time::textcolor()
{
    return input_time->text_color();
}


// timezone functions

void getCurrentTimeZone()
{
    char szZone[100];

    if(readlink("/etc/localtime", szZone, sizeof(szZone)-1)>0) {
        char **tz = fl_split(szZone, "/zoneinfo/", 2);
        timeZonesList->value(tz[1]);
        fl_freev(tz);
    } else {
        timeZonesList->value(_("Zone information not found."));
    }
}

static char *zonetab_dir = 0;
void saveTimeZone()
{
    if(!zonetab_dir) {
        fl_alert(_("Zone information not found."));
    }

    char tz[FL_PATH_MAX];
    snprintf(tz, sizeof(tz)-1, "%s%s", zonetab_dir, timeZonesList->value());

    unlink( "/etc/localtime" );

    if (symlink(tz, "/etc/localtime" ) != 0)
        fl_alert(_("Cannot setup timezone!"));

    char val[FL_PATH_MAX];
    snprintf(val, sizeof(val)-1, "TZ=%s", tz);
    putenv(val);
}

int sort_f(const void *w1, const void *w2) {
    Fl_Widget *widget1 = *(Fl_Widget**)w1;
    Fl_Widget *widget2 = *(Fl_Widget**)w2;
    return strcmp(widget1->label(), widget2->label());
}

void fillTimeZones()
{
    // This funtion is very lame :)

    FILE *f;
    char tempstring[101] = "Unknown";

    if(fl_file_exists("/usr/share/zoneinfo/zone.tab")) {
        fl_start_child_process("cat /usr/share/zoneinfo/zone.tab | grep -e  ^[^#] | cut -f 3 > /tmp/_tzone_.txt");
        zonetab_dir = "/usr/share/zoneinfo/";
    }
    else if(fl_file_exists("/usr/local/share/zoneinfo/zone.tab")) {
        fl_start_child_process("cat /usr/local/share/zoneinfo/zone.tab | grep -e  ^[^#] | cut -f 3 > /tmp/_tzone_.txt");
        zonetab_dir = "/usr/local/share/zoneinfo/";
    } else {
        Fl_Item *o = new Fl_Item(_("Zone information not found."));
        o->label_color(FL_RED);
        return;
    }

    if((f = fopen("/tmp/_tzone_.txt", "r")) != NULL)
    {
        while(fgets(tempstring, 100, f) != NULL)
        {
            Fl_Item *o = new Fl_Item();
            o->copy_label(fl_trimright(tempstring));
        }
        fclose(f);
    }
    remove("/tmp/_tzone_.txt");
    Fl_Group::current()->array().sort(sort_f);
}
