#include "Applet.h"

#include <edelib/MenuButton.h>
#include "XdgMenuReader.h"

EDELIB_NS_USING(MenuButton)

class StartMenu : public MenuButton {
private:
	MenuItem *mcontent;

public:
	StartMenu();
	~StartMenu();
};

StartMenu::StartMenu() : MenuButton(600, 0, 90, 25, "EDE"), mcontent(NULL) {
	mcontent = xdg_menu_load();

	/* skip the first item, since it often contains only one submenu */
	if(mcontent->submenu()) {
		MenuItem *mc = mcontent + 1;
		menu(mc);
	} else {
		menu(mcontent);
	}
}

StartMenu::~StartMenu() {
	xdg_menu_delete(mcontent);
}

EDE_PANEL_APPLET_EXPORT (
 StartMenu, 
 EDE_PANEL_APPLET_OPTION_ALIGN_FAR_LEFT,
 "Main menu",
 "0.1",
 "empty",
 "Sanel Zukan"
)
