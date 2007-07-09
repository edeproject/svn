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

#include "prefs.h"
#include "evolume.h"
#include "../common/aboutdialog.h"
#include <edeconf.h>

char 	  	device[1024]={0};
Fl_Window 	*main_window=0;

void get_device_info(int mixer_dev, Fl_Slider *sl, Fl_Slider *bal, Fl_Check_Button *ck, int device);
void set_device(int mixer_fd, int device, Fl_Slider *device_sl, Fl_Slider *balance);
void set_mute(int mixer_fd, int device, Fl_Slider *device_sl, Fl_Slider *balance, Fl_Check_Button *check_button);
void set_rec(int mixer_fd, int device, Fl_Check_Button *ck);

void default_look(Fl_Slider* slider) 
{
    slider->type(Fl_Slider::VERTICAL|Fl_Slider::TICK_BOTH);
    slider->minimum(1); 
    slider->maximum(200);
    slider->value(1);
    slider->step(1); 
    slider->align(FL_ALIGN_TOP);
}

void default_look_b(Fl_Slider* balance_slider) 
{
    balance_slider->type(1); 
    balance_slider->minimum(0.00);
    balance_slider->maximum(1.00); 
    balance_slider->step(0.01);
    balance_slider->value(0.01);
}

void quit_cb(Fl_Widget*, void*) 
{
    main_window->hide();
}

void cb_volume(Fl_Widget*, long x) 
{
    if (x == 1) set_device(mixer_device, SOUND_MIXER_VOLUME, volume_slider, volume_balance);
    if (x == 2) set_device(mixer_device, SOUND_MIXER_VOLUME, volume_slider, volume_balance);
    if (x == 3) set_mute(mixer_device, SOUND_MIXER_VOLUME, volume_slider, volume_balance, volume_mute);
    if (x == 4) set_rec(mixer_device, SOUND_MIXER_VOLUME, volume_rec);
}

void cb_cd(Fl_Widget*, long x) 
{
    if (x == 1) set_device(mixer_device, SOUND_MIXER_CD, cd_slider, cd_balance);
    if (x == 2) set_device(mixer_device, SOUND_MIXER_CD, cd_slider, cd_balance);
    if (x == 3) set_mute(mixer_device, SOUND_MIXER_CD, cd_slider, cd_balance, cd_mute);
    if (x == 4) set_rec(mixer_device, SOUND_MIXER_CD, cd_rec);
}

void cb_pcm(Fl_Widget*, long x) 
{
    if (x == 1) set_device(mixer_device, SOUND_MIXER_PCM, pcm_slider, pcm_balance);
    if (x == 2) set_device(mixer_device, SOUND_MIXER_PCM, pcm_slider, pcm_balance);
    if (x == 3) set_mute(mixer_device, SOUND_MIXER_PCM, pcm_slider, pcm_balance, pcm_mute);
    if (x == 4) set_rec(mixer_device, SOUND_MIXER_PCM, pcm_rec);
}

void cb_synth(Fl_Widget*, long x) 
{
    if (x == 1) set_device(mixer_device, SOUND_MIXER_SYNTH, synth_slider, synth_balance);
    if (x == 2) set_device(mixer_device, SOUND_MIXER_SYNTH, synth_slider, synth_balance);
    if (x == 3) set_mute(mixer_device, SOUND_MIXER_SYNTH, synth_slider, synth_balance, synth_mute);
    if (x == 4) set_rec(mixer_device, SOUND_MIXER_SYNTH, synth_rec);
}

void cb_line(Fl_Widget*, long x) 
{
    if (x == 1) set_device(mixer_device, SOUND_MIXER_LINE, line_slider, line_balance);
    if (x == 2) set_device(mixer_device, SOUND_MIXER_LINE, line_slider, line_balance);
    if (x == 3) set_mute(mixer_device, SOUND_MIXER_LINE, line_slider, line_balance, line_mute);
    if (x == 4) set_rec(mixer_device, SOUND_MIXER_LINE, line_rec);
}

void cb_bass(Fl_Widget*, long x) 
{
    if (x == 1) set_device(mixer_device, SOUND_MIXER_BASS, bass_slider, bass_balance);
    if (x == 2) set_device(mixer_device, SOUND_MIXER_BASS, bass_slider, bass_balance);
    if (x == 3) set_mute(mixer_device, SOUND_MIXER_BASS, bass_slider, bass_balance, bass_mute);
    if (x == 4) set_rec(mixer_device, SOUND_MIXER_BASS, bass_rec);
}

void cb_treble(Fl_Widget*, long x) 
{
    if (x == 1) set_device(mixer_device, SOUND_MIXER_TREBLE, treble_slider, treble_balance);
    if (x == 2) set_device(mixer_device, SOUND_MIXER_TREBLE, treble_slider, treble_balance);
    if (x == 3) set_mute(mixer_device, SOUND_MIXER_TREBLE, treble_slider, treble_balance, treble_mute);
    if (x == 4) set_rec(mixer_device, SOUND_MIXER_TREBLE, treble_rec);
}

void cb_mic(Fl_Widget*, long x) 
{
    if (x == 1) set_device(mixer_device, SOUND_MIXER_MIC, mic_slider, mic_balance);
    if (x == 2) set_device(mixer_device, SOUND_MIXER_MIC, mic_slider, mic_balance);
    if (x == 3) set_mute(mixer_device, SOUND_MIXER_MIC, mic_slider, mic_balance, mic_mute);
    if (x == 4) set_rec(mixer_device, SOUND_MIXER_MIC, mic_rec);
}

void cb_speaker(Fl_Widget*, long x) 
{
    if (x == 1) set_device(mixer_device, SOUND_MIXER_SPEAKER, speaker_slider, speaker_balance);
    if (x == 2) set_device(mixer_device, SOUND_MIXER_SPEAKER, speaker_slider, speaker_balance);
    if (x == 3) set_mute(mixer_device, SOUND_MIXER_SPEAKER, speaker_slider, speaker_balance, speaker_mute);
    if (x == 4) set_rec(mixer_device, SOUND_MIXER_SPEAKER, speaker_rec);
}

void cb_imix(Fl_Widget*, long x) 
{
    if (x == 1) set_device(mixer_device, SOUND_MIXER_IMIX, imix_slider, imix_balance);
    if (x == 2) set_device(mixer_device, SOUND_MIXER_IMIX, imix_slider, imix_balance);
    if (x == 3) set_mute(mixer_device, SOUND_MIXER_IMIX, imix_slider, imix_balance, imix_mute);
    if (x == 4) set_rec(mixer_device, SOUND_MIXER_IMIX, imix_rec);
}

void cb_igain(Fl_Widget*, long x) 
{
    if (x == 1) set_device(mixer_device, SOUND_MIXER_IGAIN, igain_slider, igain_balance);
    if (x == 2) set_device(mixer_device, SOUND_MIXER_IGAIN, igain_slider, igain_balance);
    if (x == 3) set_mute(mixer_device, SOUND_MIXER_IGAIN, igain_slider, igain_balance, igain_mute);
    if (x == 4) set_rec(mixer_device, SOUND_MIXER_IGAIN, igain_rec);
}

void cb_ogain(Fl_Widget*, long x) 
{
    if (x == 1) set_device(mixer_device, SOUND_MIXER_OGAIN, ogain_slider, ogain_balance);
    if (x == 2) set_device(mixer_device, SOUND_MIXER_OGAIN, ogain_slider, ogain_balance);
    if (x == 3) set_mute(mixer_device, SOUND_MIXER_OGAIN, ogain_slider, ogain_balance, ogain_mute);
    if (x == 4) set_rec(mixer_device, SOUND_MIXER_OGAIN, ogain_rec);
}

void set_device(int mixer_fd, int device, Fl_Slider *device_sl, Fl_Slider *balance)
{
    int l = (unsigned int) ((1.0-(balance->value()) ) * device_sl->value() );	
    int r = (unsigned int) ( (balance->value()) * device_sl->value());
    int v = (r << 8) | l;
    if (ioctl (mixer_fd, MIXER_WRITE (device), &v) < 0)    
	fl_alert(_("Cannot setup device, sorry."));
}

void get_device_info(int mixer_dev, Fl_Slider *sl, Fl_Slider *bal, 
		     Fl_Check_Button *ck, int device)
{
    unsigned int devmask, recmask, recsrc, stereo;
    volume real_volume;
    
    real_volume.left = real_volume.right = 0;
    devmask = recmask = recsrc = stereo = 0;
    
    if (ioctl(mixer_dev, SOUND_MIXER_READ_DEVMASK, &devmask) == -1) 
	fprintf(stderr, "Read devmask failed.\n");
    if (devmask & (1 << (device))) 
	sl->activate();
    else 
	sl->deactivate();

    if (ioctl(mixer_dev, SOUND_MIXER_READ_STEREODEVS, &stereo) == -1) 
	fprintf(stderr, "Read recsrc failed.\n"); 
    if ( stereo & (1 << (device) ) ) bal->activate();		    
    else ck->deactivate();		    
		    
    if (ioctl(mixer_dev, SOUND_MIXER_READ_RECMASK, &recmask) == -1) 
	fprintf(stderr, "Read recmask failed.\n");
    if ( recmask & (1 << (device) ) ) ck->activate();		    
    else ck->deactivate();
    
    if (ioctl(mixer_dev, SOUND_MIXER_READ_RECSRC, &recsrc) == -1) 
	fprintf(stderr, "Read recsrc failed.\n"); 
    if ( recsrc & (1 << (device) ) ) ck->set();		    
    else ck->clear();		    
    
    if ( ioctl(mixer_dev, MIXER_READ(device), &real_volume) < 0 ) {
        fprintf(stderr, "Can't obtain current volume settings.\n");  
    }

    float volume = real_volume.left + real_volume.right;

    float balance = 0; 
    balance = ( (1.0 * (unsigned char)real_volume.right ) / 
		(1.0 * ((unsigned char)real_volume.left + (unsigned char)real_volume.right)) );

    if (volume == 0)
	volume=1;
    if (balance < 0)
	balance=0.5;
    sl->value(volume);
    bal->value(balance);
}

void set_mute(int mixer_fd, int device, Fl_Slider *device_sl, Fl_Slider *balance, Fl_Check_Button *check_button)
{
    int vol = 0;
    
    if ( check_button->value() ) 
    {
	if (ioctl(mixer_fd, MIXER_WRITE(device), &vol) < 0 ) 
	 fprintf(stderr, "Cannot set mute.\n");  
    }
    else {
	volume real_volume;
        double old_volume = device_sl->value();
        double old_balance = balance->value();
        real_volume.left  = (unsigned char) ( (1.0 - (old_balance)) * old_volume );	
        real_volume.right = (unsigned char) ( (old_balance) * old_volume);
        if ( ioctl(mixer_fd, MIXER_WRITE(device), &real_volume) < 0 )
        {
    	    fprintf(stderr, "Cannot setup volume, sorry.\n");
	}
    }
}

void set_rec(int mixer_fd, int device, Fl_Check_Button *ck)
{
    unsigned int recsrc;
    
    if (ioctl(mixer_fd, SOUND_MIXER_READ_RECSRC, &recsrc) == -1) 
	printf("read recsrc failed\n");
    unsigned int new_recsrc = recsrc ^ ( 1 << device );
    
    if (ioctl(mixer_fd, SOUND_MIXER_WRITE_RECSRC, &new_recsrc) == -1) 
	printf("oh no\n");				    
}

static void cb_About(Fl_Item*, void*) {
  AboutDialog("Volume Control","1.0","");
}

void update_info()
{
    mixer_info minfo;

    if (ioctl(mixer_device, SOUND_MIXER_INFO, &minfo) < 0)    
	fprintf(stderr, "Read device info failed.\n");
    else  
    {
    	char *title = fl_strdup_printf(_("Volume control: [%s]"), minfo.name);
	main_window->label(title);	
    }
}


int main (int argc, char **argv) 
{
    fl_init_locale_support("evolume", PREFIX"/share/locale");

    Fl_Config globalConfig("EDE Team", "evolume");
    globalConfig.get("Sound mixer", "Device", device, "/dev/mixer", sizeof(device));

    main_window = new Fl_Window(720, 205, _("Volume control"));
    
    Fl_Menu_Bar *vc_menubar = new Fl_Menu_Bar(0, 0, 724, 25);
    vc_menubar->begin();
    
    Fl_Item_Group file(_("&File"));
          Fl_Item* pref_item = new Fl_Item(_("Preferences"));
                   pref_item->shortcut(FL_CTRL+'p');
		   pref_item->callback(PreferencesDialog);
    
          Fl_Item* quit_item = new Fl_Item(_("Quit"));
                   quit_item->shortcut(FL_CTRL+'q');
		   quit_item->callback(quit_cb);
		   
    file.end();

    Fl_Item_Group help(_("&Help"));
	  Fl_Item* about_item = new Fl_Item(_("About"));
                   about_item->shortcut(FL_CTRL+'a');
		   about_item->callback((Fl_Callback*)cb_About);
    help.end();
    vc_menubar->end();

    new Fl_Divider(0, 24, 724, 3);

    volume_slider = new Fl_Slider(20, 50, 20, 80, "VOL");
                default_look(volume_slider);
        volume_balance = new Fl_Slider(10, 135, 40, 15, "Balance");
		default_look_b(volume_balance);
        volume_mute = new Fl_Check_Button(5, 165, 20, 20, "Mute");
		volume_mute->align(FL_ALIGN_BOTTOM);
        volume_rec = new Fl_Check_Button(35, 165, 20, 20, "Rec");
    	        volume_rec->align(FL_ALIGN_BOTTOM);
		    
    cd_slider = new Fl_Slider(80, 50, 20, 80, "CD");
                default_look(cd_slider);
	cd_balance = new Fl_Slider(70, 135, 40, 15, "Balance");
		default_look_b(cd_balance);
	cd_mute = new Fl_Check_Button(65, 165, 20, 20, "Mute");
		cd_mute->align(FL_ALIGN_BOTTOM);
	cd_rec = new Fl_Check_Button(95, 165, 20, 20, "Rec");
	        cd_rec->align(FL_ALIGN_BOTTOM);
     
    pcm_slider = new Fl_Slider(140, 50, 20, 80, "PCM");
	         default_look(pcm_slider);
    pcm_balance = new Fl_Slider(130, 135, 40, 15, "Balance");
		default_look_b(pcm_balance);
    pcm_mute = new Fl_Check_Button(125, 165, 20, 20, "Mute");
      pcm_mute->align(FL_ALIGN_BOTTOM);
    pcm_rec = new Fl_Check_Button(155, 165, 20, 20, "Rec");
     pcm_rec->align(FL_ALIGN_BOTTOM);
      
    synth_slider = new Fl_Slider(200, 50, 20, 80, "SYNTH");
                   default_look(synth_slider);
    synth_balance = new Fl_Slider(190, 135, 40, 15, "Balance");
		   default_look_b(synth_balance);
    synth_mute = new Fl_Check_Button(185, 165, 20, 20, "Mute");
                   synth_mute->align(FL_ALIGN_BOTTOM);
    synth_rec = new Fl_Check_Button(215, 165, 20, 20, "Rec");
                   synth_rec->align(FL_ALIGN_BOTTOM);
   
    line_slider = new Fl_Slider(260, 50, 20, 80, "LINE");
                  default_look(line_slider);
    line_balance = new Fl_Slider(250, 135, 40, 15, "Balance");
		default_look_b(line_balance);
    line_mute = new Fl_Check_Button(245, 165, 20, 20, "Mute");
                   line_mute->align(FL_ALIGN_BOTTOM);
    line_rec = new Fl_Check_Button(275, 165, 20, 20, "Rec");
                   line_rec->align(FL_ALIGN_BOTTOM);
		  
    bass_slider = new Fl_Slider(320, 50, 20, 80, "BASS");
                  default_look(bass_slider);
    bass_balance = new Fl_Slider(310, 135, 40, 15, "Balance");
		  default_look_b(bass_balance);
    bass_mute = new Fl_Check_Button(305, 165, 20, 20, "Mute");
                  bass_mute->align(FL_ALIGN_BOTTOM);
    bass_rec = new Fl_Check_Button(335, 165, 20, 20, "Rec");
                  bass_rec->align(FL_ALIGN_BOTTOM);
      
    treble_slider = new Fl_Slider(380, 50, 20, 80, "TREBLE");
                    default_look(treble_slider);
    treble_balance = new Fl_Slider(370, 135, 40, 15, "Balance");
                  default_look_b(treble_balance);
    treble_mute = new Fl_Check_Button(365, 165, 20, 20, "Mute");
      treble_mute->align(FL_ALIGN_BOTTOM);
    treble_rec = new Fl_Check_Button(395, 165, 20, 20, "Rec");
      treble_rec->align(FL_ALIGN_BOTTOM);
		    
    mic_slider = new Fl_Slider(440, 50, 20, 80, "MIC");
                 default_look(mic_slider);
    mic_balance = new Fl_Slider(430, 135, 40, 15, "Balance");
		default_look_b(mic_balance);
    mic_mute = new Fl_Check_Button(425, 165, 20, 20, "Mute");
                 mic_mute->align(FL_ALIGN_BOTTOM);
    mic_rec = new Fl_Check_Button(455, 165, 20, 20, "Rec");
                 mic_rec->align(FL_ALIGN_BOTTOM);
		 
    speaker_slider = new Fl_Slider(500, 50, 20, 80, "SPK");
                     default_look(speaker_slider);
    speaker_balance = new Fl_Slider(490, 135, 40, 15, "Balance");
         	     default_look_b(speaker_balance);
    speaker_mute = new Fl_Check_Button(485, 165, 20, 20, "Mute");
                     speaker_mute->align(FL_ALIGN_BOTTOM);
    speaker_rec = new Fl_Check_Button(515, 165, 20, 20, "Rec");
                speaker_rec->align(FL_ALIGN_BOTTOM);
     
    imix_slider = new Fl_Slider(560, 50, 20, 80, "IMIX");
	          default_look(imix_slider);
    imix_balance = new Fl_Slider(550, 135, 40, 15, "Balance");
                  default_look_b(imix_balance);
    imix_mute = new Fl_Check_Button(545, 165, 20, 20, "Mute");
                  imix_mute->align(FL_ALIGN_BOTTOM);
    imix_rec = new Fl_Check_Button(575, 165, 20, 20, "Rec");
                  imix_rec->align(FL_ALIGN_BOTTOM);
    
    igain_slider = new Fl_Slider(620, 50, 20, 80, "IGAIN");
    	           default_look(igain_slider);
    igain_balance = new Fl_Slider(610, 135, 40, 15, "Balance");
    		   default_look_b(igain_balance);
    igain_mute = new Fl_Check_Button(605, 165, 20, 20, "Mute");
                   igain_mute->align(FL_ALIGN_BOTTOM);
    igain_rec = new Fl_Check_Button(635, 165, 20, 20, "Rec");
                   igain_rec->align(FL_ALIGN_BOTTOM);
    
    ogain_slider = new Fl_Slider(680, 50, 20, 80, "OGAIN");
    	           default_look(ogain_slider);
    ogain_balance = new Fl_Slider(670, 135, 40, 15, "Balance");
		   default_look_b(ogain_balance);
    ogain_mute = new Fl_Check_Button(665, 165, 20, 20, "Mute");
	           ogain_mute->align(FL_ALIGN_BOTTOM);
    ogain_rec = new Fl_Check_Button(695, 165, 20, 20, "Rec");
		   ogain_rec->align(FL_ALIGN_BOTTOM);
		   
    mixer_device = open(device, O_RDWR);
    
    if (mixer_device == -1) 
    { 
    	    fl_alert(_("Opening mixer device %s failed. Setup correct device in configuration dialog."), device);
	    volume_slider->deactivate(); cd_slider->deactivate();
	    pcm_slider->deactivate(); synth_slider->deactivate();
	    line_slider->deactivate(); bass_slider->deactivate();
	    treble_slider->deactivate(); mic_slider->deactivate();
    	    speaker_slider->deactivate(); imix_slider->deactivate();
	    igain_slider->deactivate(); ogain_slider->deactivate();
    }

    update_info();
    
    volume_slider->callback( cb_volume, 1 );
    volume_balance->callback( cb_volume, 2 );    
    volume_mute->callback( cb_volume, 3 );    
    volume_rec->callback( cb_volume, 4 );
    get_device_info(mixer_device, volume_slider, volume_balance, volume_rec, SOUND_MIXER_VOLUME);    
    
    cd_slider->callback(  cb_cd,  1 );
    cd_balance->callback(  cb_cd, 2 );    
    cd_mute->callback(  cb_cd, 3 );    
    cd_rec->callback(  cb_cd, 4 );
    get_device_info(mixer_device, cd_slider, cd_balance, cd_rec, SOUND_MIXER_CD);

    pcm_slider->callback(  cb_pcm,  1 );
    pcm_balance->callback(  cb_pcm, 2 );    
    pcm_mute->callback(  cb_pcm, 3 );    
    pcm_rec->callback(  cb_pcm, 4 );
    get_device_info(mixer_device, pcm_slider, pcm_balance, pcm_rec, SOUND_MIXER_PCM);

    synth_slider->callback(  cb_synth,  1 );
    synth_balance->callback(  cb_synth, 2 );    
    synth_mute->callback(  cb_synth, 3 );    
    synth_rec->callback(  cb_synth, 4 );
    get_device_info(mixer_device, synth_slider, synth_balance, synth_rec, SOUND_MIXER_SYNTH);    
    
    line_slider->callback(  cb_line,  1 );
    line_balance->callback(  cb_line, 2 );    
    line_mute->callback(  cb_line, 3 );    
    line_rec->callback(  cb_line, 4 );
    get_device_info(mixer_device, line_slider, line_balance, line_rec, SOUND_MIXER_LINE);    
    
    bass_slider->callback(  cb_bass,  1 );
    bass_balance->callback(  cb_bass, 2 );    
    bass_mute->callback(  cb_bass, 3 );    
    bass_rec->callback(  cb_bass, 4 );
    get_device_info(mixer_device, bass_slider, bass_balance, bass_rec, SOUND_MIXER_BASS);    
     
    treble_slider->callback(  cb_treble,  1 );
    treble_balance->callback(  cb_treble, 2 );    
    treble_mute->callback(  cb_treble, 3 );    
    treble_rec->callback(  cb_treble, 4 );
    get_device_info(mixer_device, treble_slider, treble_balance, treble_rec, SOUND_MIXER_TREBLE);    
    
    mic_slider->callback(  cb_mic,  1 );
    mic_balance->callback(  cb_mic, 2 );    
    mic_mute->callback(  cb_mic, 3 );    
    mic_rec->callback(  cb_mic, 4 );
    get_device_info(mixer_device, mic_slider, mic_balance, mic_rec, SOUND_MIXER_MIC);    
     
    speaker_slider->callback(  cb_speaker,  1 );
    speaker_balance->callback(  cb_speaker, 2 );    
    speaker_mute->callback(  cb_speaker, 3 );    
    speaker_rec->callback(  cb_speaker, 4 );
    get_device_info(mixer_device, speaker_slider, speaker_balance, speaker_rec, SOUND_MIXER_SPEAKER);    
     
    imix_slider->callback(  cb_imix,  1 );
    imix_balance->callback(  cb_imix, 2 );    
    imix_mute->callback(  cb_imix, 3 );    
    imix_rec->callback(  cb_imix, 4 );
    get_device_info(mixer_device, imix_slider, imix_balance, imix_rec, SOUND_MIXER_IMIX);    
    
    igain_slider->callback(  cb_igain,  1 );
    igain_balance->callback(  cb_igain, 2 );    
    igain_mute->callback(  cb_igain, 3 );    
    igain_rec->callback(  cb_igain, 4 );
    get_device_info(mixer_device, igain_slider, igain_balance, igain_rec, SOUND_MIXER_IGAIN);    
	       
    ogain_slider->callback(  cb_ogain,  1 );
    ogain_balance->callback(  cb_ogain, 2 );    
    ogain_mute->callback(  cb_ogain, 3 );    
    ogain_rec->callback(  cb_ogain, 4 );
    get_device_info(mixer_device, ogain_slider, ogain_balance, ogain_rec, SOUND_MIXER_OGAIN);    

    main_window->end();
    main_window->show(argc, argv);

    return Fl::run();
}

