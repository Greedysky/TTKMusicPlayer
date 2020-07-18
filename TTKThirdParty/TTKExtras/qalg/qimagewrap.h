#ifndef QIMAGEWRAP_H
#define QIMAGEWRAP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include <QImage>
#include "musicextrasglobaldefine.h"

/*! @brief The class of the image wrapper.
 * @author Greedysky <greedysky@163.com>
 */
namespace QImageWrap {
/*! @brief The class of the gauss blur wrapper.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT QGaussBlur
{
public:
    /*!
     * Image gauss blur render.
     */
    void render(int* pix, int width, int height, int radius);

};


class QWaterWavePrivate;
/*! @brief The class of the water wave wrapper.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT QWaterWave
{
public:
    QWaterWave(const QImage &image, int radius);

    int* data();
    void render();

    void input(int x, int y);

private:
    TTK_DECLARE_PRIVATE(QWaterWave)

};
}

#endif // QIMAGEWRAP_H
