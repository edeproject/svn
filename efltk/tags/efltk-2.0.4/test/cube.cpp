//
// "$Id$"
//
// Another forms test program for the Fast Light Tool Kit (FLTK).
//
// Modified to have 2 cubes to test multiple OpenGL contexts
//
// Copyright 1998-1999 by Bill Spitzak and others.
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

#include <config.h>
#include <efltk/Fl.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Radio_Light_Button.h>
#include <efltk/Fl_Slider.h>
#include <stdlib.h>

#if !HAVE_GL
class cube_box : public Fl_Box {
public:	
  double lasttime;
  int wire;
  double size;
  double speed;
  cube_box(int x,int y,int w,int h,const char *l=0)
    :Fl_Box(x,y,w,h,l){
        box(FL_DOWN_BOX);
	label("This demo does\nnot work without GL");
  }
};
#else
#include <efltk/Fl_Gl_Window.h>
#include <efltk/gl.h>

class cube_box : public Fl_Gl_Window {
  void draw();
public:
  double lasttime;
  int wire;
  double size;
  double speed;
  cube_box(int x,int y,int w,int h,const char *l=0)
    : Fl_Gl_Window(x,y,w,h,l) {lasttime = 0.0;}
};

/* The cube definition */
float v0[3] = {0.0, 0.0, 0.0};
float v1[3] = {1.0, 0.0, 0.0};
float v2[3] = {1.0, 1.0, 0.0};
float v3[3] = {0.0, 1.0, 0.0};
float v4[3] = {0.0, 0.0, 1.0};
float v5[3] = {1.0, 0.0, 1.0};
float v6[3] = {1.0, 1.0, 1.0};
float v7[3] = {0.0, 1.0, 1.0};

#define v3f(x) glVertex3fv(x)

void drawcube(int wire) {
/* Draw a colored cube */
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(0,0,255);
  v3f(v0); v3f(v1); v3f(v2); v3f(v3);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(0,255,255); v3f(v4); v3f(v5); v3f(v6); v3f(v7);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(255,0,255); v3f(v0); v3f(v1); v3f(v5); v3f(v4);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(255,255,0); v3f(v2); v3f(v3); v3f(v7); v3f(v6);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(0,255,0); v3f(v0); v3f(v4); v3f(v7); v3f(v3);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(255,0,0); v3f(v1); v3f(v2); v3f(v6); v3f(v5);
  glEnd();
}

void cube_box::draw() {
  lasttime = lasttime+speed;
  if (!valid()) {
    glLoadIdentity();
    glViewport(0,0,w(),h());
    glEnable(GL_DEPTH_TEST);
    glFrustum(-1,1,-1,1,2,10000);
    glTranslatef(0,0,-10);
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glRotatef(float(lasttime*1.6),0,0,1);
  glRotatef(float(lasttime*4.2),1,0,0);
  glRotatef(float(lasttime*2.3),0,1,0);
  glTranslatef(-1.0, 1.2f, -1.5);
  glScalef(float(size),float(size),float(size));
  drawcube(wire);
  glPopMatrix();
}

#endif

Fl_Window *form;
Fl_Slider *speed, *size;
Fl_Button *button, *wire, *flat;
cube_box *cube, *cube2;

static void exit_cb(Fl_Widget* w, void*) {exit(0);}

void makeform(const char *name) {
  form = new Fl_Window(510+390,390,name);
  (void) new Fl_Box(20,20,350,350,"");
  (void) new Fl_Box(510,20,350,350,"");
  speed = new Fl_Slider(390,90,40,220,"Speed");
  size = new Fl_Slider(450,90,40,220,"Size");
  wire = new Fl_Radio_Light_Button(390,20,100,30,"Wire");
  flat = new Fl_Radio_Light_Button(390,50,100,30,"Flat");
  button = new Fl_Button(390,340,100,30,"Exit");
  button->callback(exit_cb);
  cube = new cube_box(23,23,344,344, 0);
  cube2 = new cube_box(513,23,344,344, 0);
  Fl_Box *b = new Fl_Box(cube->x(),size->y(),
			 cube->w(),size->h(),0);
  form->resizable(b);
  b->hide();
  form->end();
}
#include <stdio.h>

int main(int argc, char **argv) {
  makeform(argv[0]);
  speed->range(0,4);
  speed->value(cube->speed = cube2->speed = 1.0);
  size->range(0.01,4);
  size->value(cube->size = cube2->size = 1.0);
  flat->value(1); cube->wire = 0; cube2->wire = 1;
  form->label("cube");
  form->show(argc,argv);
#if 0
  Fl::run();
#else
  for (;form->visible();) {
    if (form->iconic() || !speed->value())
      Fl::wait();	// waits until something happens
    else
      Fl::check();	// returns immediately
    cube->wire = wire->value();
    cube2->wire = !wire->value();
    cube->size = cube2->size = size->value();
    cube->speed = cube2->speed = speed->value();
    cube->redraw();
    cube2->redraw();
  }
#endif
  return 0;
}

//
// End of "$Id$".
//
