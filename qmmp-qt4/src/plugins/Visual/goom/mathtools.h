/***************************************************************************
 *   Copyright (C) 2000-2004 by                                            *
 *   Jean-Christophe Hoelt <jeko@ios-software.com>                         *
 *   Guillaume Borios <gyom@ios-software.com>                              *
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef MATHTOOLS_H
#define MATHTOOLS_H


#define _double2fixmagic (68719476736.0*1.5)
/* 2^36 * 1.5,  (52-_shiftamt=36) uses limited precisicion to floor */
#define _shiftamt 16
/* 16.16 fixed point representation */

#if BigEndian_
#define iexp_				0
#define iman_				1
#else
#define iexp_				1
#define iman_				0
#endif /* BigEndian_ */

/* TODO: this optimization is very efficient: put it again when all works
#ifdef HAVE_MMX
#define F2I(dbl,i) {double d = dbl + _double2fixmagic; i = ((int*)&d)[iman_] >> _shiftamt;}
#else*/
#define F2I(dbl,i) i=(int)dbl;
/*#endif*/

#if 0
#define SINCOS(f,s,c) \
  __asm__ __volatile__ ("fsincos" : "=t" (c), "=u" (s) : "0" (f))
#else
#define SINCOS(f,s,c) {s=sin(f);c=cos(f);}
#endif

#include "mathtools.c"

#endif

