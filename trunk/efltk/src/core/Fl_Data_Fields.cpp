/***************************************************************************
                          Fl_Data_Fields.cpp  -  description
                             -------------------
    begin                : Mon Nov 25 2002
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

#include <efltk/Fl_Data_Fields.h>
#include <efltk/Fl_Exception.h>
#include <stdio.h>

const Fl_Variant Fl_Data_Fields::m_fieldNotFound;

// convertors
int Fl_Data_Field::as_int() const {
   switch (value.type()) {
   case VAR_INT:        return value.get_int();              break;
   case VAR_FLOAT:      return (int)value.get_float();       break;
   case VAR_STRING:
   case VAR_TEXT:
   case VAR_BUFFER:     return atoi(value.get_string());     break;
   case VAR_DATETIME:   return int(value.get_date());        break;
   case VAR_NONE:       break;
   }
   return 0;
}

double Fl_Data_Field::as_float() const {
   switch (value.type()) {
   case VAR_INT:        return value.get_int();              break;
   case VAR_FLOAT:      return value.get_float();            break;
   case VAR_STRING:
   case VAR_TEXT:
   case VAR_BUFFER:     return atof(value.get_string());     break;
   case VAR_DATETIME:   return double(value.get_date());     break;
   case VAR_NONE:       break;
   }
   return 0;
}

Fl_String Fl_Data_Field::as_string() const {
   char print_buffer[32];
   switch (value.type()) {
   case VAR_INT:        sprintf(print_buffer,"%i",value.get_int());
                        return Fl_String(print_buffer);
   case VAR_FLOAT:      sprintf(print_buffer,"%0.4f",value.get_float());
                        return Fl_String(print_buffer);
   case VAR_STRING:
   case VAR_TEXT:
   case VAR_BUFFER:     return Fl_String(value.get_string());
   case VAR_DATETIME:   return value.get_date().date_string() + " " + value.get_date().time_string();
   case VAR_NONE:       break;
   }
   return "";
}

Fl_Date_Time Fl_Data_Field::as_date() const {
   Fl_Date_Time   result;
   switch (value.type()) {
   case VAR_INT:        result = value.get_int();
                        break;
   case VAR_FLOAT:      result = value.get_float();
                        break;
   case VAR_STRING:
   case VAR_TEXT:
   case VAR_BUFFER:     result = value.get_string();
                        break;
   case VAR_DATETIME:   result = value.get_date();
                        break;
   case VAR_NONE:
                        break;
   }
   return result;
}

void Fl_Data_Fields::clear() {
   unsigned cnt = m_list.count();
   for (unsigned i = 0; i < cnt; i++) {
      Fl_Data_Field *field = (Fl_Data_Field *)m_list[i];
      delete field;
   }
}

Fl_Data_Field& Fl_Data_Fields::add(const char *fname) {
   int index = field_index(fname);
   if (index < 0) {
      Fl_Data_Field *field = new Fl_Data_Field(fname);
      m_list.append(field);
      return *field;
   }
   throw Fl_Exception("Attempt to duplicate field name",__FILE__,__LINE__);
}

int Fl_Data_Fields::field_index(const char *fname) const {
   unsigned cnt = m_list.count();
   for (unsigned i = 0; i < cnt; i++) {
      Fl_Data_Field *field = (Fl_Data_Field *)m_list[i];
      if (strcmp(field->name(),fname) == 0)
         return i;
   }
   return -1;  // field not found
}

Fl_Variant& Fl_Data_Fields::operator [] (int index) {
   Fl_Data_Field *field = (Fl_Data_Field *)m_list[index];
   return field->value;
}

const Fl_Variant& Fl_Data_Fields::operator [] (int index) const {
   Fl_Data_Field *field = (Fl_Data_Field *)m_list[index];
   return field->value;
}

Fl_Variant& Fl_Data_Fields::operator [] (const char *fname) {
   Fl_Data_Field *field;
   int index = field_index(fname);
   if (index < 0) {
      throw Fl_Exception("Field name not found",__FILE__,__LINE__);
      //field = new Fl_Data_Field(fname);
      //m_list.append(field);
   } else {
      field = (Fl_Data_Field *)m_list[index];
   }
   return field->value;
}

const Fl_Variant& Fl_Data_Fields::operator [] (const char *fname) const {
   int index = field_index(fname);
   if (index < 0) {
      throw Fl_Exception("Field name not found",__FILE__,__LINE__);
      //return m_fieldNotFound;
   } else {
      Fl_Data_Field *field = (Fl_Data_Field *)m_list[index];
      return field->value;
   }
}

const Fl_Data_Field& Fl_Data_Fields::field(unsigned index) const {
   return *(Fl_Data_Field *)m_list[index];
}

Fl_Data_Field& Fl_Data_Fields::field(unsigned index) {
   return *(Fl_Data_Field *)m_list[index];
}

