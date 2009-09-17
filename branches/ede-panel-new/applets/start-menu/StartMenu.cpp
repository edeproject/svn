#include "Applet.h"
#include "XdgMenuReader.h"

#include <edelib/MenuButton.h>

EDELIB_NS_USING(MenuButton)

class StartMenu : public MenuButton {
private:
	MenuItem *mcontent;

public:
	StartMenu() : MenuButton(600, 0, 90, 25, "EDE"), mcontent(NULL) {
		mcontent = xdg_menu_load();
		menu(mcontent);
	}

	~StartMenu() {
		xdg_menu_delete(mcontent);
	}
};

EDE_PANEL_APPLET_EXPORT (
 StartMenu, 
 "Main menu",
 "0.1",
 "empty",
 "Sanel Zukan"
)
