#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include <edelib/TiXml.h>
#include <edelib/Debug.h>
#include <edelib/String.h>
#include <edelib/StrUtil.h>
#include <edelib/List.h>
#include <edelib/Util.h>
#include <edelib/FileTest.h>
#include <edelib/Directory.h>

#include "DesktopEntry.h"

EDELIB_NS_USING(String)
EDELIB_NS_USING(list)
EDELIB_NS_USING(system_config_dirs)
EDELIB_NS_USING(system_data_dirs)
EDELIB_NS_USING(build_filename)
EDELIB_NS_USING(file_test)
EDELIB_NS_USING(str_ends)
EDELIB_NS_USING(FILE_TEST_IS_DIR)

#define DOT_OR_DOTDOT(base)   (base[0] == '.' && (base[1] == '\0' || (base[1] == '.' && base[2] == '\0')))
#define ELEMENT_IS(elem, val) (strcmp(elem->Value(), val) == 0)

struct MenuParseContext;

typedef list<MenuParseContext*> MenuParseList;
typedef list<MenuParseContext*>::iterator MenuParseListIt;

typedef list<String> StrList;
typedef list<String>::iterator StrListIt;

struct MenuParseContext {
	/* menu tag content */
	String *name;

	/* a stack of .directory names; the top one is used */
	StrList dir_files;

	/* directories where to find .directory file */
	StrList dir_dirs;

	/* a list of desktop files */
	DesktopEntryList desk_files;

	/* nested menus */
	MenuParseList submenus;
};

static MenuParseContext *menu_parse_context_new(void) {
	MenuParseContext *m = new MenuParseContext;
	m->name = NULL;
	return m;
}

static void menu_parse_context_delete(MenuParseContext *m) {
	E_RETURN_IF_FAIL(m != NULL);

	delete m->name;

	/* recurse for nested menus */
	if(!m->submenus.empty()) {
		MenuParseListIt it = m->submenus.begin(), it_end = m->submenus.end();

		while(it != it_end) {
			menu_parse_context_delete(*it);
			it = m->submenus.erase(it);
		}
	}

	delete m;
}

static void menu_parse_context_append_default_dir_dirs(MenuParseContext *ctx) {
	StrList lst;

	int ret = system_data_dirs(lst);
	if(ret < 1)
		return;

	StrListIt it = lst.begin(), it_end = lst.end();

	for(; it != it_end; ++it)
		ctx->dir_dirs.push_back(build_filename((*it).c_str(), "desktop-directories"));
}

static void menu_parse_context_append_desktop_files(MenuParseContext *ctx, const char *dir, const char *basedir) {
	DIR *ds = opendir(dir);
	if(!ds)
		return;

	dirent       *dp;
	DesktopEntry *entry;

	while((dp = readdir(ds)) != NULL) {
		/* skip dots and (possibly) hidden files */
		if(dp->d_name[0] == '.' || DOT_OR_DOTDOT(dp->d_name))
			continue;

		entry = new DesktopEntry;
		entry->assign_path(dir, dp->d_name, basedir);

		if(file_test(entry->get_path(), FILE_TEST_IS_DIR)) {
			/* recurse if needed; the spec said we must */
			menu_parse_context_append_desktop_files(ctx, entry->get_path(), basedir);

			/* delete it */
			delete entry;
			continue;
		}

		/* files must ends with this extension */
		if(str_ends(entry->get_path(), ".desktop")) {
			ctx->desk_files.push_back(entry);
		} else {
			/* clear non .desktop items */
			delete entry;
		}
	}

	closedir(ds);
}

static void menu_parse_context_append_desktop_files_from_xdg_data_dirs(MenuParseContext *ctx) {
	StrList lst;
	if(system_data_dirs(lst) < 1)
		return;

	StrListIt it = lst.begin(), it_end = lst.end();
	String tmp;

	for(; it != it_end; ++it) {
		tmp = build_filename((*it).c_str(), "applications");
		menu_parse_context_append_desktop_files(ctx, tmp.c_str(), tmp.c_str());
	}
}

static void scan_menu_tag(TiXmlNode *elem, MenuParseList &parse_list) {
	E_RETURN_IF_FAIL(elem != NULL);

	TiXmlText *txt;
	bool got_default_app_dirs = false, got_default_dir_dirs = false;

	MenuParseContext *ctx = menu_parse_context_new();

	for(elem = elem->FirstChildElement(); elem; elem = elem->NextSibling()) {
		/* in case we got '<Menu>' as submenu, dive in it recursively */
		if(ELEMENT_IS(elem, "Menu"))
			scan_menu_tag(elem, parse_list);

		if(ELEMENT_IS(elem, "Name")) {
			txt = elem->FirstChild()->ToText();
			if(txt && !ctx->name)
				ctx->name = new String(txt->Value());

			continue;
		}
		
		if(ELEMENT_IS(elem, "Directory")) {
			txt = elem->FirstChild()->ToText();
			if(txt) {
				/* push it at the top */
				ctx->dir_files.push_front(txt->Value());
			}

			continue;
		} 
		
		if(ELEMENT_IS(elem, "AppDir")) {
			txt = elem->FirstChild()->ToText();
			if(txt)
				menu_parse_context_append_desktop_files(ctx, txt->Value(), NULL);

			continue;
		} 
		
		if(ELEMENT_IS(elem, "DirectoryDir")) {
			txt = elem->FirstChild()->ToText();
			if(txt) {
				/* push it at the top */
				ctx->dir_dirs.push_front(txt->Value());
			}

			continue;
		}

		/* spec: '<DefaultAppDirs>' expands to $XDG_DATA_DIRS/applications */
		if(ELEMENT_IS(elem, "DefaultAppDirs")) {
			if(!got_default_app_dirs) {
				menu_parse_context_append_desktop_files_from_xdg_data_dirs(ctx);
				/* scan it only once */
				got_default_app_dirs = true;
			}

			continue;
		}

		if(ELEMENT_IS(elem, "DefaultDirectoryDirs")) {
			if(!got_default_dir_dirs) {
				menu_parse_context_append_default_dir_dirs(ctx);
				got_default_dir_dirs = true;
			}

			continue;
		}
	}

	parse_list.push_back(ctx);
}

static void print_list(StrList &s) {
	StrListIt it = s.begin(), it_end = s.end();
	while(it != it_end) {
		E_DEBUG(" %s\n", (*it).c_str());
		++it;
	}
}

void xdg_menu_load(void) {
	TiXmlDocument doc;
	//if(!doc.LoadFile("applets/start-menu/applications.menu")) {
	//if(!doc.LoadFile("applications.menu")) {
	if(!doc.LoadFile("/etc/xdg/menu/xfce-applications.menu")) {
	//if(!doc.LoadFile("/etc/xfce/xdg/menus/xfce-applications.menu")) {
	//if(!doc.LoadFile("/etc/kde/xdg/menus/applications.menu")) {
		E_WARNING(E_STRLOC ": Can't load menu\n");
		return;
	}

	TiXmlNode *elem = doc.FirstChild("Menu");
	if(!elem) {
		E_WARNING(E_STRLOC ": No 'Menu' tag\n");
		return;
	}

	MenuParseList parse_list;
	scan_menu_tag(elem, parse_list);

	MenuParseListIt first = parse_list.begin(), last = parse_list.end();
	MenuParseContext *ctx;

	while(first != last) {
		ctx = *first;
		E_DEBUG("Menu: %s\n", ctx->name->c_str());

		/* dump duplicate desktop id's */
		desktop_entry_remove_duplicates(ctx->desk_files);

		/* load desktop entries */
		desktop_entry_load_all(ctx->desk_files);

		/* cleanup desktop entries */
		DesktopEntryListIt df = ctx->desk_files.begin(), de = ctx->desk_files.end();
		while(df != de) {
			//E_DEBUG("%i %s => %s\n", (*df)->get_age(), (*df)->get_path(), (*df)->get_id());
			delete *df;
			df = ctx->desk_files.erase(df);
		}

		menu_parse_context_delete(*first);
		first = parse_list.erase(first);
	}
}

int main() {
	xdg_menu_load();
	return 0;
}
