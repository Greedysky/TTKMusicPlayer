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

#ifndef GOOMSL_HEAP
#define GOOMSL_HEAP

/**
 * Resizable Array that guarranty that resizes don't change address of
 * the stored datas.
 *
 * This is implemented as an array of arrays... granularity is the size
 * of each arrays.
 */

typedef struct _GOOM_HEAP GoomHeap;

/* Constructors / Destructor */
GoomHeap *goom_heap_new(void);
GoomHeap *goom_heap_new_with_granularity(int granularity);
void      goom_heap_delete(GoomHeap *_this);

/* This method behaves like malloc. */
void     *goom_heap_malloc(GoomHeap *_this, int nb_bytes);
/* This adds an alignment constraint. */
void     *goom_heap_malloc_with_alignment(GoomHeap *_this, int nb_bytes, int alignment);

/* Returns a pointeur on the bytes... prefix is before */
void     *goom_heap_malloc_with_alignment_prefixed(GoomHeap *_this, int nb_bytes,
                                                   int alignment, int prefix_bytes);

#endif

