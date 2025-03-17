/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
 ***************************************************************************/

#ifndef VISUALPALETTE_H
#define VISUALPALETTE_H

#include <stdint.h>
#include "qmmp_export.h"

namespace VisualPalette {
enum Palette
{
    PALETTE_SPECTRUM,
    PALETTE_PERCEPTUAL,
    PALETTE_RAINBOW,
    PALETTE_SOX,
    PALETTE_MAGMA,
    PALETTE_LINAS,
    PALETTE_CUBEHELIX,
    PALETTE_FRACTALIZER,
    PALETTE_MONO,
    PALETTE_COUNT,
    PALETTE_DEFAULT = PALETTE_PERCEPTUAL,
};

/*!
* Returns visual render palette by type.
*/
QMMP_EXPORT uint32_t renderPalette(Palette palette, double level);

}

#endif
