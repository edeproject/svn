/*
 * $Id$
 *
 * Evolume, sound volume tool
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef EVOLUME_H_
#define EVOLUME_H_

extern "C" {
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <linux/soundcard.h>
#include <sys/ioctl.h>
}

#include <efltk/Fl.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Slider.h>
#include <efltk/Fl_Menu_Bar.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Item.h>
#include <efltk/Fl_Item_Group.h>
#include <efltk/Fl_Divider.h>
#include <efltk/Fl_Check_Button.h>
#include <efltk/x.h>
#include <efltk/fl_ask.h>
#include <efltk/Fl_Config.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Util.h>
#include <efltk/Fl_Divider.h>

typedef struct 
volume
{
    unsigned char left;
    unsigned char right;

} volume;

int mixer_device;
  
Fl_Slider *volume_slider, *cd_slider, *pcm_slider, *synth_slider,
 *line_slider, *bass_slider, *treble_slider, *mic_slider,
 *speaker_slider, *imix_slider, *igain_slider, *ogain_slider;
   
Fl_Slider *volume_balance, *cd_balance, *pcm_balance, *synth_balance,
 *line_balance, *bass_balance, *treble_balance, *mic_balance,
 *speaker_balance, *imix_balance, *igain_balance, *ogain_balance;
   
Fl_Check_Button *volume_mute, *cd_mute, *pcm_mute, *synth_mute,
 *line_mute, *bass_mute, *treble_mute, *mic_mute,
 *speaker_mute, *imix_mute, *igain_mute, *ogain_mute;

Fl_Check_Button *volume_rec, *cd_rec, *pcm_rec, *synth_rec,
 *line_rec, *bass_rec, *treble_rec, *mic_rec,
 *speaker_rec, *imix_rec, *igain_rec, *ogain_rec;

void get_device_info(int mixer_dev, Fl_Slider *sl, Fl_Slider *bal, Fl_Check_Button *ck, int device);
void set_device(int mixer_fd, int device, Fl_Slider *device_sl, Fl_Slider *balance);
void set_mute(int mixer_fd, int device, Fl_Slider *device_sl, Fl_Slider *balance, Fl_Check_Button *check_button);
void set_rec(int mixer_fd, int device, Fl_Check_Button *ck);
void update_info();

#endif

