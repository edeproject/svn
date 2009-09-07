#include <string.h>

#include <edelib/TiXml.h>
#include <edelib/Debug.h>
#include <edelib/String.h>
#include <edelib/List.h>
#include <edelib/Util.h>
#include <edelib/Directory.h>

#include "XdgMenuReader.h"

EDELIB_NS_USING(String)
EDELIB_NS_USING(list)
EDELIB_NS_USING(system_data_dirs)

struct XdgMenu;

typedef list<String*> StrList;
typedef list<String*>::iterator StrListIter;

typedef list<XdgMenu*> XdgMenuList;
typedef list<XdgMenu*>::iterator XdgMenuListIter;

struct XdgMenu {
	String *name;               /* menu name */

	/* 
	 * For .directory file(s). The spec allows multiple '<Directory>' tags for easier merging and the last
	 * one should be used only if is valid file; if not, use one before and so on. Duplicate entries are
	 * allowed, and last one is used too.
	 */
	StrList directory_stack;      

	StrList app_dirs;          /* <AppDirs> elements */
	StrList dir_dirs;          /* <DirectoryDirs> elements */
	StrList exclude_list;      /* <Exclude> elements */

	XdgMenuList submenus;      /* submenus */
};

static void strlist_append(StrList &sl, const char *name) {
	String *s = new String(name);
	sl.push_back(s);
}

static void strlist_append_xdg_data_path(StrList &sl, const char *suffix) {
	list<String> lst;
	if(system_data_dirs(lst) < 1)
		return;

	list<String>::iterator it = lst.begin(), it_end = lst.end();

	/* expand it as $XDG_DATA_DIRS/suffix */
	for(; it != it_end; ++it) {
		(*it) += E_DIR_SEPARATOR;
		(*it) += suffix;
		strlist_append(sl, (*it).c_str());
	}
}

static void strlist_delete(StrList &sl) {
	if(sl.empty())
		return;

	StrList::iterator it = sl.begin(), it_end = sl.end();
	for(; it != it_end; ++it)
		delete *it;
	sl.clear();
}

static XdgMenu *xdg_menu_new(void) {
	XdgMenu *m = new XdgMenu;
	m->name = NULL;

	return m;
}

static void xdg_menu_delete(XdgMenu *m) {
	delete m->name;

	strlist_delete(m->directory_stack);
	strlist_delete(m->app_dirs);
	strlist_delete(m->dir_dirs);
	strlist_delete(m->exclude_list);

	if(!m->submenus.empty()) {
		XdgMenuListIter it = m->submenus.begin(), it_end = m->submenus.end();

		/* recursively clean submenus */
		for(; it != it_end; ++it)
			xdg_menu_delete(*it);

		m->submenus.clear();
	}
}

static void scan_menu_tag(TiXmlNode *elem, XdgMenuList& menus) {
	if(!elem)
		return;

	TiXmlText *txt;
	XdgMenu   *m = xdg_menu_new();

	bool got_default_app_dirs = false, got_default_dir_dirs = false;

	for(elem = elem->FirstChildElement(); elem; elem = elem->NextSibling()) {
		/* in case we got '<Menu>' as submenu, dive in it recursively */
		if(strcmp(elem->Value(), "Menu") == 0)
			scan_menu_tag(elem, menus);

		if(strcmp(elem->Value(), "Name") == 0) {
			if(!m->name) {
				txt = elem->FirstChild()->ToText();
				if(txt) {
					String *s = new String(txt->Value());
					m->name = s;
				}
			}

			continue;
		}
		
		if(strcmp(elem->Value(), "Directory") == 0) {
			txt = elem->FirstChild()->ToText();
			if(txt)
				strlist_append(m->directory_stack, txt->Value());
			continue;
		} 
		
		if(strcmp(elem->Value(), "AppDir") == 0) {
			txt = elem->FirstChild()->ToText();
			if(txt)
				strlist_append(m->app_dirs, txt->Value());
			continue;
		} 
		
		if(strcmp(elem->Value(), "DirectoryDir") == 0) {
			txt = elem->FirstChild()->ToText();
			if(txt)
				strlist_append(m->dir_dirs, txt->Value());
			continue;
		}

		/* according to the spec, '<DefaultAppDirs>' expands to $XDG_DATA_DIRS/applications */
		if(strcmp(elem->Value(), "DefaultAppDirs") == 0) {
			if(!got_default_app_dirs) {
				strlist_append_xdg_data_path(m->app_dirs, "applications");
				/* scan it only once */
				got_default_app_dirs = true;
			}

			continue;
		}

		/* according to the spec, '<DefaultDirectoryDirs>' expands to $XDG_DATA_DIRS/desktop-directories */
		if(strcmp(elem->Value(), "DefaultDirectoryDirs") == 0) {
			if(!got_default_dir_dirs) {
				strlist_append_xdg_data_path(m->dir_dirs, "desktop-directories");
				got_default_dir_dirs = true;
			}

			continue;
		}
	}

	menus.push_back(m);
}

void xdg_menu_load(void) {
	TiXmlDocument doc;
	if(!doc.LoadFile("applets/start-menu/applications.menu")) {
		E_WARNING(E_STRLOC ": Can't load menu\n");
		return;
	}

	TiXmlNode *elem = doc.FirstChild("Menu");
	if(!elem) {
		E_WARNING(E_STRLOC ": No 'Menu' tag\n");
		return;
	}

	XdgMenuList menus;
	scan_menu_tag(elem, menus);

	XdgMenuListIter it = menus.begin(), it_end = menus.end();

	for(; it != it_end; ++it) {
		XdgMenu* m = (*it);
		StrListIter sit = m->app_dirs.begin(), sit_end = m->app_dirs.end();
		for(; sit != sit_end; ++sit)
			E_DEBUG("appdir: %s\n", (*sit)->c_str());

		sit = m->dir_dirs.begin(), sit_end = m->dir_dirs.end();
		for(; sit != sit_end; ++sit)
			E_DEBUG("dirdir: %s\n", (*sit)->c_str());

		sit = m->directory_stack.begin(), sit_end = m->directory_stack.end();
		for(; sit != sit_end; ++sit)
			E_DEBUG("directory: %s\n", (*sit)->c_str());
	}

	for(it = menus.begin(); it != it_end; ++it)
		xdg_menu_delete(*it);
}
