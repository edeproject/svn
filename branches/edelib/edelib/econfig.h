/*
 * $Id$
 *
 * Base defs for edelib
 * Part of edelib.
 * Copyright (c) 2005-2007 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef __ECONFIG_H__
#define __ECONFIG_H__

/* Needed so we don't have to include
 * necessary libraries.
 *
 * TODO: place it in Types.h ?
 */
#ifndef __cplusplus
	#define NULL 0
#else 
	#define NULL ((void*)0)
#endif

#define EDELIB_NS edelib
#define EDELIB_NAMESPACE namespace EDELIB_NS
#define EDELIB_NAMESPACE_USE using namespace EDELIB_NS

#define EDELIB_API 

#endif