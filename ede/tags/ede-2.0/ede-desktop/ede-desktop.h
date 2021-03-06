/*
 * $Id$
 *
 * ede-desktop, desktop and icon manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2006-2009 EDE Authors.
 *
 * This program is licensed under terms of the 
 * GNU General Public License version 2 or newer.
 * See COPYING for details.
 */

#ifndef __EDE_DESKTOP_H__
#define __EDE_DESKTOP_H__

#ifdef USE_EDELIB_WINDOW
# include <edelib/Window.h>
# define EDE_DESKTOP_WINDOW edelib::Window
#else
# include <FL/Fl_Window.H>
# include <FL/Fl_Double_Window.H>
# define EDE_DESKTOP_WINDOW Fl_Window
#endif

#include <FL/Fl_Image.H>

#include <edelib/String.h>
#include <edelib/Resource.h>
#include <edelib/List.h>
#include <edelib/EdbusConnection.h>
#include <edelib/MenuButton.h>

#define EDAMAGE_CHILD_LABEL    0x10
#define EDAMAGE_OVERLAY        0x20

#define ICON_NORMAL  1     // .desktop file
#define ICON_TRASH   2     // trash.desktop (specific since have two icons for empty/full)
#define ICON_FILE    3     // real file
#define ICON_SYMLINK 4     // symbolic link

#define ICON_FACE_ONE 1    // use icon
#define ICON_FACE_TWO 2    // use icon2

struct GlobalIconSettings {
	int  label_background;
	int  label_foreground;
	int  label_font;
	int  label_fontsize;
	int  label_maxwidth;
	bool label_transparent;
	bool label_draw;
	bool one_click_exec;
	bool auto_arrange;
	bool same_size;
};

/*
 * Settings representing related to icon on desktop. To complicate our lives
 * (and to, at least, simplify code) it can be: 
 *  - .desktop file content (normal or trash like)
 *  - real file copied/moved inside ~/Desktop directory
 *  - symlink in ~/Desktop directory pointing to the real file
 */
struct IconSettings {
	int  x, y;
    int  type;                // ICON_NORMAL, ICON_TRASH,...
    bool cmd_is_url;          // interpret cmd as url, like system:/,trash:/,$HOME
	
	edelib::String name;
	edelib::String cmd;
	edelib::String icon;
	edelib::String icon2;     // for stateable icons, like trash (empty/full)
	edelib::String key_name;  // name used as key when storing positions
	edelib::String full_path; // for tracking changes
};

/* selection overlay */
struct SelectionOverlay {
	int x, y, w, h;
	bool show;
};

class Wallpaper;
class DesktopIcon;

typedef edelib::list<DesktopIcon*>           DesktopIconList;
typedef edelib::list<DesktopIcon*>::iterator DesktopIconListIter;

typedef edelib::list<edelib::String>           StringList;
typedef edelib::list<edelib::String>::iterator StringListIter;

class Desktop : public EDE_DESKTOP_WINDOW {
private:
	static Desktop* pinstance;

	int selection_x, selection_y;

	/* last recorded pointer position, so icon can be created at position where menu is clicked */
	int last_px, last_py;

	bool moving;
	bool do_dirwatch;

	edelib::String desktop_dir_path;

	SelectionOverlay*  selbox;

	GlobalIconSettings* gisett;

	edelib::MenuButton*      dmenu;
	Wallpaper*               wallpaper;
	edelib::EdbusConnection* dbus;

	DesktopIconList icons;
	DesktopIconList selectionbuf;

	void init_internals(void);

	void load_icons(const char* path);
	void save_icons_positions(void);
	IconSettings* read_desktop_file(const char* path);

	void add_icon(DesktopIcon* ic);

	DesktopIcon* find_icon_by_path(const char* path, DesktopIconListIter* pos);
	bool remove_icon_by_path(const char* path);

	void auto_arrange(void);

	void unfocus_all(void);

	void select(DesktopIcon* ic, bool do_redraw = true);
	void select_only(DesktopIcon* ic);
	bool in_selection(const DesktopIcon* ic);
	void move_selection(int x, int y, bool apply);

	void select_in_area(void);

	//void dnd_drop_source(const char* src, int src_len, int x, int y);

	DesktopIcon* below_mouse(int px, int py);

public:
	Desktop();
	~Desktop();

	virtual void show(void);
	virtual void hide(void);
	virtual void draw(void);
	virtual int handle(int event);

	static void init(void);
	static void shutdown(void);
	static Desktop* instance(void);

	void read_config(void);

	bool add_icon_by_path(const char* path, edelib::Resource* conf);
	bool remove_icon(DesktopIcon *d, bool real_delete);
	bool rename_icon(DesktopIcon *d, const char *n);

	void update_workarea(void);
	void area(int& X, int& Y, int& W, int& H) { X = x(); Y = y(); W = w(); H = h(); }

	void notify_desktop_changed(void);

	void dir_watch(const char* dir, const char* changed, int flags);
	void dir_watch_on(void) { do_dirwatch = true; }
	void dir_watch_off(void) { do_dirwatch = false; }

	const char *desktop_path(void) { return desktop_dir_path.c_str(); }
};

#endif
