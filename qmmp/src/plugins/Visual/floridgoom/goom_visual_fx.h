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

#ifndef _VISUAL_FX_H
#define _VISUAL_FX_H

/**
 * File created on 2003-05-21 by Jeko.
 * (c)2003, JC Hoelt for iOS-software.
 *
 * LGPL Licence.
 * If you use this file on a visual program,
 *    please make my name being visible on it.
 */

#include "goom_config_param.h"
#include "goom_graphic.h"
#include "goom_typedefs.h"

struct _VISUAL_FX {
  void (*init) (struct _VISUAL_FX *_this, PluginInfo *info);
  void (*free) (struct _VISUAL_FX *_this);
  void (*apply) (struct _VISUAL_FX *_this, Pixel *src, Pixel *dest, PluginInfo *info);
  void *fx_data;

  PluginParameters *params;
};

#endif
