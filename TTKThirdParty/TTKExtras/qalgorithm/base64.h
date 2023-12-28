#ifndef BASE64_H
#define BASE64_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include "ttkglobal.h"

namespace QAlgorithm
{
    /*!
     * Base64 encode.
     */
    TTKString base64Encode(const unsigned char *bytes, unsigned int len);
    /*!
     * Base64 dncode.
     */
    TTKString base64Decode(const TTKString &bytes);

}

#endif // BASE64_H
