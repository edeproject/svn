/***************************************************************************
                          Fl_Params.h  -  description
                             -------------------
    begin                : Tue Dec 14 1999
    copyright            : (C) 1999 by Alexey Parshin
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

#ifndef __Fl_Params_H__
#define __Fl_Params_H__

#include <Fl_String.h>
#include <Fl_Variant.h>
#include <Fl_Ptr_List.h>

class Fl_Param : public Fl_Variant {
   Fl_String   m_name;
   Fl_Ptr_List m_bindParamIndexes;
public:
   Fl_Param(char *name);
   ~Fl_Param();
   Fl_Param& operator = (Fl_Param& param);
   Fl_String  name() const { return m_name; }
   void       bindClear() { m_bindParamIndexes.clear(); }
   void       bindAdd(unsigned bindIndex);
   unsigned   bindCount() { return m_bindParamIndexes.count(); }
   unsigned   bindIndex(unsigned ind) { return m_bindParamIndexes[ind]; }
};

class Fl_Params {
   Fl_Ptr_List m_items;
   void       clear();
public:
   Fl_Params() {};
   ~Fl_Params() { clear(); }
   int        paramIndex(const char *paramName);
   Fl_Param&  operator[] (unsigned index) const;
   Fl_Param&  operator[] (const char *paramName) const;
   Fl_Param&  operator[] (const Fl_String& paramName) const;
   void       add(const Fl_Param * item);
   void       remove(unsigned index);
   void       bind(void *statement);
   unsigned   count() const { return m_items.count(); }
};
//---------------------------------------------------------------------------
#endif
