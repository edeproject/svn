
CPPFILES = eiconman.cpp edeskicon.cpp propdialog.cpp edeskconf.cpp
TARGET   = eiconman

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

