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
 * Author : Alexey Parshin
 * Email  : alexey@fltk.net
 *
 * Please report all bugs and problems to "efltk-bugs@fltk.net"
 *
 */

#ifndef _FL_DATA_SOURCE_H_
#define _FL_DATA_SOURCE_H_

#include "Fl_Variant.h"
#include "Fl_Data_Fields.h"

class Fl_Group;

class Fl_Data_Source {
public:
    // ctor, dtor
    Fl_Data_Source(Fl_Group *group) { parent_ = group; }
    virtual ~Fl_Data_Source() {}

    // access to the field by name
    virtual const Fl_Variant& operator [] (const char *field_name) const = 0;
    virtual Fl_Variant&       operator [] (const char *field_name) = 0;

    // how many fields do we have in the current record?
    virtual unsigned          field_count() const = 0;
    virtual int               field_index(const char *) const { return -1; }

    // access to the field by number, 0..field_count()-1
    virtual const Fl_Variant& operator [] (int) const = 0;
    virtual Fl_Variant&       operator [] (int) = 0;

    virtual const Fl_Data_Field& field (int field_index) const = 0;
    virtual Fl_Data_Field&       field (int field_index) = 0;

    virtual bool              read_field(const char *fname,Fl_Variant& value) = 0;
    virtual bool              write_field(const char *fname,const Fl_Variant& fvalue) = 0;

    // dataset navigation
    virtual bool              open()  { return true; }
    virtual bool              close() { return true; }
    virtual bool              first() { return true; }
    virtual bool              next()  { return true; }
    virtual bool              prior() { return true; }
    virtual bool              last()  { return true; }
    virtual bool              find(Fl_Variant position) { return true; }
    virtual bool              eof() const  { return true; }

    // load data into widgets
    bool load();
    // unload data from widgets
    bool save();

protected:
    // these methods should be implemented in derived class
    virtual bool              load_data() = 0;
    virtual bool              save_data() = 0;

private:
    friend class Fl_Group;
    Fl_Group* parent_;
};

#endif
