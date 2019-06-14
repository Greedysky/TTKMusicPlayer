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

#ifndef _GOOMSL_H
#define _GOOMSL_H

#include "goomsl_hash.h"

typedef struct _GoomSL GoomSL;
typedef void (*GoomSL_ExternalFunction)(GoomSL *gsl, GoomHash *global_vars, GoomHash *local_vars);

GoomSL*gsl_new(void);
void   gsl_free(GoomSL *gss);

char *gsl_init_buffer(const char *file_name);
void  gsl_append_file_to_buffer(const char *file_name, char **buffer);

void   gsl_compile (GoomSL *scanner, const char *script);
void   gsl_execute (GoomSL *scanner);
int    gsl_is_compiled  (GoomSL *gss);
void   gsl_bind_function(GoomSL *gss, const char *fname, GoomSL_ExternalFunction func);

int    gsl_malloc  (GoomSL *_this, int size);
void  *gsl_get_ptr (GoomSL *_this, int id);
void   gsl_free_ptr(GoomSL *_this, int id);

GoomHash *gsl_globals(GoomSL *_this);

#define GSL_LOCAL_PTR(gsl,local,name)   gsl_get_ptr(gsl, *(int*)goom_hash_get(local,name)->ptr)
#define GSL_LOCAL_INT(gsl,local,name)   (*(int*)goom_hash_get(local,name)->ptr)
#define GSL_LOCAL_FLOAT(gsl,local,name) (*(float*)goom_hash_get(local,name)->ptr)

#define GSL_GLOBAL_PTR(gsl,name)   gsl_get_ptr(gsl, *(int*)goom_hash_get(gsl_globals(gsl),name)->ptr)
#define GSL_GLOBAL_INT(gsl,name)   (*(int*)goom_hash_get(gsl_globals(gsl),name)->ptr)
#define GSL_GLOBAL_FLOAT(gsl,name) (*(float*)goom_hash_get(gsl_globals(gsl),name)->ptr)

#endif
