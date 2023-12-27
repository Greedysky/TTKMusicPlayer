#ifndef QIMAGEWRAPPER_H
#define QIMAGEWRAPPER_H

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

#include <QImage>
#include "ttkprivate.h"

namespace QAlgorithm
{
class SharpeImagePrivate;

/*! @brief The class of the sharpe image.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT SharpeImage
{
public:
    /*!
     * Object constructor.
     */
    SharpeImage();
    /*!
     * Object destructor.
     */
    virtual ~SharpeImage() = default;

    /*!
     * Init item data.
     */
    virtual void input(const QRect &region);
    /*!
     * Render data.
     * Subclass should implement this function.
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
    /*!
     * Object constructor.
     */
    GaussBlur();

    /*!
     * Render data.
     */
    virtual QPixmap render(const QPixmap &pixmap, int value) override final;

};


/*! @brief The class of the cube wave.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT CubeWave : public SharpeImage
{
public:
    /*!
     * Object constructor.
     */
    CubeWave();

    /*!
     * Init item data.
     */
    virtual void input(const QRect &region) override final;
    /*!
     * Render data.
     */
    virtual QPixmap render(const QPixmap &pixmap, int value) override final;

};


/*! @brief The class of the water wave.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT WaterWave : public SharpeImage
{
public:
    /*!
     * Object constructor.
     */
    WaterWave(const QImage &image, int radius);

    /*!
     * Init item data.
     */
    virtual void input(const QRect &region) override final;
    /*!
     * Render data.
     */
    virtual QPixmap render(const QPixmap &pixmap, int value) override final;

};
}

#endif // QIMAGEWRAPPER_H
