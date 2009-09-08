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
#include <edelib/FileTest.h>
#include <edelib/Config.h>
#include <edelib/DesktopFile.h>

#include "XdgMenuReader.h"
#include "XdgMenuCategory.h"

EDELIB_NS_USING(String)
EDELIB_NS_USING(Config)
EDELIB_NS_USING(DesktopFile)
EDELIB_NS_USING(list)
EDELIB_NS_USING(system_data_dirs)
EDELIB_NS_USING(build_filename)
EDELIB_NS_USING(file_test)
EDELIB_NS_USING(str_ends)
EDELIB_NS_USING(FILE_TEST_IS_DIR)

#define DOT_OR_DOTDOT(base) (base[0] == '.' && (base[1] == '\0' || (base[1] == '.' && base[2] == '\0')))

static int global_age_counter;

struct MenuNode;
struct DeskFileUndone;
struct DeskFileDone;
struct XdgMenu;

typedef list<String*> PStrList;
typedef list<String*>::iterator PStrListIter;

typedef list<MenuNode*> MenuNodeList;
typedef list<MenuNode*>::iterator MenuNodeListIter;

typedef list<DeskFileUndone*> UndoneList;
typedef list<DeskFileUndone*>::iterator UndoneListIter;

typedef list<DeskFileDone*> DoneList;
typedef list<DeskFileDone*>::iterator DoneListIter;

typedef list<XdgMenu*> XdgMenuList;
typedef list<XdgMenu*>::iterator XdgMenuListIter;


/* contains data from <Menu> tag */
struct MenuNode {
	String *name;               /* menu name */

	/* 
	 * For .directory file(s). The spec allows multiple '<Directory>' tags for easier merging and the last
	 * one should be used only if is valid file; if not, use one before and so on. Duplicate entries are
	 * allowed, and last one is used too.
	 */
	PStrList directory_stack;      

	PStrList app_dirs;          /* <AppDirs> elements */
	PStrList dir_dirs;          /* <DirectoryDirs> elements */
	PStrList merge_dirs;        /* <MergeDir> elements */

	MenuNodeList submenus;      /* submenus */
};

/* each .desktop file in <AppDir> must have own Desktop-File Id */
struct DeskFileUndone {
	String path;
	String id;         /* Desktop File Id */
	int    age;        /* so we know which file was found first; this increments on each new file */
};

/* .desktop file with completedly collected information */
struct DeskFileDone {
	String name;
	String gen_name;
	String icon;
	int    category;
};

/* final menu populated in Fl_Menu_Item */
struct XdgMenu {
	String name;
	String icon;

	DoneList items;

	XdgMenuList submenus;
};

static bool desk_file_indone_sorter(DeskFileUndone* const& u1, DeskFileUndone* const& u2) {
	return (u1->id < u2->id) && (u1->age < u2->age);
}

static void strlist_append(PStrList &sl, const char *name) {
	String *s = new String(name);
	sl.push_back(s);
}

static void strlist_prepend(PStrList &sl, const char *name) {
	String *s = new String(name);
	sl.push_front(s);
}

static void strlist_append_with_xdg_path(PStrList &sl, const char *suffix, bool is_config) {
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

static void strlist_delete(PStrList &sl) {
	if(sl.empty())
		return;

	PStrList::iterator it = sl.begin(), it_end = sl.end();
	for(; it != it_end; ++it)
		delete *it;
	sl.clear();
}

static MenuNode *menu_node_new(void) {
	MenuNode *m = new MenuNode;
	m->name = NULL;

	return m;
}

static void menu_node_delete(MenuNode *m) {
	delete m->name;

	strlist_delete(m->directory_stack);
	strlist_delete(m->app_dirs);
	strlist_delete(m->dir_dirs);
	strlist_delete(m->merge_dirs);

	if(!m->submenus.empty()) {
		MenuNodeListIter it = m->submenus.begin(), it_end = m->submenus.end();

		/* recursively clean submenus */
		for(; it != it_end; ++it)
			menu_node_delete(*it);

		m->submenus.clear();
	}

	delete m;
}

static XdgMenu *xdg_menu_new(void) {
	XdgMenu *m = new XdgMenu;
	return m;
}

static void xdg_menu_delete(XdgMenu *m) {
	DoneListIter dit = m->items.begin(), dit_end = m->items.end();
	while(dit != dit_end) {
		delete (*dit);
		dit = m->items.erase(dit);
	}

	if(!m->submenus.empty()) {
		XdgMenuListIter it = m->submenus.begin(), it_end = m->submenus.end();
		/* recursively clean submenus */
		for(; it != it_end; ++it)
			xdg_menu_delete(*it);

		m->submenus.clear();
	}

	delete m;
}

static void scan_menu_tag(TiXmlNode *elem, MenuNodeList &menus) {
	if(!elem)
		return;

	TiXmlText *txt;
	MenuNode  *m = menu_node_new();

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
			if(txt) {
				/* 
				 * the spec said we must use "further down" in case multiple ones are given,
				 * so we prepends each of them, getting last one at the top
				 */
				strlist_prepend(m->dir_dirs, txt->Value());
			}

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

static bool load_desktop_files_internal(const String *dir, UndoneList &lst) {
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
			DeskFileUndone *en = new DeskFileUndone;

			en->path = path;
			en->age  = global_age_counter++;
			lst.push_back(en);	
		}
	}

	closedir(dirp);
	return status;
}

static void load_desktop_files(const String *dir, UndoneList &lst) {
	/* reset age counter */
	global_age_counter = 0;

	/* 
	 * do not process further if failed, because 'lst' will be filled with
	 * the content of previous directory
	 */
	if(!load_desktop_files_internal(dir, lst))
		return;

	const char     *ptr;
	DeskFileUndone *en;
	String         tmp;
	UndoneListIter it = lst.begin(), it_end = lst.end();

	/* Now, scan the list and construct Desktop File Id and fill id for each member.
	 * Desktop File Id is constructed by replacing '/' with '-' in subdirectory path, e.g.:
	 *
	 *   <AppDir>/foo.desktop           => foo.desktop
	 *   <AppDir>/bla/foo.desktop       => bla-foo.desktop
	 *   <AppDir>/bla/again/foo.desktop => bla-again-foo.desktop
	 */
	for(; it != it_end; ++it) {
		en = *it;

		if(en->path.length() < dir->length()) {
			E_WARNING(E_STRLOC ": '%s' length is smaller than allowed. This entry will be skipped\n");
			continue;
		}

		/* skip directory path + starting separator */
		ptr = en->path.c_str() + dir->length();

		while(*ptr == E_DIR_SEPARATOR)
			ptr++;

		tmp.assign(ptr);
		tmp.replace(E_DIR_SEPARATOR, '-');

		en->id = tmp;
	}
}

static void menu_node_app_dirs_to_xdg_menu_items(MenuNode *node, XdgMenu *menu) {
	UndoneList   undone_list;

	PStrListIter it = node->app_dirs.begin(), it_end = node->app_dirs.end();

	/* load unscanned .desktop files from app dirs */
	for(; it != it_end; ++it)
		load_desktop_files(*it, undone_list);

	if(undone_list.empty())
		return;

	/* 
	 * sort them and via consecutive unique remove duplicates; things are sorted
	 * respecting id and age
	 */
	undone_list.sort(desk_file_indone_sorter);

	UndoneListIter uit = undone_list.begin(), uit_end = undone_list.end();
	UndoneListIter uit_next = uit;

	while(++uit_next != uit_end) {
		if((*uit)->id == (*uit_next)->id) {
			delete *uit_next;
			undone_list.erase(uit_next);
		} else {
			uit = uit_next;
		}

		uit_next = uit;
	}

	/* now load each of .desktop files, and fill menu items */
	DesktopFile df;
	char buf[128];

	uit = undone_list.begin();
	E_DEBUG("items: %i\n", undone_list.size());

	while(uit != uit_end) {
		df.load((*uit)->path.c_str());
		if(!df) {
			E_WARNING(E_STRLOC ": Unable to load '%s'. Skipping this file\n", (*uit)->path.c_str());
			++uit;
			continue;
		}

		DeskFileDone *d = new DeskFileDone;

		if(df.generic_name(buf, sizeof(buf)))
			d->gen_name = buf;
		if(df.name(buf, sizeof(buf)))
			d->name = buf;
		if(df.icon(buf, sizeof(buf)))
			d->icon = buf;

		/* TODO: extend edelib for this */
		if(df.get("Desktop Entry", "Category", buf, sizeof(buf)))
			d->category = xdg_deduce_category(buf);

		menu->items.push_back(d);

		/* clean it at the same time, increasing iterator */
		delete *uit;
		uit = undone_list.erase(uit);
	}

	/* now, recurse for submenus */
	if(!node->submenus.empty()) {
		MenuNodeListIter mit = node->submenus.begin(), mit_end = node->submenus.end();

		for(; mit != mit_end; ++mit) {
			XdgMenu *m = xdg_menu_new();
			menu_node_app_dirs_to_xdg_menu_items(*mit, m);

			menu->submenus.push_back(m);
		}
	}
}

void menu_node_dir_to_xdg_menu_name(MenuNode *node, XdgMenu *menu) {
	/* spec require we use last name in <Directory> tag stack */
	String *dirname = node->directory_stack.back();

	if(!str_ends(dirname->c_str(), ".directory"))
		return;

	String path;

	PStrListIter it = node->dir_dirs.begin(), it_end = node->dir_dirs.end();
	Config conf;

	for(; it != it_end; ++it) {
		path = build_filename((*it)->c_str(), dirname->c_str());
		if(conf.load(path.c_str()))
			break;
	}

	bool name_found = false;

	if(conf) {
		char buf[128];

		if(conf.get("Desktop Entry", "Icon", buf, sizeof(buf)))
			menu->icon = buf;

		if(conf.get_localized("Desktop Entry", "Name", buf, sizeof(buf))) {
			menu->name = buf;
			name_found = true;
		}
	}

	if(!name_found) {
		/* if name wasn't found, use directory name, stripping .directory extension */
		menu->name = dirname->substr(0, dirname->length() - 10);
	}

	E_DEBUG("%s\n", menu->name.c_str());
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

	MenuNodeList menus;
	scan_menu_tag(elem, menus);

	MenuNodeListIter it = menus.begin(), it_end = menus.end();

	XdgMenuList xmenus;

	for(; it != it_end; ++it) {
		XdgMenu *m = xdg_menu_new();

		menu_node_app_dirs_to_xdg_menu_items(*it, m);
		menu_node_dir_to_xdg_menu_name(*it, m);

		xmenus.push_back(m);
	}

	for(XdgMenuListIter mit = xmenus.begin(), mite = xmenus.end(); mit != mite; ++mit)
		xdg_menu_delete(*mit);

	for(it = menus.begin(); it != it_end; ++it)
		menu_node_delete(*it);
}

int main() {
	xdg_menu_load();
	xdg_deduce_category("Application;Qt;Network;WebBrowser;X-Ximian-Main;X-Ximian-Toplevel");
	return 0;
}
