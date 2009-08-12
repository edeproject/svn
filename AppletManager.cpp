#include <dlfcn.h>
#include <string.h>
#include <edelib/Debug.h>

#include "AppletManager.h"
#include "Panel.h"

EDELIB_NS_USING(String)

struct AppletData {
	void             *dl;
	Fl_Widget        *awidget; /* widget from the applet */
	String            name;

	applet_create_t   create_func;
	applet_destroy_t  destroy_func;
};

static char* get_basename(const char* path) {
	char* p = strrchr(path, '/');
	if(p)
		return (p + 1);

	return (char*)path;
}

static void clear_applet(AppletData *a) {
	E_RETURN_IF_FAIL(a != NULL);

	/* clear applet specific suff */
	(a->destroy_func)(a->awidget);
	dlclose(a->dl);
	delete a;
}


AppletManager::~AppletManager() {
	clear();
}

bool AppletManager::load(const char *path) {
	void *a = dlopen(path, RTLD_LAZY);
	if(!a) {
		E_WARNING(E_STRLOC ": Unable to load '%s'\n", path);
		return false;
	}

	dlerror();
	const char *dl_err = NULL;

	/* first check if we have valid plugin by requesting version function */
	void *av = (applet_version_t*)dlsym(a, "ede_panel_applet_get_iface_version");
	if(!av) {
		dl_err = dlerror();
		E_WARNING(E_STRLOC ": Invalid ede-panel plugin. The plugin will not be loaded\n");
		return false;

		applet_version_t avf = (applet_version_t)av;
		if((avf)() > EDE_PANEL_APPLET_INTERFACE_VERSION) {
			E_WARNING(E_STRLOC ": This plugin requries newer ede-panel version\n");
			return false;
		}
	}

	void *ac = (applet_create_t*)dlsym(a, "ede_panel_applet_create");
	if(!ac) {
		dl_err = dlerror();
		E_WARNING(E_STRLOC ": Unable to find 'create' function in ede-panel plugin: '%s'\n", dl_err);
		return false;
	}

	void *ad = (applet_destroy_t*)dlsym(a, "ede_panel_applet_destroy");
	if(!ad) {
		dl_err = dlerror();
		E_WARNING(E_STRLOC ": Unable to find 'destroy' function in ede-plugin plugin: '%s'\n", dl_err);
		return false;
	}

	AppletData *data = new AppletData;
	data->dl = a;
	data->awidget = NULL;

	data->create_func = (applet_create_t)ac;
	data->destroy_func = (applet_destroy_t)ad;

	/* see it's base as name */
	data->name = get_basename(path);

	applet_list.push_back(data);
	return true;
}

void AppletManager::clear(void) {
	if(applet_list.empty())
		return;

	AListIter it = applet_list.begin(), it_end = applet_list.end();
	for(; it != it_end; ++it)
		clear_applet(*it);

	applet_list.clear();
}

/*
 * Must be called so widget can actually be added to FLTK parent.
 * Widgets will be created when this function was called
 */
void AppletManager::fill_group(Panel *p) {
	AListIter it = applet_list.begin(), it_end = applet_list.end();

	for(; it != it_end; ++it) {
		(*it)->awidget = (*it)->create_func();
		p->add((*it)->awidget);
	}
}

void AppletManager::unfill_group(Panel *p) {
	AListIter it = applet_list.begin(), it_end = applet_list.end();
	/* slow !!! */
	for(; it != it_end; ++it) 
		p->remove((*it)->awidget);
}
