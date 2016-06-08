
CPPFILES = evolume.cpp prefs.cpp ../common/aboutdialog.cpp
TARGET   = evolume


POFILES = locale/ru.po\
		  locale/sk.po\
		  locale/hu.po

include ../makeinclude

install:
	$(INSTALL_PROGRAM) $(TARGET) $(bindir)
	$(INSTALL_LOCALE)

uninstall:
	$(RM) $(bindir)/$(TARGET)

clean:
	$(RM) $(TARGET)
	$(RM) *.o

