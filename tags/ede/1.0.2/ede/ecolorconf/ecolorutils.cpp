// Colors settings for EDE
// Copyright (C) 2000-2002 Martin Pekar
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include <unistd.h>
#include <stdlib.h>

#include <efltk/fl_draw.h>
#include <efltk/x.h>
#include <efltk/Fl_Util.h>
#include <efltk/Fl_String.h>

#include "ecolorutils.h"
#include "ecolorconf.h"

static void sendClientMessage(Window w, Atom a, long x)
{
  XEvent ev;
  long mask;

  memset(&ev, 0, sizeof(ev));
  ev.xclient.type = ClientMessage;
  ev.xclient.window = w;
  ev.xclient.message_type = a;
  ev.xclient.format = 32;
  ev.xclient.data.l[0] = x;
  ev.xclient.data.l[1] = CurrentTime;
  mask = 0L;
  if (w == RootWindow(fl_display, fl_screen))
    mask = SubstructureRedirectMask;	   
  XSendEvent(fl_display, w, False, mask, &ev);
}

void sendUpdateInfo(Atom what)
{
    unsigned int i, nrootwins;
    Window dw1, dw2, *rootwins = 0;
    int screen_count = ScreenCount(fl_display);
    for (int s = 0; s < screen_count; s++) {
        Window root = RootWindow(fl_display, s);
        XQueryTree(fl_display, root, &dw1, &dw2, &rootwins, &nrootwins);
        for (i = 0; i < nrootwins; i++) {
            if (rootwins[i]!=RootWindow(fl_display, fl_screen)) {
                sendClientMessage(rootwins[i], what, 0);
            }
        }
    }
}
/*
void updateSizes(Fl_Input_Browser *font_sizes)
{
    int *sizes;
    int cnt = fl_font()->sizes(sizes);

    font_sizes->clear();
    font_sizes->begin();

    char tmp[8];
    for(int n=0; n<cnt; n++)
    {
        snprintf(tmp, sizeof(tmp)-1, "%d", sizes[n]);
        Fl_Item *i = new Fl_Item();
        i->copy_label(tmp);
    }
    font_sizes->end();
}

void updateEncodings(Fl_Input_Browser *font_combo)
{
    int encs;
    const char **array;
    encs = fl_font()->encodings(array);

    fontEncoding->clear();
    fontEncoding->begin();
    for(int n=0; n<encs; n++)
    {
        new Fl_Item(array[n]);
    }
    fontEncoding->end();
}

void updateFontChange(Fl_Input_Browser *font_combo, Fl_Input_Browser *font_sizes)
{
    //Fl_Font f = fl_find_font(font_combo->value());
    Fl_Font f = fl_create_font(font_combo->value());    
    int s = (int)atoi(font_sizes->value());

    if(!f) return;

    fl_font(f,s);

    updateSizes(font_sizes);
    updateEncodings(font_combo);
}

void updateFontAll()
{
    updateFontChange(labelFontInput, labelSize);
    updateFontChange(textFontInput, textSize);
}*/


void apply_colors_apps(Fl_Color fg, Fl_Color bg, Fl_Color text, Fl_String font)
{
    uchar r, g, b, r1, g1, b1, r2, g2, b2;
    fl_get_color(bg, r, g, b);
    fl_get_color(fg, r1, g1, b1);
    fl_get_color(text, r2, g2, b2);

    Fl_String filePath(fl_homedir()); 
    filePath += "/.Xdefaults";
    
    char *backgroundTypes[34] = 
    {
    "*XmList.background" ,    "*XmLGrid.background",
    "Netscape*XmList.background" ,   "Netscape*XmLGrid.background",
    "*text*background",   "*list*background",
    "*Text*background",   "*List*background", 
    "*textBackground",   "*XmTextField.background", 
    "*XmText.background",     "Netscape*XmTextField.background", 
    "Netscape*XmText.background",     "*background", 
    "*Background",  "nscal*Background",
    "*Menu*background",     "OpenWindows*WindowColor",
    "Window.Color.Background",   "netscape*background",
    "Netscape*background",   ".netscape*background",
    "Ddd*background",   "Emacs*Background",
    "Emacs*backgroundToolBarColor",//25 
    "*XmList.selectBackground" ,   "*XmLGrid.selectBackground",
    "Netscape*XmList.selectBackground" ,  "Netscape*XmLGrid.selectBackground",
    "*XmTextField.selectBackground",  "*XmText.selectBackground", 
    "Netscape*XmTextField.selectBackground",  "Netscape*XmText.selectBackground", 
    "*selectBackground" //34
		   
    };	

    FILE *colorFile = fopen(filePath, "w");
    for (int i = 0 ; i < 34; i++)
    {
        fprintf(colorFile, "%s:  #%02X%02X%02X\n", backgroundTypes[i],(short int) r, (short int) g, (short int) b);
    }	
    fprintf(colorFile, "foreground:  #%02X%02X%02X\n", r1, g1, b1);
    fprintf(colorFile, "xterm*background:  #FFFFFF\n");	//especialy for Xterm
    fclose(colorFile);

    Fl_String runString("xrdb -merge -all ");
    runString += fl_homedir(); 
    runString += "/.Xdefaults";
    
    if (fl_start_child_process(runString)==-1)
	fl_alert("Error executing xrdb program.");
}


void apply_colors_gtk(Fl_Color fg, 
		      Fl_Color bg, 
		      Fl_Color selection, 
		      Fl_Color selection_text, 
		      Fl_Color tooltip, 
		      Fl_Color tooltip_text, 
		      
		      Fl_Color text, 		      
		      Fl_String font)
{
    uchar r, g, b;
    uchar text_r, text_g, text_b;
    //, b1, r2, g2, b2;
    
    uchar selection_r, selection_g, selection_b;
    uchar selection_text_r, selection_text_g, selection_text_b;
    uchar tooltip_r, tooltip_g, tooltip_b;
    uchar tooltip_text_r, tooltip_text_g, tooltip_text_b;
    
    fl_get_color(bg, r, g, b);
    fl_get_color(fg, text_r, text_g, text_b);

    fl_get_color(selection, selection_r, selection_g, selection_b);
    fl_get_color(selection_text, selection_text_r, selection_text_g, selection_text_b);
    fl_get_color(tooltip, tooltip_r, tooltip_g, tooltip_b);
    fl_get_color(tooltip_text, tooltip_text_r, tooltip_text_g, tooltip_text_b);
    
//    fl_get_color(text, r2, g2, b2);

    Fl_String filePath(fl_homedir()); 
    filePath += "/.gtkrc";

    FILE *gtkFile = fopen(filePath.c_str(), "w");
    
    fprintf(gtkFile, "style \"default\" \n");
    fprintf(gtkFile, "{\n");
    fprintf(gtkFile, "fontset = \"%s\" \n", font.c_str());
    fprintf(gtkFile, "bg[NORMAL] = \"#%02X%02X%02X\"\n", r, g, b);
    fprintf(gtkFile, "fg[NORMAL] = \"#%02X%02X%02X\"\n", text_r, text_g, text_b);
    fprintf(gtkFile, "bg[PRELIGHT] = \"#%02X%02X%02X\"\n", r, g, b);
    fprintf(gtkFile, "fg[PRELIGHT] = \"#%02X%02X%02X\"\n", text_r, text_g, text_b);
    fprintf(gtkFile, "bg[ACTIVE] = \"#%02X%02X%02X\"\n", r, g, b);
    fprintf(gtkFile, "fg[ACTIVE] = \"#%02X%02X%02X\"\n", text_r, text_g, text_b);
    fprintf(gtkFile, "bg[SELECTED] = \"#%02X%02X%02X\"\n", selection_r, selection_g, selection_b);
    fprintf(gtkFile, "fg[SELECTED] = \"#%02X%02X%02X\"\n", selection_text_r, selection_text_g, selection_text_b);
    fprintf(gtkFile, "}\n");
    
    fprintf(gtkFile, "style \"menu\" \n");
    fprintf(gtkFile, "{\n");
    fprintf(gtkFile, "bg[PRELIGHT] = \"#%02X%02X%02X\"\n", selection_r, selection_g, selection_b);
    fprintf(gtkFile, "fg[PRELIGHT] = \"#%02X%02X%02X\"\n", selection_text_r, selection_text_g, selection_text_b);
    fprintf(gtkFile, "}\n");

    fprintf(gtkFile, "style \"tooltip\" \n");
    fprintf(gtkFile, "{\n");
    fprintf(gtkFile, "bg[NORMAL] = \"#%02X%02X%02X\"\n", tooltip_r, tooltip_g, tooltip_b);
    fprintf(gtkFile, "fg[NORMAL] = \"#%02X%02X%02X\"\n", tooltip_text_r, tooltip_text_g, tooltip_text_b);
    fprintf(gtkFile, "}\n");
    
    fprintf(gtkFile, "class \"*\" style \"default\"\n");
    fprintf(gtkFile, "widget_class \"*Menu*\" style \"menu\"  \n");
    fprintf(gtkFile, "widget \"gtk-tooltips\" style \"tooltip\"  \n");
    
    
    fclose(gtkFile);
}


void apply_colors_qt(Fl_Color fg, Fl_Color bg, Fl_Color text, Fl_String font)
{
    uchar r, g, b, r1, g1, b1, r2, g2, b2;
    fl_get_color(bg, r, g, b);
    fl_get_color(fg, r1, g1, b1);
    fl_get_color(text, r2, g2, b2);

    Fl_String filePath(fl_homedir()); 
    filePath += "/.qt/qtrc";

    FILE *qtfile = fopen(filePath.c_str(), "w");
    
    fprintf(qtfile, "[General]\n");
    fprintf(qtfile, "GUIEffects=none^e\n");
    fprintf(qtfile, "style=Windows\n\n");
    fprintf(qtfile, "[Palette]\n");
    fprintf(qtfile, "active=#000000^e#%02x%02x%02x^e#ffffff^e#%02x%02x%02x^e#000000^e"
	    "#%02x%02x%02x^e#000000^e#ffffff^e#000000^e#ffffff^e#%02x%02x%02x^e#000000^e"
	    "#7783bd^e#ffffff^e#0000ff^e#ff00ff^e\n",
	    r,g,b, r,g,b, r,g,b, r,g,b);
    fprintf(qtfile, "disabled=#808080^e#%02x%02x%02x^e#ffffff^e#f2f2f2^e#%02x%02x%02x^e"
	    "#b7b7b7^e#b7b7b7^e#ffffff^e#000000^e#ffffff^e#dcdcdc^e#000000^e"
	    "#000080^e#ffffff^e#0000ff^e#ff00ff^e\n", 
	    r,g,b, r,g,b);
    fprintf(qtfile, "inactive=#000000^e#%02x%02x%02x^e#ffffff^e#f2f2f2^e#%02x%02x%02x^e"
		    "#b7b7b7^e#000000^e#ffffff^e#000000^e#ffffff^e#dcdcdc^e"
		    "#000000^e#7783bd^e#ffffff^e#0000ff^e#ff00ff^e\n",
		    r,g,b, r,g,b);

    fclose(qtfile);
}


void apply_colors_kde(Fl_Color fg, Fl_Color bg, Fl_Color text, Fl_String font)
{
    uchar r, g, b, r1, g1, b1, r2, g2, b2;
    fl_get_color(bg, r, g, b);
    fl_get_color(fg, r1, g1, b1);
    fl_get_color(text, r2, g2, b2);

    Fl_String filePath(fl_homedir()); 
    filePath += "/.kderc";

    FILE *kdefile = fopen(filePath.c_str(), "w");
    
    fprintf(kdefile, "[General]\n");
    fprintf(kdefile, "background=%d,%d,%d\n", r, g, b);
    fprintf(kdefile, "foreground=%d,%d,%d\n", r1, g1, b1);
    

    fclose(kdefile);
}

void saveScheme(char *scheme)
{
    char *keys[] = 
    {
        "color", "label color", "selection color",
        "selection text color", "highlight color", "off color", "text color",
        "highlight label color",
    };
    Fl_Button *colorBoxes[8] = 
    {
        colorBox, labelColorBox, selectionColorBox, selectionTextColorBox,
        highlightColorBox, offColorBox, textColorBox, highlightLabelColorBox
    };

    if (schemeListBox->size() > 1) 
    {
        if (colorBox->color() == labelColorBox->color()) 
	{	fl_alert(_("Color and label color are the same. Edit colors first."));
	}
        else 
	{
            Fl_Config colorConfig(scheme); //save to "active".scheme

            colorConfig.set_section("widgets/default");
            for (int boxIndex=0; boxIndex<8; boxIndex++) {
                colorConfig.write(keys[boxIndex], (int)colorBoxes[boxIndex]->color());
            }

            colorConfig.write("text background", (int)textBackgroundBox->color());
            colorConfig.write("label font", labelFontInput->value());
	    colorConfig.write("text font", textFontInput->value());
            colorConfig.write("label size", labelSize->value());
            colorConfig.write("text size",  textSize->value());
	    colorConfig.write("font encoding",  fontEncoding->value());

            colorConfig.set_section("widgets/tooltip");
	    colorConfig.write("color", (int)tooltipBox->color());
            colorConfig.write("label color", (int)tooltipTextColorButton->color());

            colorConfig.set_section("global colors");
            colorConfig.write("background", (int)backgroundBox->color());
        }
    }
}

void saveActiveScheme()
{
    Fl_String pathActive(fl_homedir()); 
    pathActive += "/.ede/schemes/Active.scheme";

    saveScheme(pathActive);
}

void saveSchemeAs()
{
    const char *schemeName = fl_input(_("Save scheme as:"), _("New scheme"));
    if (schemeName) 
    {
	Fl_String pathScheme; 
	pathScheme.printf("%s/.ede/schemes/%s.scheme", fl_homedir().c_str(), schemeName);
	saveScheme(pathScheme);
	schemeListBox->add(fl_file_filename(pathScheme));
    }	
}

void applyColors() 
{
    sendUpdateInfo(FLTKChangeScheme);

    if (allApplyRadioButton->value()==1)
    {
	apply_colors_apps(labelColorBox->color(), backgroundBox->color(), 
		    textBackgroundBox->color(), labelFontInput->value());
	apply_colors_gtk(labelColorBox->color(), backgroundBox->color(), 
			 selectionColorBox->color(), selectionTextColorBox->color(),
			 tooltipBox->color(), tooltipTextColorButton->color(),
			 textBackgroundBox->color(), labelFontInput->value()
	    );
	apply_colors_qt(labelColorBox->color(), backgroundBox->color(), 
	    textBackgroundBox->color(), labelFontInput->value());
	apply_colors_kde(labelColorBox->color(), backgroundBox->color(), 
	    textBackgroundBox->color(), labelFontInput->value());
    }
}

void fillItems() 
{
    char *file;

    Fl_String path(fl_homedir()); 
    path += "/.ede/schemes";

    dirent **files;
    int count = fl_filename_list(path, &files);

    if (count > 0)
    {
        new Fl_Item("Active");
	schemeListBox->value("Active");


        for(int n=0; n<count; n++)
        {
            file = files[n]->d_name;
            if( strcmp(file, ".") && strcmp(file, ".."))
            {
		char *filename = fl_strdup_printf("%s/%s", (char*)path, file);
                if (!fl_is_dir(filename) &&
                    fl_file_match(file, "*.scheme") && strcmp(file, "Active.scheme")!=0) 
		{
                    new Fl_Item(strdup(fl_file_filename(filename)));
                }
		delete [] filename;
            }
            free(files[n]);
        }
        free(files);
	getSchemeColors(); //we apply first scheme - active.scheme
    }

}

void getSchemeColors()
{
    char tr[128];
    int ir = 0;
    char *keys[] = 
    {  
        "color", "label color", "selection color",
        "selection text color", "highlight color", "off color", "text color",
        "highlight label color",
    };
    Fl_Button *colorBoxes[8] = 
    {
        colorBox, labelColorBox, selectionColorBox, selectionTextColorBox,
        highlightColorBox, offColorBox, textColorBox, highlightLabelColorBox
    };

    if (schemeListBox->size() > 1)
    {
        Fl_Config *colorConfig;

        const char *ai = schemeListBox->value();
        if (strcmp(ai, "Active")==0)
        {
            char pathActive[FL_PATH_MAX];
            snprintf(pathActive, sizeof(pathActive)-1, "%s/.ede/schemes/Active.scheme", getenv("HOME"));
            colorConfig = new Fl_Config(pathActive);
        } else {
            char pathScheme[FL_PATH_MAX];
            snprintf(pathScheme, sizeof(pathScheme)-1, "%s/.ede/schemes/%s", getenv("HOME"), ai);
            colorConfig = new Fl_Config(pathScheme);
        }

        for(int boxIndex = 0; boxIndex < 8; boxIndex++)
	{
            colorConfig->set_section("widgets/default");
            if(!colorConfig->read(keys[boxIndex], ir)) {
                colorBoxes[boxIndex]->color((Fl_Color)ir);
                colorBoxes[boxIndex]->highlight_color((Fl_Color)ir);
	    }    
        }

        colorConfig->set_section("widgets/tooltip");
        if(!colorConfig->read("color", ir)) {
            tooltipBox->color((Fl_Color)ir);
            tooltipBox->highlight_color((Fl_Color)ir);
        }
        if(!colorConfig->read("label color",ir)) {
            tooltipTextColorButton->color((Fl_Color)ir);
            tooltipTextColorButton->highlight_color((Fl_Color)ir);
        }

        colorConfig->set_section("widgets/default");
        if (!colorConfig->read("text background", ir)) {
            textBackgroundBox->color((Fl_Color)ir);
            textBackgroundBox->highlight_color((Fl_Color)ir);
        }
        if(!colorConfig->read("label font", tr, 0, sizeof(tr))) { labelFontInput->value(tr); }

        if(!colorConfig->read("text font", tr, 0, sizeof(tr))) { textFontInput->value(tr); }

        if(!colorConfig->read("font encoding", tr, 0, sizeof(tr))) { fontEncoding->value(tr); }

        if(!colorConfig->read("label size", tr, 0, sizeof(tr))) { labelSize->value(tr); }

        if(!colorConfig->read("text size", tr, 0, sizeof(tr))) { textSize->value(tr); }

        colorConfig->set_section("global colors");
        if(!colorConfig->read("background", ir)) backgroundBox->color((Fl_Color)ir);

        colorBox->parent()->parent()->redraw();

        delete colorConfig;
    }
}

void loadEfltkConfig()
{
    char *file = 0;
    file = Fl_Config::find_config_file("efltk.conf", false, Fl_Config::USER);
    if(!file) file = Fl_Config::find_config_file("efltk.conf", false, Fl_Config::SYSTEM);

    Fl_Config cfg(file, true, false);
    if(!cfg.error()) 
    {
        bool b_val;
        float f_val;
        int i_val;

        // Read Fl_Image defaults:
        cfg.get("Images", "State Effects", b_val, true);
        imagesStateEffect->value(b_val);

        // Read Fl_Menu_Window defaults:
        cfg.get("Menus", "Effects", b_val, true);
        menusEnableEffects->value(b_val);
        cfg.get("Menus", "Subwindow Effect", b_val, true);
        menusEnableSubwindowEffects->value(b_val);
        cfg.get("Menus", "Effect Type", i_val, 1);
        menusEffectType->value(i_val);
        cfg.get("Menus", "Speed", f_val, 1.5f);
        menusSpeed->value(f_val);
        cfg.get("Menus", "Delay", f_val, 0.2f);
        menusDelay->value(f_val);


        // Read Fl_Tooltip defaults:
        cfg.get("Tooltips", "Effects", b_val, true);
        tooltipsEnableEffects->value(b_val);
        cfg.get("Tooltips", "Effect Type", i_val, 2);
        tooltipsEffectType->value(i_val);
        cfg.get("Tooltips", "Enabled", b_val, true);
        tooltipsEnable->value(b_val);
        cfg.get("Tooltips", "Delay", f_val, 1.0f);
        tooltipsDelay->value(f_val);

        // Read Fl_MDI_Window defaults:
        cfg.get("MDI", "Animate", b_val, true);
        mdiAnimation->value(b_val);
        cfg.get("MDI", "Opaque", b_val, false);
        mdiOpaqueAnimation->value(b_val);
    } 
}    

void saveEfltkConfig()
{
    char *file = 0;
    file = Fl_Config::find_config_file("efltk.conf", false, Fl_Config::USER);
    if(!file) file = Fl_Config::find_config_file("efltk.conf", false, Fl_Config::SYSTEM);

    Fl_Config cfg(file, true, true);
    if(!cfg.error()) 
    {
        cfg.set("Images", "State Effects", imagesStateEffect->value());
	
        cfg.set("Menus", "Effects", menusEnableEffects->value());
        cfg.set("Menus", "Subwindow Effect", menusEnableSubwindowEffects->value());
        cfg.set("Menus", "Effect Type", menusEffectType->value());
        cfg.set("Menus", "Speed", (float)menusSpeed->value());
        cfg.set("Menus", "Delay", (float)menusDelay->value());

        cfg.set("Tooltips", "Effects", tooltipsEnableEffects->value());
        cfg.set("Tooltips", "Effect Type", tooltipsEffectType->value());
        cfg.set("Tooltips", "Enabled", tooltipsEnable->value());
        cfg.set("Tooltips", "Delay", (float)tooltipsDelay->value());

        cfg.set("MDI", "Animate", mdiAnimation->value());
        cfg.set("MDI", "Opaque", mdiOpaqueAnimation->value());

        sendUpdateInfo(FLTKChangeSettings);
    }
}

