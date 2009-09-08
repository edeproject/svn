#ifndef __XDG_MENU_CATEGORY_H__
#define __XDG_MENU_CATEGORY_H__

/* Known XDG main categories; see spec for the list.
 *
 * 'Application' (XDG_MENU_CATEGORY_APPLICATION) is not listed in spec's section about main categories, but
 * app .desktop files often has it, probably to deduce it from setting app or something.
 */
enum { 
	XDG_MENU_CATEGORY_UNKNOWN     = 0, /* not in the spec, but this will be returned if we fail */
	XDG_MENU_CATEGORY_APPLICATION = (1 << 0),
	XDG_MENU_CATEGORY_AUDIO_VIDEO = (1 << 1),
	XDG_MENU_CATEGORY_DEVELOPMENT = (1 << 2),
	XDG_MENU_CATEGORY_EDUCATION   = (1 << 3),
	XDG_MENU_CATEGORY_GAME        = (1 << 4),
	XDG_MENU_CATEGORY_GRAPHICS    = (1 << 5),
	XDG_MENU_CATEGORY_NETWORK     = (1 << 6),
	XDG_MENU_CATEGORY_OFFICE      = (1 << 7),
	XDG_MENU_CATEGORY_SETTINGS    = (1 << 8),
	XDG_MENU_CATEGORY_SYSTEM      = (1 << 9),
	XDG_MENU_CATEGORY_UTILITY     = (1 << 10)
};

int xdg_deduce_category(const char *cat);

#endif
