#ifndef _DOCK_H_
#define _DOCK_H_

#include <efltk/Fl_Group.h>

class Dock : public Fl_Group
{
public:
    Dock();

    void add_to_tray(Fl_Widget *w);
    void remove_from_tray(Fl_Widget *w);
};

#endif
