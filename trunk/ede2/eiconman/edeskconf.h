// generated by Fast Light User Interface Designer (fluid) version 2.0100

#ifndef edeskconf_h
#define edeskconf_h
/*
 * $Id$
 *
 * Desktop icons manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */
#include <fltk/Window.h>
extern void bg_prop_cb(fltk::Window*, void*);
extern fltk::Window* bg_prop_window;
#include <fltk/Group.h>
#include <fltk/InvisibleBox.h>
extern fltk::InvisibleBox* mini_image_box;
#include <fltk/LightButton.h>
extern void bg_image_color(fltk::LightButton*, void*);
extern fltk::LightButton* color_button;
#include <fltk/CheckButton.h>
extern void bg_image_use(fltk::CheckButton*, void*);
extern fltk::CheckButton* use_button;
#include <fltk/Divider.h>
extern fltk::Group* bg_image_group;
#include <fltk/Input.h>
extern void bg_image_input(fltk::Input*, void*);
extern fltk::Input* image_input;
#include <fltk/Button.h>
extern void bg_image_browse(fltk::Button*, void*);
#include <fltk/ValueSlider.h>
extern void bg_image_opacity(fltk::ValueSlider*, void*);
extern fltk::ValueSlider* opacity_slider;
#include <fltk/Choice.h>
extern void bg_image_mode(fltk::Choice*, void*);
extern fltk::Choice* mode_choice;
#include <fltk/Item.h>
extern void bg_ok(fltk::Button*, void*);
extern void bg_apply(fltk::Button*, void*);
void make_desktop_properties();
#endif