#ifndef _ITEM_H_
#define _ITEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*#include <efltk/Fl.h>
#include <efltk/Fl_Window.h>
#include <efltk/x.h>
#include <efltk/Fl_Menu_Button.h>
#include <efltk/Fl_Item_Group.h>
#include <efltk/Fl_Item.h>
#include <efltk/filename.h>
#include <efltk/Fl_Divider.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Config.h>
#include <efltk/Fl_Image.h>
#include <efltk/Fl_Locale.h>*/

#include <fltk/Fl.h>
#include <fltk/Window.h>
#include <fltk/x.h>
#include <fltk/Menu_Button.h>
#include <fltk/Item_Group.h>
#include <fltk/Item.h>
#include <fltk/filename.h>
#include <fltk/Divider.h>
#include <fltk/Locale.h>
#include "EDE_Config.h"
#include <fltk/Image.h>


class MainMenu;

enum {
    NO_TYPE = 0,
    APP_GROUP,
    BROWSER_GROUP
};

class EItemGroup : public fltk::Item_Group {
public:
    EItemGroup(MainMenu *menu, int type, const char *name=0);
    ~EItemGroup() { }

    void add_items();

    int handle(int event);

    int group_type() const { return m_gtype; }
    void group_type(int val) { m_gtype = val; }

    bool access() const { return m_access; }
    void access(bool val) { m_access = val; }

//    void dir(const Fl_String &dir) { m_dir = dir; }
    void dir(const char *dir) { strcpy(m_dir, dir); }
    const char *dir() const { return m_dir; }

    MainMenu *menu() { return m_menu; }

private:
    long m_modified;
    int m_gtype;
    char* m_dir;
    bool m_access;
    MainMenu *m_menu;
};

class EItem : public fltk::Item {
public:
    EItem(MainMenu *menu, const char *name=0) : fltk::Item(name) { m_menu = menu; }

    enum { FILE=fltk::Item::NO_EXECUTE+1 };

    int handle(int event);

    const char* dir() const { return ((EItemGroup*)parent())->dir(); }

    void exec(const char *exec) { strcpy(m_exec, exec); }
//    void exec(const Fl_String &exec) { m_exec = exec; }
    const char* exec() const { return m_exec; }

    void filename(const char *filename) { strcpy(m_filename, filename); }
//    void filename(const Fl_String &filename) { m_filename = filename; }
    const char* filename() const { return m_filename; }

    MainMenu *menu() const { return m_menu; }

private:
    char* m_filename;
    char* m_exec;
    MainMenu *m_menu;
};

#endif
