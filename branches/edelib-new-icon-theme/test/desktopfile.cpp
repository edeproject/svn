#include <edelib/DesktopFile.h>
#include <edelib/File.h>
#include <string.h>
#include <stdlib.h>
#include "UnitTest.h"

#define CCHARP(str)           ((const char*)str)
#define STR_EQUAL(str1, str2) (strcmp(CCHARP(str1), CCHARP(str2)) == 0)

EDELIB_NS_USE

// write messy .desktop file intentionally :P
const char* desktop_sample = "\
[Desktop Entry]\n                      \
   Type=Application\n                  \
   Hidden=False\n                      \
   Terminal=False\n                    \
   Icon=sample-icon\n                  \
   Exec=sample-exec\n                  \
   TryExec=sample-try-exec-must-fail\n \
   OnlyShowIn=EDE;gnome;\n";

UT_FUNC(DesktopFileTest, "Test DesktopFile class")
{
	DesktopFile desk;
	desk.load("test.desktop");
	if(!desk) {
		UT_FAIL("No test.desktop, but expected to be");
		return;
	}

	char buff[256];
	// URL
	UT_VERIFY( desk.url(buff, 256) == true );
	UT_VERIFY( STR_EQUAL(buff, "file:$HOME") );
	UT_VERIFY( desk.type() == DESK_FILE_TYPE_LINK );

	// Icon
	UT_VERIFY( desk.icon(buff, 256) == true );
	UT_VERIFY( STR_EQUAL(buff, "folder_home") );

	// Name
	UT_VERIFY( desk.name(buff, 256) == true );
	UT_VERIFY( STR_EQUAL(buff, "Home Folder") );

	// Comment
	UT_VERIFY( desk.comment(buff, 256) == true );
	UT_VERIFY( STR_EQUAL(buff, "This folder contains your personal files") );
}

UT_FUNC(DesktopFileTest2, "Test DesktopFile class (2)")
{
	File f;
	f.open("foo.desktop", FIO_WRITE);
	if(!f.write(desktop_sample, strlen(desktop_sample))) {
		UT_FAIL("Unable to write foo.desktop");
		return;
	}
	f.close();

	DesktopFile desk;
	desk.load("foo.desktop");
	if(!desk) {
		UT_FAIL("No foo.desktop, but expected to be");
		return;
	}

	char buff[256];
	UT_VERIFY( desk.type() == DESK_FILE_TYPE_APPLICATION );
	UT_VERIFY( desk.hidden() == false );
	UT_VERIFY( desk.terminal() == false );

	UT_VERIFY( desk.icon(buff, 256) == true );
	UT_VERIFY( STR_EQUAL(buff, "sample-icon") );

	UT_VERIFY( desk.exec(buff, 256) == true );
	UT_VERIFY( STR_EQUAL(buff, "sample-exec") );

	// here try_exec() must fail since it checks if value actually exists on system
	UT_VERIFY( desk.try_exec(buff, 256) == false );

	UT_VERIFY( desk.only_show_in(buff, 256) == true );
	UT_VERIFY( STR_EQUAL(buff, "EDE;gnome;") );

	list<String> lret;
	list<String>::iterator first;

	UT_VERIFY( desk.only_show_in(lret) == true );
	UT_VERIFY( lret.size() == 2 );

	first = lret.begin();
	UT_VERIFY( (*first) == "EDE" );
	++first;
	UT_VERIFY( (*first) == "gnome" );

	// now replace last item in OnlyShowIn
	(*first) = "KDE";
	desk.set_only_show_in(lret);

	if(!desk.save("foo.desktop")) {
		UT_FAIL("Unable to write to foo.desktop");
		return;
	}

	// re-read again
	desk.load("foo.desktop");
	if(!desk) {
		UT_FAIL("No foo.desktop, but expected to be");
		return;
	}

	lret.clear();

	UT_VERIFY( desk.only_show_in(buff, 256) == true );
	UT_VERIFY( STR_EQUAL(buff, "EDE;KDE;") );

	UT_VERIFY( desk.only_show_in(lret) == true );
	first = lret.begin();
	UT_VERIFY( (*first) == "EDE" );
	++first;
	UT_VERIFY( (*first) == "KDE" );

	file_remove("foo.desktop");
}

UT_FUNC(DesktopFileTest3, "Test DesktopFile class (3)")
{
	// test unique OnlyShowIn/NotShowIn
	File f;
	f.open("foo.desktop", FIO_WRITE);
	if(!f.write(desktop_sample, strlen(desktop_sample))) {
		UT_FAIL("Unable to write foo.desktop");
		return;
	}
	f.close();

	DesktopFile desk;
	desk.load("foo.desktop");
	if(!desk) {
		UT_FAIL("No foo.desktop, but expected to be");
		return;
	}

	list<String> lst;
	lst.push_back("foo1");
	lst.push_back("foo2");
	lst.push_back("foo3");

	desk.set_not_show_in(lst);

	if(!desk.save("foo.desktop")) {
		UT_FAIL("Unable to write to foo.desktop");
		return;
	}

	// re-read again, NotShowIn must not be written since OnlyShowIn is already there
	lst.clear();
	UT_VERIFY( desk.not_show_in(lst) == false );

	file_remove("foo.desktop");
}
