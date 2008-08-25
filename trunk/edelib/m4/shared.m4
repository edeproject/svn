dnl
dnl $Id$
dnl
dnl Part of edelib.
dnl Copyright (c) 2000-2007 EDE Authors.
dnl 
dnl This program is licenced under terms of the 
dnl GNU General Public Licence version 2 or newer.
dnl See COPYING for details.

dnl Add --enable-shared option
dnl Also the bone of this code is based from FLTK's configure.in script
AC_DEFUN([EDELIB_SHARED], [
	AC_ARG_ENABLE(shared, [  --enable-shared         enable shared library],,enable_shared=no)
	if eval "test $enable_shared = no"; then
		dnl for jam
		SHARED=0
	else
		AC_MSG_CHECKING([for libtool])

		AC_PATH_PROG(LIBTOOL, libtool)
		if test -n "$LIBTOOL"; then
			AC_MSG_RESULT(yes)
			dnl only for jam
			SHARED=1
		else
			AC_MSG_RESULT(no)
			SHARED=0
		fi
	fi
])