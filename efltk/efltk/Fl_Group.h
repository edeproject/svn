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

#ifndef _FL_GROUP_H_
#define _FL_GROUP_H_

#include "Fl_Widget.h"
#include "Fl_Data_Source.h"
#include "Fl_Widget_List.h"
#include "Fl_Int_List.h"

class FL_API Fl_Group : public Fl_Widget {
public:
    Fl_Group(int,int,int,int, const char * = 0);
    virtual ~Fl_Group();

    int children() const { return array_.size(); }
    Fl_Widget* child(int n) const { return array_.item(n); }
    Fl_Widget_List &array() { return array_; }

    void draw_group_box() const;
    virtual void draw();
    virtual void layout();
    virtual int handle(int);

    void begin() { current_ = this; }
    void end()   { current_ = (Fl_Group*)parent(); }
    static Fl_Group *current() {return current_;}
    static void current(Fl_Group *g) {current_ = g;}

    int find(const Fl_Widget*) const;
    int find(const Fl_Widget& o) const {return find(&o);}

    void add(Fl_Widget&);
    void insert(Fl_Widget&, int index);
    void remove(int index);
    void replace(int index, Fl_Widget&);
    void clear();

    void add(Fl_Widget* o) {add(*o);}
    void insert(Fl_Widget& o, Fl_Widget* before) {insert(o,find(before));}
    void remove(Fl_Widget& o) {remove(find(o));}
    void remove(Fl_Widget* o) {remove(find(*o));}
    void replace(Fl_Widget& old, Fl_Widget& o) {replace(find(old),o);}

    void resizable(Fl_Widget& o) {resizable_ = &o;}
    void resizable(Fl_Widget* o) {resizable_ = o;}
    Fl_Widget* resizable() const {return resizable_;}
    void add_resizable(Fl_Widget& o) {resizable_ = &o; add(o);}
    void init_sizes();

    void focus(int i) {focus_ = i;}
    void focus(Fl_Widget* w) {focus(find(w));}
    int focus() const {return focus_;}
    static int navigation_key();

    // data source support methods
    void data_source(Fl_Data_Source *ds);
    Fl_Data_Source* data_source() const { return data_source_; }
    virtual bool load_data(Fl_Data_Source *ds);
    virtual bool save_data(Fl_Data_Source *ds) const;

protected:
    void draw_child(Fl_Widget&) const;
    void update_child(Fl_Widget&) const;
    void draw_outside_label(Fl_Widget&) const ;
    int* sizes();

private:
    Fl_Widget_List array_;
    Fl_Int_List sizes_; // remembered initial sizes of children

    Fl_Widget* resizable_;
    Fl_Data_Source* data_source_;

    int focus_;

    static Fl_Group *current_;
};

// This dummy class can be used in constructors to set the parent
// group to any desired value before creating child widget instances.
class FL_API Fl_End {
public:
    Fl_End() {Fl_Group::current()->end();}
    Fl_End(Fl_Group* g) {Fl_Group::current(g);}
};

#endif
