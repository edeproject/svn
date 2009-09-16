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
#include <edelib/DesktopFile.h>

#include "DesktopEntry.h"
#include "MenuRules.h"

EDELIB_NS_USING(String)
EDELIB_NS_USING(DesktopFile)
EDELIB_NS_USING(list)
EDELIB_NS_USING(system_config_dirs)
EDELIB_NS_USING(system_data_dirs)
EDELIB_NS_USING(build_filename)
EDELIB_NS_USING(file_test)
EDELIB_NS_USING(str_ends)
EDELIB_NS_USING(FILE_TEST_IS_DIR)
EDELIB_NS_USING(DESK_FILE_TYPE_DIRECTORY)

#define DOT_OR_DOTDOT(base)    (base[0] == '.' && (base[1] == '\0' || (base[1] == '.' && base[2] == '\0')))
#define ELEMENT_IS(elem, val)  (strcmp(elem->Value(), val) == 0)
#define ELEMENT_GET_TEXT(elem) (elem->FirstChild() ? elem->FirstChild()->ToText() : NULL)

/* max. name size */
#define NAME_BUFSZ 128

struct MenuParseContext;
struct MenuContext;

typedef list<MenuParseContext*> MenuParseList;
typedef list<MenuParseContext*>::iterator MenuParseListIt;

typedef list<MenuContext*> MenuContextList;
typedef list<MenuContext*>::iterator MenuContextListIt;

struct MenuParseContext {
	/* menu tag content */
	String *name;

	/* a stack of .directory names; the top one is used */
	StrList dir_files;

	/* directories where to find .directory file */
	StrList dir_dirs;

	/* a list of .desktop files */
	DesktopEntryList desk_files;

	/* include rules */
	MenuRulesList include_rules;

	/* exclude rules */
	MenuRulesList exclude_rules;

	/* nested menus */
	MenuParseList submenus;
};

struct MenuContext {
	/* menu label */
	String *name;

	/* a list of .desktop files; at the same time also items in menu list */
	DesktopEntryList items;

	/* nested menus */
	MenuContextList submenus;
};

static MenuParseContext *menu_parse_context_new(void) {
	MenuParseContext *m = new MenuParseContext;
	m->name = NULL;
	return m;
}

static void menu_parse_context_delete(MenuParseContext *m) {
	E_RETURN_IF_FAIL(m != NULL);

	delete m->name;

	/* delete rules */
	if(!m->include_rules.empty()) {
		MenuRulesListIt it = m->include_rules.begin(), it_end = m->include_rules.end();
		while(it != it_end) {
			menu_rules_delete(*it);
			it = m->include_rules.erase(it);
		}
	}

	if(!m->exclude_rules.empty()) {
		MenuRulesListIt it = m->exclude_rules.begin(), it_end = m->exclude_rules.end();
		while(it != it_end) {
			menu_rules_delete(*it);
			it = m->exclude_rules.erase(it);
		}
	}

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

static void scan_include_exclude_tag(TiXmlNode *elem, MenuRulesList &rules) {
	E_RETURN_IF_FAIL(elem != NULL);

	TiXmlNode *child;
	TiXmlText *txt;

	for(child = elem->FirstChildElement(); child; child = child->NextSibling()) {
		/* assure we do not include/exclude insinde include/exclude */
		if(ELEMENT_IS(child, "Include") || ELEMENT_IS(child, "Exclude")) {
			E_WARNING(E_STRLOC ": Nesting <Include> and <Exclude> tags are not supported\n");
			continue;
		}

		if(ELEMENT_IS(child, "Filename")) {
			txt = ELEMENT_GET_TEXT(child);
			menu_rules_append_rule(rules, MENU_RULES_OPERATOR_FILENAME, txt->Value());
			continue;
		}

		if(ELEMENT_IS(child, "Category")) {
			txt = ELEMENT_GET_TEXT(child);
			menu_rules_append_rule(rules, MENU_RULES_OPERATOR_CATEGORY, txt->Value());
			continue;
		}

		if(ELEMENT_IS(child, "All")) {
			menu_rules_append_rule(rules, MENU_RULES_OPERATOR_ALL, NULL);
			continue;
		}

		if(ELEMENT_IS(child, "And")) {
			MenuRules *and_rule = menu_rules_append_rule(rules, MENU_RULES_OPERATOR_AND, NULL);
			/* recurse */
			scan_include_exclude_tag(child, and_rule->subrules);
			continue;
		}

		if(ELEMENT_IS(child, "Or")) {
			MenuRules *or_rule = menu_rules_append_rule(rules, MENU_RULES_OPERATOR_OR, NULL);
			/* recurse */
			scan_include_exclude_tag(child, or_rule->subrules);
			continue;
		}

		if(ELEMENT_IS(child, "Not")) {
			MenuRules *not_rule = menu_rules_append_rule(rules, MENU_RULES_OPERATOR_NOT, NULL);
			/* recurse */
			scan_include_exclude_tag(child, not_rule->subrules);
			continue;
		}
	}
}

static void scan_menu_tag(TiXmlNode *elem, MenuParseList &parse_list) {
	E_RETURN_IF_FAIL(elem != NULL);

	TiXmlText *txt;
	bool got_default_app_dirs = false, got_default_dir_dirs = false;

	MenuParseContext *ctx = menu_parse_context_new();

	for(elem = elem->FirstChildElement(); elem; elem = elem->NextSibling()) {
		/* in case we got '<Menu>' as submenu, dive in it recursively and fill submenus */
		if(ELEMENT_IS(elem, "Menu"))
			scan_menu_tag(elem, ctx->submenus);

		if(ELEMENT_IS(elem, "Name")) {
			txt = ELEMENT_GET_TEXT(elem);
			if(txt && !ctx->name)
				ctx->name = new String(txt->Value());

			continue;
		}
		
		if(ELEMENT_IS(elem, "Directory")) {
			txt = ELEMENT_GET_TEXT(elem);
			/* entries must ends with .directory */
			if(txt && str_ends(txt->Value(), ".directory")) {
				/* push it at the top */
				ctx->dir_files.push_front(txt->Value());
			}

			continue;
		} 
		
		if(ELEMENT_IS(elem, "AppDir")) {
			txt = ELEMENT_GET_TEXT(elem);
			if(txt)
				menu_parse_context_append_desktop_files(ctx, txt->Value(), NULL);

			continue;
		} 
		
		if(ELEMENT_IS(elem, "DirectoryDir")) {
			txt = ELEMENT_GET_TEXT(elem);
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

		if(ELEMENT_IS(elem, "Include")) {
			scan_include_exclude_tag(elem, ctx->include_rules);
			continue;
		}

		if(ELEMENT_IS(elem, "Exclude")) {
			scan_include_exclude_tag(elem, ctx->exclude_rules);
			continue;
		}
	}

	parse_list.push_back(ctx);
}

static String *menu_context_construct_name(MenuParseContext *m, MenuParseContext *top) {
	E_RETURN_VAL_IF_FAIL(m != NULL, NULL);

	if(!m->dir_files.empty()) {
		/*
		 * We have two locations where are keeping directory list: node specific and top node
		 * specific, where often is put <DefaultDirectoryDirs/> tag. Here, first we will look in
		 * node specific directory list, then will go in top node <DefaultDirectoryDirs/>.
		 */
		StrListIt dir_it = m->dir_dirs.begin(), dir_it_end = m->dir_dirs.end();

		/* this list has 'stack-ed' items; the last one is on top */
		StrListIt file_it, file_it_end = m->dir_files.end();

		DesktopFile df;
		String      tmp;

		/* first try specific directory list */
		for(; dir_it != dir_it_end; ++dir_it) {
			for(file_it = m->dir_files.begin(); file_it != file_it_end; ++file_it) {
				tmp = build_filename((*dir_it).c_str(), (*file_it).c_str());
				//E_DEBUG("==> %s\n", tmp.c_str());

				/* load it and see if it is real .desktop file */
				df.load(tmp.c_str());
				if(df && df.type() == DESK_FILE_TYPE_DIRECTORY) {
					char buf[NAME_BUFSZ];

					if(df.name(buf, NAME_BUFSZ))
						return new String(buf);
					else 
						continue;
				}
			}
		}

		/* now try default ones */
		dir_it = top->dir_dirs.begin(), dir_it_end = top->dir_dirs.end();

		for(; dir_it != dir_it_end; ++dir_it) {
			for(file_it = m->dir_files.begin(); file_it != file_it_end; ++file_it) {
				tmp = build_filename((*dir_it).c_str(), (*file_it).c_str());
				//E_DEBUG("++> %s\n", tmp.c_str());

				/* load it and see if it is real .desktop file */
				df.load(tmp.c_str());
				if(df && df.type() == DESK_FILE_TYPE_DIRECTORY) {
					char buf[NAME_BUFSZ];

					if(df.name(buf, NAME_BUFSZ))
						return new String(buf);
					else 
						continue;
				}
			}
		}
	}	

	E_RETURN_VAL_IF_FAIL(m->name != NULL, NULL);
	/* if there are no files, use context name */
	return new String(*(m->name));
}

static void menu_context_apply_include_rules(MenuContext *ctx, 
											 MenuParseContext *m, 
											 MenuParseContext *top, 
											 MenuRulesList &rules)
{
	MenuRulesListIt    rit = rules.begin(), rit_end = rules.end();
	DesktopEntryListIt dit = m->desk_files.begin(), dit_end = m->desk_files.end();

	/* check rules for the current list */
	for(; dit != dit_end; ++dit) {
		for(rit = rules.begin(); rit != rit_end; ++rit) {
			/* E_DEBUG("%i %s\n", menu_rules_eval(*rit, *dit), (*dit)->get_path()); */

			/* append entry if matches to the rule, and mark it as allocated */
			if(menu_rules_eval(*rit, *dit)) {
				(*dit)->mark_as_allocated();
				ctx->items.push_back(*dit);
			}
		}
	}

	/* check rules for the top list */
	dit = top->desk_files.begin(), dit_end = top->desk_files.end();
	for(; dit != dit_end; ++dit) {
		for(rit = rules.begin(); rit != rit_end; ++rit) {
			/* E_DEBUG("%i %s\n", menu_rules_eval(*rit, *dit), (*dit)->get_path()); */

			/* append entry if matches to the rule, and mark it as allocated */
			if(menu_rules_eval(*rit, *dit)) {
				(*dit)->mark_as_allocated();
				ctx->items.push_back(*dit);
			}
		}
	}
}

static MenuContext *menu_parse_context_to_menu_context(MenuParseContext *m, MenuParseContext *top, TiXmlNode *menu_node) {
	E_RETURN_VAL_IF_FAIL(m != NULL, NULL);

	MenuContext *ctx = new MenuContext;

	/* figure out the name first */
	ctx->name = menu_context_construct_name(m, top);
	E_DEBUG("+ Menu: %s %i\n", ctx->name->c_str(), m->include_rules.size());

	menu_context_apply_include_rules(ctx, m, top, m->include_rules);

	/* process submenus */
	if(!m->submenus.empty()) {
		MenuParseListIt mit = m->submenus.begin(), mit_end = m->submenus.end();
		MenuContext *sub_ctx;

		for(; mit != mit_end; ++mit) {
			sub_ctx = menu_parse_context_to_menu_context(*mit, top, menu_node);
			ctx->submenus.push_back(sub_ctx);
		}
	}

	return ctx;
}

static void menu_context_delete(MenuContext *c) {
	delete c->name;

	if(!c->submenus.empty()) {
		MenuContextListIt it = c->submenus.begin(), it_end = c->submenus.end();
		for(; it != it_end; ++it)
			menu_context_delete(*it);
	}

	delete c;
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
		//E_DEBUG("Menu: %s\n", ctx->name->c_str());

		desktop_entry_list_remove_duplicates(ctx->desk_files);

		desktop_entry_list_load_all(ctx->desk_files);

		MenuContext *cc = menu_parse_context_to_menu_context(ctx, ctx, elem);
		menu_context_delete(cc);

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
