/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2018 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#ifndef SPEKPALETTE_H
#define SPEKPALETTE_H

#include <stdint.h>

typedef enum palette {
    PALETTE_SPECTRUM,
    PALETTE_SOX,
    PALETTE_MONO,
    PALETTE_COUNT,
    PALETTE_DEFAULT = PALETTE_SOX,
}Palette;

uint32_t spek_palette(Palette palette, double level);

#endif
