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

#ifndef _GOOMSL_HASH_H
#define _GOOMSL_HASH_H

typedef struct GOOM_HASH_ENTRY GoomHashEntry;
typedef struct GOOM_HASH GoomHash;

typedef union {
    void *ptr;
    int   i;
    float f;
} HashValue;

struct GOOM_HASH_ENTRY {
    char          *key;
  HashValue      value;
    GoomHashEntry *lower;
    GoomHashEntry *upper;
};

struct GOOM_HASH {
    GoomHashEntry *root;
  int number_of_puts;
};

GoomHash *goom_hash_new(void);
void      goom_hash_free(GoomHash *gh);

void goom_hash_put(GoomHash *gh, const char *key, HashValue value);
HashValue *goom_hash_get(GoomHash *gh, const char *key);

void goom_hash_put_int  (GoomHash *_this, const char *key, int i);
void goom_hash_put_float(GoomHash *_this, const char *key, float f);
void goom_hash_put_ptr  (GoomHash *_this, const char *key, void *ptr);

typedef void (*GH_Func)(GoomHash *caller, const char *key, HashValue *value);

void goom_hash_for_each(GoomHash *_this, GH_Func func);
int goom_hash_number_of_puts(GoomHash *_this);

#endif /* _GOOM_HASH_H */
