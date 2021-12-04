#ifndef QIMAGEWRAPPER_H
#define QIMAGEWRAPPER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include <QImage>
#include "ttkprivate.h"

/*! @brief The class of the image wrapper.
 * @author Greedysky <greedysky@163.com>
 */
namespace QImageWrapper {
class SharpeImagePrivate;

/*! @brief The class of the sharpe image.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT SharpeImage
{
public:
    SharpeImage();
    virtual ~SharpeImage();

    /*!
     * Init item data.
     */
    virtual void input(const QRect &region);
    /*!
     * Render data.
     */
    virtual QPixmap render(const QPixmap &pixmap, int value) = 0;

protected:
    SharpeImage(SharpeImagePrivate &pvt);

protected:
    TTK_DECLARE_PRIVATE(SharpeImage)

};


/*! @brief The class of the gauss blur.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT GaussBlur : public SharpeImage
{
public:
    GaussBlur();

    /*!
     * Render data.
     */
    virtual QPixmap render(const QPixmap &pixmap, int value) override;

};


/*! @brief The class of the cube wave.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT CubeWave : public SharpeImage
{
public:
    CubeWave();

    /*!
     * Init item data.
     */
    virtual void input(const QRect &region) override;
    /*!
     * Render data.
     */
    virtual QPixmap render(const QPixmap &pixmap, int value) override;

};


/*! @brief The class of the water wave.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT WaterWave : public SharpeImage
{
public:
    WaterWave(const QImage &image, int radius);

    /*!
     * Init item data.
     */
    virtual void input(const QRect &region) override;
    /*!
     * Render data.
     */
    virtual QPixmap render(const QPixmap &pixmap, int value) override;

};
}

#endif // QIMAGEWRAPPER_H
