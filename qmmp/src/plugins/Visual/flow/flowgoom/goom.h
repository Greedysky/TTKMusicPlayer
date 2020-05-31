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

#ifndef _GOOMCORE_H
#define _GOOMCORE_H

#include "goom_config.h"
#include "goom_plugin_info.h"
#include "goomsl.h"

#define NB_FX 10

PluginInfo *goom_init (guint32 resx, guint32 resy);
void goom_set_resolution (PluginInfo *goomInfo, guint32 resx, guint32 resy);

/*
 * forceMode == 0 : do nothing
 * forceMode == -1 : lock the FX
 * forceMode == 1..NB_FX : force a switch to FX n# forceMode
 *
 * songTitle = pointer to the title of the song...
 *      - NULL if it is not the start of the song
 *      - only have a value at the start of the song
 */
guint32 *goom_update (PluginInfo *goomInfo, gint16 data[2][512], int forceMode, float fps,
                      const char *songTitle, const char *message);

/* returns 0 if the buffer wasn't accepted */
int goom_set_screenbuffer(PluginInfo *goomInfo, void *buffer);

void goom_close (PluginInfo *goomInfo);

#endif
