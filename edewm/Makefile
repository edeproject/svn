
#CPPFILES = WMWindow.cpp Theme.cpp Netwm.cpp Mwm.cpp Icccm.cpp Icon.cpp Cursor.cpp Desktop.cpp Events.cpp Frame.cpp Hotkeys.cpp Tabmenu.cpp Titlebar.cpp Windowmanager.cpp Winhints.cpp main.cpp
CPPFILES = WMWindow.cpp      \
           Theme.cpp         \
           Netwm.cpp         \
		   Mwm.cpp           \
		   Icccm.cpp         \
		   Icon.cpp          \
		   Cursor.cpp        \
		   Desktop.cpp       \
		   Events.cpp        \
		   Frame.cpp         \
		   Hotkeys.cpp       \
		   Tabmenu.cpp       \
		   Titlebar.cpp      \
		   Windowmanager.cpp \
		   Winhints.cpp      \
		   main.cpp

TARGET   = edewm

POFILES = locale/ru.po\
			locale/sr.po\
			locale/sk.po\
			locale/hu.po\

include ../makeinclude

install:
	$(INSTALL_PROGRAM) $(TARGET) $(bindir)
	$(INSTALL_LOCALE)

uninstall:
	$(RM) $(bindir)/$(TARGET)

clean:
	$(RM) $(TARGET)
	$(RM) *.o

