#ifndef __DESKTOPENTRY_H__
#define __DESKTOPENTRY_H__

#include <edelib/String.h>
#include <edelib/Debug.h>
#include <edelib/List.h>

EDELIB_NS_USING(String)
EDELIB_NS_USING(list)

class DesktopEntry;

typedef list<DesktopEntry*> DesktopEntryList;
typedef list<DesktopEntry*>::iterator DesktopEntryListIt;

/* Represents entry for a menu. Do not confuse it with DesktopFile from edelib */
class DesktopEntry {
private:
	/* used to load "later" found .desktop file, in case duplicate desktop file id's was found */
	unsigned int age;

	/* absolute path to .desktop file */
	String *path;

	/* Desktop File Id */
	String *id;

	/* Categories value from .desktop file; filled with load() */
	String *categories;

	/* Localized Name value from .desktop file; filled with load() */
	String *name;

	/* GenericName value from .desktop file; filled with load() */
	String *generic_name;

	/* Comment value from .desktop file; filled with load() */
	String *comment;

	/* Icon value from .desktop file; filled with load() */
	String *icon;

	/* Exec value from .desktop file; filled with load() */
	String *exec;

	/* TryExec value from .desktop file; filled with load() */
	String *try_exec;

	E_DISABLE_CLASS_COPY(DesktopEntry)
public:
	DesktopEntry() : age(0), path(NULL), id(NULL),
	categories(NULL), name(NULL), generic_name(NULL), comment(NULL), icon(NULL), exec(NULL), try_exec(NULL) { }

	~DesktopEntry();

	/*
	 * Construct full path by using 'dir/p' and construct desktop file id according
	 * to the menu spec. If 'basedir' was given (can be NULL), 'dir' will be diffed against it
	 * so correct prefix for desktop file id can be calculated.
	 *
	 * The id is calculated like this:
	 *   $XDG_DATA_DIRS/applications/foo.desktop => foo.desktop
	 *   $XDG_DATA_DIRS/applications/ede/foo.desktop => ede-foo.desktop
	 *   $XDG_DATA_DIRS/applications/ede/menu/foo.desktop => ede-menu-foo.desktop
	 */
	void assign_path(const char *dir, const char *p , const char *basedir);

	/* loads actual .desktop file and reads it */
	bool load(void);

	const char   *get_path(void) { return path ? path->c_str() : NULL; }
	const char   *get_id(void)   { return id ? id->c_str() : NULL; }
	unsigned int  get_age(void)  { return age; }
};

/* remove duplicate items in the list, by looking at DesktopEntry id */
void desktop_entry_remove_duplicates(DesktopEntryList &lst);

/* call 'load()' on each member; if 'load()' fails, that member will be removed */
void desktop_entry_load_all(DesktopEntryList &lst);

#endif
