#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include <edelib/TiXml.h>
#include <edelib/Debug.h>
#include <edelib/String.h>
#include <edelib/StrUtil.h>
#include <edelib/List.h>
#include <edelib/Util.h>
#include <edelib/Directory.h>
#include <edelib/Config.h>
#include <edelib/FileTest.h>

#include "XdgMenuReader.h"
#include "XdgMenuCategory.h"

EDELIB_NS_USING(String)
EDELIB_NS_USING(Config)
EDELIB_NS_USING(list)
EDELIB_NS_USING(system_data_dirs)
EDELIB_NS_USING(build_filename)
EDELIB_NS_USING(file_test)
EDELIB_NS_USING(str_ends)
EDELIB_NS_USING(FILE_TEST_IS_DIR)

#define DOT_OR_DOTDOT(base) (base[0] == '.' && (base[1] == '\0' || (base[1] == '.' && base[2] == '\0')))

struct XdgMenu;
struct MenuDesktopEntry;

typedef list<String*> StrList;
typedef list<String*>::iterator StrListIter;

typedef list<XdgMenu*> XdgMenuList;
typedef list<XdgMenu*>::iterator XdgMenuListIter;

typedef list<MenuDesktopEntry*> DesktopEntryList;
typedef list<MenuDesktopEntry*>::iterator DesktopEntryListIter;

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
	StrList merge_dirs;        /* <MergeDir> elements */

	XdgMenuList submenus;      /* submenus */
};

/* each .desktop file in <AppDir> must have own Desktop-File Id */
struct MenuDesktopEntry {
	String name;
	String desktop_file_id;
};

static void strlist_append(StrList &sl, const char *name) {
	String *s = new String(name);
	sl.push_back(s);
}

static void strlist_append_with_xdg_path(StrList &sl, const char *suffix, bool is_config) {
	list<String> lst;
	int ret;

	ret = is_config ? system_config_dirs(lst) : system_data_dirs(lst);
	if(ret < 1)
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
	strlist_delete(m->merge_dirs);

	if(!m->submenus.empty()) {
		XdgMenuListIter it = m->submenus.begin(), it_end = m->submenus.end();

		/* recursively clean submenus */
		for(; it != it_end; ++it)
			xdg_menu_delete(*it);

		m->submenus.clear();
	}

	delete m;
}

static void scan_menu_tag(TiXmlNode *elem, XdgMenuList& menus) {
	if(!elem)
		return;

	TiXmlText *txt;
	XdgMenu   *m = xdg_menu_new();

	bool got_default_app_dirs = false, got_default_dir_dirs = false, got_default_merge_dirs = false;

	for(elem = elem->FirstChildElement(); elem; elem = elem->NextSibling()) {
		/* in case we got '<Menu>' as submenu, dive in it recursively */
		if(strcmp(elem->Value(), "Menu") == 0)
			scan_menu_tag(elem, m->submenus);

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

		if(strcmp(elem->Value(), "MergeDir") == 0) {
			txt = elem->FirstChild()->ToText();
			if(txt)
				strlist_append(m->merge_dirs, txt->Value());
			continue;
		}

		/* spec: '<DefaultAppDirs>' expands to $XDG_DATA_DIRS/applications */
		if(strcmp(elem->Value(), "DefaultAppDirs") == 0) {
			if(!got_default_app_dirs) {
				strlist_append_with_xdg_path(m->app_dirs, "applications", false);
				/* scan it only once */
				got_default_app_dirs = true;
			}

			continue;
		}

		/* spec: '<DefaultDirectoryDirs>' expands to $XDG_DATA_DIRS/desktop-directories */
		if(strcmp(elem->Value(), "DefaultDirectoryDirs") == 0) {
			if(!got_default_dir_dirs) {
				strlist_append_with_xdg_path(m->dir_dirs, "desktop-directories", false);
				got_default_dir_dirs = true;
			}

			continue;
		}

		/* spec: '<DefaultMergeDirs>' expands to $XDG_CONFIG_DIRS/applications-merged */
		if(strcmp(elem->Value(), "DefaultMergeDirs") == 0) {
			if(!got_default_merge_dirs) {
				strlist_append_with_xdg_path(m->merge_dirs, "applications-merged", true);
				got_default_merge_dirs = true;
			}

			continue;
		}
	}

	menus.push_back(m);
}

static bool load_desktop_files_internal(const String *dir, DesktopEntryList &lst) {
	DIR *dirp = opendir(dir->c_str());
	if(!dirp)
		return false;

	dirent *dp;
	String path;
	bool   status = true;

	while((dp = readdir(dirp)) != NULL) {
		/* skip dots and (possibly) hidden files */
		if(dp->d_name[0] == '.' || DOT_OR_DOTDOT(dp->d_name))
			continue;

		path = build_filename(dir->c_str(), dp->d_name);

		if(file_test(path.c_str(), FILE_TEST_IS_DIR)) {
			/* recurse if needed; the spec said we must */
			status = load_desktop_files_internal(&path, lst);
			continue;
		}

		/* it must have .desktop extension */
		if(str_ends(path.c_str(), ".desktop")) {
			MenuDesktopEntry *en = new MenuDesktopEntry;
			en->name = path;
			lst.push_back(en);	
		}
	}

	closedir(dirp);
	return status;
}

static void load_desktop_files(const String *dir, DesktopEntryList &lst) {
	/* 
	 * do not process further if failed, because 'lst' will be filled with
	 * the content of previous directory
	 */
	if(!load_desktop_files_internal(dir, lst))
		return;

	const char *ptr;
	MenuDesktopEntry *en;
	DesktopEntryListIter it = lst.begin(), it_end = lst.end();
	String tmp;

	/* Now, scan the list and construct Desktop File Id and fill desktop_file_id for each member.
	 * Desktop File Id is constructed by replacing '/' with '-' in subdirectory path, e.g.:
	 *
	 *   <AppDir>/foo.desktop           => foo.desktop
	 *   <AppDir>/bla/foo.desktop       => bla-foo.desktop
	 *   <AppDir>/bla/again/foo.desktop => bla-again-foo.desktop
	 */
	for(; it != it_end; ++it) {
		en = *it;

		if(en->name.length() < dir->length()) {
			E_WARNING(E_STRLOC ": '%s' length is smaller than allowed. This entry will be skipped\n");
			continue;
		}

		/* skip directory path + starting separator */
		ptr = en->name.c_str() + dir->length();

		while(*ptr == E_DIR_SEPARATOR)
			ptr++;

		tmp.assign(ptr);
		tmp.replace(E_DIR_SEPARATOR, '-');

		en->desktop_file_id = tmp;
	}
}

void xdg_menu_load(void) {
	TiXmlDocument doc;
	//if(!doc.LoadFile("applets/start-menu/applications.menu")) {
	if(!doc.LoadFile("applications.menu")) {
		E_WARNING(E_STRLOC ": Can't load menu\n");
		return;
	}

	TiXmlNode *elem = doc.FirstChild("Menu");
	if(!elem) {
		E_WARNING(E_STRLOC ": No 'Menu' tag\n");
		return;
	}

	DesktopEntryList dlst;
	StrListIter      sit, sit_end;

	XdgMenuList menus;
	scan_menu_tag(elem, menus);

	XdgMenuListIter it = menus.begin(), it_end = menus.end();

	for(; it != it_end; ++it) {
		XdgMenu* m = (*it);

		sit = m->app_dirs.begin(), sit_end = m->app_dirs.end();
		for(; sit != sit_end; ++sit) {
			load_desktop_files(*sit, dlst);
			E_DEBUG("appdir: %s\n", (*sit)->c_str());
		}

		sit = m->dir_dirs.begin(), sit_end = m->dir_dirs.end();
		for(; sit != sit_end; ++sit)
			E_DEBUG("dirdir: %s\n", (*sit)->c_str());

		sit = m->merge_dirs.begin(), sit_end = m->merge_dirs.end();
		for(; sit != sit_end; ++sit)
			E_DEBUG("mergedir: %s\n", (*sit)->c_str());

		sit = m->directory_stack.begin(), sit_end = m->directory_stack.end();
		for(; sit != sit_end; ++sit)
			E_DEBUG("directory: %s\n", (*sit)->c_str());
	}

	DesktopEntryListIter dit = dlst.begin(), dit_end = dlst.end();
	for(; dit != dit_end; ++dit) {
		E_DEBUG("%s => %s\n", (*dit)->name.c_str(), (*dit)->desktop_file_id.c_str());
		delete *dit;
	}

	for(it = menus.begin(); it != it_end; ++it)
		xdg_menu_delete(*it);
}

int main() {
	xdg_menu_load();
	xdg_deduce_category("Application;Qt;Network;WebBrowser;X-Ximian-Main;X-Ximian-Toplevel");
	return 0;
}
