#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include "ttkprivate.h"
#include "musicextrasglobaldefine.h"

class GaussianBlurPrivate;

/*! @brief The namespace of the Gaussian Blur.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT GaussianBlur
{
public:
    /*!
     * Object contsructor.
     */
    GaussianBlur();
    /*!
     * Image gauss alg.
     */
    void gaussBlur(int* pix, int w, int h, int radius);
    /*!
     * Image gauss alg.
     */
    void gaussBlurPlus(int* pix, int w, int h, int radius);

private:
    TTK_DECLARE_PRIVATE(GaussianBlur)

};

#endif // GAUSSIANBLUR_H
