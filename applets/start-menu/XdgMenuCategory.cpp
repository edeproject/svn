#include <string.h>
#include <stdlib.h>
#include <edelib/Debug.h>
#include "XdgMenuCategory.h"

struct CategoryTable {
	const char *name;
	int        related;
};

/* a static list of known main categories */
static CategoryTable ctable[] = {
	{ "Application",     XDG_MENU_CATEGORY_APPLICATION },
	{ "AudioVideo",      XDG_MENU_CATEGORY_AUDIO_VIDEO },
	{ "Audio",           XDG_MENU_CATEGORY_AUDIO_VIDEO },
	{ "Video",           XDG_MENU_CATEGORY_AUDIO_VIDEO },
	{ "Development",     XDG_MENU_CATEGORY_DEVELOPMENT },
	{ "Education",       XDG_MENU_CATEGORY_EDUCATION },
	{ "Game",            XDG_MENU_CATEGORY_GAME },
	{ "Graphics",        XDG_MENU_CATEGORY_GRAPHICS },
	{ "Network",         XDG_MENU_CATEGORY_NETWORK },
	{ "Office",          XDG_MENU_CATEGORY_OFFICE },
	{ "Settings",        XDG_MENU_CATEGORY_SETTINGS },
	{ "System",          XDG_MENU_CATEGORY_SYSTEM },
	{ "Utility",         XDG_MENU_CATEGORY_UTILITY }
};

#define CTABLE_SIZE (sizeof(ctable)/sizeof(ctable[0]))

int xdg_deduce_category(const char *cat) {
	E_ASSERT(cat != NULL);
	int ret = 0;
	char *c = strdup(cat);

	for(char *tok = strtok(c, ";"); tok; tok = strtok(NULL, ";")) {
		for(unsigned int i = 0; i < CTABLE_SIZE; i++) {
			if(strcmp(tok, ctable[i].name) == 0)
				ret &=  ctable[i].related;
		}
	}

	free(c);
	return ret;
}
