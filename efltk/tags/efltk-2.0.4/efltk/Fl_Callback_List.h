/*
 * $Id$
 *
 * Extended Fast Light Toolkit (EFLTK)
 * Copyright (C) 2002-2003 by EDE-Team
 * WWW: http://www.sourceforge.net/projects/ede
 *
 * Fast Light Toolkit (FLTK)
 * Copyright (C) 1998-2003 by Bill Spitzak and others.
 * WWW: http://www.fltk.org
 *
 * This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
 * version 2. See COPYING for details.
 *
 * Author : Mikko Lahteenmaki
 * Email  : mikko@fltk.net
 *
 * Please report all bugs and problems to "efltk-bugs@fltk.net"
 *
 */


#ifndef _FL_CALLBACK_LIST_H_
#define _FL_CALLBACK_LIST_H_

#include "Fl_Ptr_List.h"
#include "Fl_Widget.h"

typedef Fl_Callback * Fl_Callback_p; // for Borland CBuilder

typedef struct CallbackDataStruct {
    CallbackDataStruct(Fl_Callback *c, void *a) { cb=c; arg=a; }
    Fl_Callback *cb;
    void *arg;
} CallbackData;

/** Fl_Callback_List */
class Fl_Callback_List : public Fl_Ptr_List {
public:
    Fl_Callback_List() : Fl_Ptr_List() { auto_delete(true); }
	virtual ~Fl_Callback_List() { clear(); }

    void do_callback(Fl_Widget *w);
    void do_callback(Fl_Widget *w, void *arg);

    void append(Fl_Callback *cb, void *arg) { Fl_Ptr_List::append((void *)new CallbackData(cb, arg)); }
    void prepend(Fl_Callback *cb, void *arg) { Fl_Ptr_List::prepend((void *)new CallbackData(cb, arg)); }
    void insert(uint pos, Fl_Callback *cb, void *arg) { Fl_Ptr_List::insert(pos, (void *)new CallbackData(cb, arg)); }
    void replace(uint pos,Fl_Callback *cb, void *arg) { Fl_Ptr_List::replace(pos, (void *)new CallbackData(cb, arg)); }
    void remove(uint pos) { Fl_Ptr_List::remove(pos); }
    bool remove(Fl_Callback *cb, void *arg);
    int index_of(Fl_Callback *cb, const void *arg) const;

    Fl_Callback_p callback(uint index) const { return (Fl_Callback_p)((CallbackData*)Fl_Ptr_List::item(index))->cb; }
    void *arg(uint index) const { return (void*)((CallbackData*)Fl_Ptr_List::item(index))->arg; }
    CallbackData *item(uint index) const { return (CallbackData*)Fl_Ptr_List::item(index); }

    CallbackData **data() { return (CallbackData**)items; }
    CallbackData *operator [](uint ind) const { return (CallbackData*)items[ind]; }

protected:
    virtual void free_item(Fl_Ptr_List_Item item) { delete (CallbackData*)(item); }
};

#endif
