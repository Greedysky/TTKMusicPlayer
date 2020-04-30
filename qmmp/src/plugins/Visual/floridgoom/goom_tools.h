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

#ifndef _GOOMTOOLS_H
#define _GOOMTOOLS_H

/**
 * Random number generator wrapper for faster random number.
 */

#define GOOM_NB_RAND 0x10000

typedef struct _GOOM_RANDOM {
    int array[GOOM_NB_RAND];
    unsigned short pos;
} GoomRandom;

GoomRandom *goom_random_init(int i);
void goom_random_free(GoomRandom *grandom);

inline static int goom_random(GoomRandom *grandom) {

    grandom->pos++; /* works because pos is an unsigned short */
    return grandom->array[grandom->pos];
}

inline static int goom_irand(GoomRandom *grandom, int i) {

    grandom->pos++;
    return grandom->array[grandom->pos] % i;
}

/* called to change the specified number of value in the array, so that the array does not remain the same*/
void goom_random_update_array(GoomRandom *grandom, int numberOfValuesToChange);

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#endif
