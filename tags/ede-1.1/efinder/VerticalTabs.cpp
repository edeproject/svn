/*
 * $Id$
 *
 * Efinder, file search tool
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */


#include <edeconf.h>
#include "VerticalTabs.h"


static Fl_Widget* active_child = 0;
int toplabelwidth;

/*
Fl_Widget* VerticalTabs::selected_child() {
  int i = value();
  return i >= 0 ? child(i) : 0;
}*/

#define	TAB_HEIGHT	25
#define	TAB_SLOPE	5
#define	TAB_SPACING	3


int VerticalTabs::which(int event_x, int event_y) {
  int x2 = toplabelwidth + TAB_SPACING*4+TAB_SLOPE;
  int cur = 0;
  int prev = 0;
  
  if (event_y < 0) return -1;
  if ((event_x < 0) || (event_x > x2)) return -1;

  for (int i=0; i<children(); i++) {
    prev = cur;
    cur += TAB_HEIGHT;
    if ((event_y > prev) && (event_y < cur)) return i;
    if (active_child == child(i)) cur += active_child->h();
  }
  return -1;
}



int VerticalTabs::handle(int event) {

  int i = value();
  Fl_Widget* selected = i >= 0 ? child(i) : 0;
  int backwards = 0;

  switch (event) {

  case FL_FOCUS:
  case FL_UNFOCUS: 
    redraw();
    return 1;

  // handle mouse events in the tabs:
  case FL_PUSH:
  case FL_DRAG:
  case FL_RELEASE: 
    i = which(Fl::event_x(), Fl::event_y());
    if (i == -1) break;
    if (event == FL_RELEASE && !pushed()) {
      push(0);
      if (i >= 0 && value(i)) do_callback();
    } else {
      push(i >= 0 ? child(i) : 0);
    }
    return 1;

    
  // handle pointing at the tabs to bring up tooltip:
  case FL_ENTER:
  case FL_MOVE: {
    i = which(Fl::event_x(), Fl::event_y());
    if (i >= 0)
      Fl_Tooltip::enter(this, 0, TAB_HEIGHT<0?h()+TAB_HEIGHT:0, w(), TAB_HEIGHT<0?-TAB_HEIGHT:TAB_HEIGHT,
                     child(i)->tooltip());
    return 1;}


  // FIXME: keyboard basically doesn't work
  case FL_KEY:
    if (Fl::focus()==this) {
      switch (Fl::event_key()) {
      case FL_Space:
      case FL_Right:
        goto MOVE;
      case FL_BackSpace:
      case FL_Left:
        backwards = 1;
      MOVE:
        if (backwards) {i = i ? i-1 : children()-1;}
        else {i++; if (i >= children()) i = 0;}
        if (value(i)) do_callback();
        return 1;
      }
    }
/*    switch (navigation_key()) {
    case FL_Right:
    case FL_Down:
      if (focus_index() < 0 && selected) return selected->take_focus();
      else return 0;
    case FL_Left:
    case FL_Up:
      if (focus_index() >= 0) {Fl::focus(this); return 1;}
      else return 0;
    }*/
    // fall through to SHORTCUT case:
  case FL_SHORTCUT:
    if (Fl::event_key() == FL_Tab && Fl::event_state(FL_CTRL)) {
      backwards = Fl::event_state(FL_SHIFT);
      goto MOVE;
    }
    break;

  }

  if (selected) return selected->send(event);
  return 0;
}


int VerticalTabs::push(Fl_Widget *o) {
  if (active_child == o) return 0;
  if (active_child && !active_child->visible() || o && !o->visible())
    redraw(FL_DAMAGE_VALUE);
  if (o != 0) active_child = o;
  return 1;
}


int VerticalTabs::value() {
  for (int i=0; i < children(); i++) {
    if (active_child == child(i)) return i;
  }
  return -1;
}


bool VerticalTabs::value(int n) {
  if (n < 0) n = 0;
  if (n >= children()) {
    if (!children()) return false;
    n = children()-1;
  }
  return (active_child == child(n));
}


int VerticalTabs::drawtab (int number, int height, Fl_Widget* o, bool selected) {
	int sel = selected;	// cast bool to int
	int x2 = toplabelwidth + TAB_SPACING*4;

	Fl_Color fillcolor = o->color();
	if (fillcolor == FL_INVALID_COLOR) fillcolor = parent()->color();
	Fl_Color thiscolor = this->color();
	if (thiscolor == FL_INVALID_COLOR) thiscolor = parent()->color();

	// background
//	fl_color(fl_darker(fillcolor));
	fl_color(thiscolor);
	fl_newpath();
	fl_vertex(0, height);
	fl_vertex(w(), height);
	fl_vertex(w(), height+TAB_HEIGHT);
	fl_vertex(0, height+TAB_HEIGHT);
	fl_fill();
	
	// triangular tab
//	if (sel) fl_color(fillcolor); else fl_color(fl_color_average(fillcolor, FL_BLACK, 0.85f));
	fl_color(fillcolor);
	fl_newpath();
	fl_vertex(TAB_SPACING, height+TAB_HEIGHT);
	fl_vertex(TAB_SPACING, height+TAB_SPACING);
	fl_vertex(x2, height+TAB_SPACING);
	fl_vertex(x2+TAB_SLOPE, height+TAB_HEIGHT+sel);
	fl_fill();
	
	// dark line - 3D effect
	fl_color(fl_darker(fillcolor));
	fl_line(0, height+TAB_HEIGHT-2, TAB_SPACING, height+TAB_HEIGHT-2);
	fl_line(TAB_SPACING-1, height+TAB_HEIGHT-2, TAB_SPACING-1, height+TAB_SPACING-1);
	fl_line(TAB_SPACING-1, height+TAB_SPACING-1, x2+1, height+TAB_SPACING-1);
	fl_line(x2+1, height+TAB_SPACING-1, x2+TAB_SLOPE+2, height+TAB_HEIGHT-2); 
	fl_line(x2+TAB_SLOPE, height+TAB_HEIGHT-2, w()-1, height+TAB_HEIGHT-2);


	// bounding lines - white
	fl_color(FL_WHITE);
	fl_line(0, height+TAB_HEIGHT-1, TAB_SPACING, height+TAB_HEIGHT-1);
	fl_line(TAB_SPACING, height+TAB_HEIGHT-1, TAB_SPACING, height+TAB_SPACING);
	fl_line(TAB_SPACING, height+TAB_SPACING, x2, height+TAB_SPACING);
	fl_line(x2, height+TAB_SPACING, x2+TAB_SLOPE, height+TAB_HEIGHT-1); 
	fl_line(x2+TAB_SLOPE, height+TAB_HEIGHT-1, w()-1, height+TAB_HEIGHT-1);
	
	// draw label
	int x = TAB_SLOPE + TAB_SPACING*2;
	int y = height + TAB_SPACING*2;
	int w = toplabelwidth;
	int h = TAB_HEIGHT - TAB_SPACING*2;
	o->draw_label(x, y, w, h, FL_ALIGN_LEFT);

	return height+TAB_HEIGHT;
}


// Draw blank box. This should be called only when there are no children
void VerticalTabs::drawblank() {
	if (color() == FL_INVALID_COLOR) fl_color(parent()->color()); else fl_color(color());
	fl_newpath();
	fl_vertex(0, h());
	fl_vertex(w(), h());
	fl_vertex(w(), 0);
	fl_vertex(0, 0);
	fl_fill();
}


void VerticalTabs::draw() {
  
	int height = 0;
	bool isanyactive = false;
	int numchildren = children();

	if (damage() & FL_DAMAGE_ALL) { // redraw the entire thing:
		fl_push_clip(0,0,this->w(),this->h());
		draw_group_box();
		fl_pop_clip();
	}

	for (int i=0; i<numchildren; i++) {
		if (active_child == child(i)) {
			height = drawtab(i,height,active_child,true);
			active_child->show();
			draw_child(*active_child);
			height += active_child->h();
			isanyactive = true;
		} else {
			child(i)->hide();
			height = drawtab(i,height,child(i),false);
		}
	}

	if (!isanyactive) {
		if (numchildren>0) {
			active_child = child(0);
			draw();
		} else {
			drawblank();
		}
	}
}


void VerticalTabs::layout() {
	int numchildren = children();
	int childheight = h();
	toplabelwidth = 0;

	if (h() <= TAB_HEIGHT*numchildren) {
		// TODO: more tabs than available space -> scrollbar
	} else {
		childheight = h() - TAB_HEIGHT*numchildren;
	}

	for (int i=0; i<numchildren; i++) {
		child(i)->resize(0,TAB_HEIGHT*(i+1),w(),childheight);
		child(i)->layout();
		int wt = 0, ht = 0;
		child(i)->measure_label(wt,ht);
		if (toplabelwidth < wt) toplabelwidth = wt;
	}

	if ((active_child == 0) && (numchildren>0)) active_child = child(0);
}


VerticalTabs::VerticalTabs(int X,int Y,int W, int H, const char *l)
  : Fl_Group(X,Y,W,H,l)
{
//  style(default_style);
//  focus_index(0);
}
