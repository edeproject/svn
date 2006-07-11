/*
 * $Id$
 *
 * Edewm, window manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include "debug.h"
#include <X11/Xlib.h>

#ifdef _DEBUG
extern int x_return_code;

#define CHECK_RET1(function, ret1) \
	x_return_code = function; \
	if(x_return_code == ret1) \
		EWARNING("%s got %s (%s %i)", #function, #ret1, __FILE__, __LINE__)

#define CHECK_RET2(function, ret1, ret2) \
	x_return_code = function; \
	if(ret1 == x_return_code) \
		EWARNING("%s got %s (%s %i)", #function, #ret1, __FILE__, __LINE__); \
	if(ret2 == x_return_code) \
		EWARNING("%s got %s (%s %i)", #function, #ret2, __FILE__, __LINE__)

#define CHECK_RET3(function, ret1, ret2, ret3) \
	x_return_code = function; \
	if(ret1 == x_return_code) \
		EWARNING("%s got %s (%s %i)", #function, #ret1, __FILE__, __LINE__); \
	if(ret2 == x_return_code) \
		EWARNING("%s got %s (%s %i)", #function, #ret2, __FILE__, __LINE__); \
	if(ret3 == x_return_code) \
		EWARNING("%s got %s (%s %i)", #function, #ret3, __FILE__, __LINE__)

#else

#define CHECK_RET1(function, ret1) function
#define CHECK_RET2(function, ret1, ret2) function
#define CHECK_RET3(function, ret1, ret2, ret3) function

#endif // _DEBUG

void* GetProperty(Window w, Atom a, Atom type, unsigned long *np, int *ret);
void SendMessage(Window win, Atom a, Atom l);

/*
void SetProperty(Window w, Atom a, Atom type, unsigned long val);
void AppendProperty(Window w, Atom a, Atom type, unsigned long val);
void DeleteProperty(Window w, Atom a, Atom type);
*/

#endif // _UTILS_H_
