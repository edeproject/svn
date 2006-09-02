//
// "$Id$"
//
// OpenGL definitions for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2001 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

// Internal interface to set up OpenGL.
//
// A "Fl_Gl_Choice" is created from an OpenGL mode and holds information
// necessary to create a window (on X) and to create an OpenGL "context"
// (on both X and Win32).
//
// fl_create_gl_context takes a window (necessary only on Win32) and an
// Fl_Gl_Choice and returns a new OpenGL context. All contexts share
// display lists with each other.
//
// On X another fl_create_gl_context is provided to create it for any
// X visual.
//
// fl_set_gl_context makes the given OpenGL context current and makes
// it draw into the passed window. It tracks the current one context
// to avoid calling the context switching code when the same context
// is used, though it is a mystery to me why the GLX/WGL libraries
// don't do this themselves...
//
// fl_no_gl_context clears that cache so the next fl_set_gl_context is
// guaranteed to work.
//
// fl_delete_gl_context destroys the context.
//
// This code is used by Fl_Gl_Window, gl_start(), and gl_visual()

#ifndef Fl_Gl_Choice_H
#define Fl_Gl_Choice_H

#include <efltk/Fl_Window.h> // force Fl_X to be defined by x.h
#include <efltk/x.h>

// Warning: whatever GLContext is defined to must take exactly the same
// space in a structure as a void*!!!
#ifdef _WIN32
# include <efltk/gl.h>
# define GLContext HGLRC
#else
# include <GL/glx.h>
# define GLContext GLXContext
#endif

// Describes crap needed to create a GLContext.
class Fl_Gl_Choice {
  int mode;
  Fl_Gl_Choice *next;
public:
#ifdef _WIN32
  int pixelFormat;	// the visual to use
  PIXELFORMATDESCRIPTOR pfd; // some wgl calls need this thing
#else
  XVisualInfo *vis;	// the visual to use
  Colormap colormap;	// a colormap for that visual
#endif
  // Return one of these structures for a given gl mode, or null if impossible:
  static Fl_Gl_Choice *find(int mode);
};

class Fl_Window;

#ifdef _WIN32

GLContext fl_create_gl_context(const Fl_Window*, const Fl_Gl_Choice*, int layer=0);

#else

GLContext fl_create_gl_context(XVisualInfo* vis);

static inline
GLContext fl_create_gl_context(const Fl_Window*, const Fl_Gl_Choice* g) {
  return fl_create_gl_context(g->vis);
}

#endif

void fl_set_gl_context(const Fl_Window*, GLContext);
void fl_no_gl_context();
void fl_delete_gl_context(GLContext);

#endif

//
// End of "$Id$".
//
