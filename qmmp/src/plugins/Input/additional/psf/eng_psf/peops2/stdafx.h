/***************************************************************************
                           StdAfx.h  -  description
                             -------------------
    begin                : Wed May 15 2002
    copyright            : (C) 2002 by Pete Bernert
    email                : BlackDove@addcom.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version. See also the license.txt file for *
 *   additional informations.                                              *
 *                                                                         *
 ***************************************************************************/

//*************************************************************************//
// History of changes:
//
// 2002/05/15 - Pete
// - generic cleanup for the Peops release
//
//*************************************************************************//

#define EXPORT_GCC

#include <stdlib.h>
#define RRand(range) (random()%range)
#include <string.h>
#include <math.h>

#undef CALLBACK
#define CALLBACK
#define DWORD unsigned long
#define LOWORD(l)           ((unsigned short)(l))
#define HIWORD(l)           ((unsigned short)(((unsigned long)(l) >> 16) & 0xFFFF))

#include "psemuxa.h"

