#ifndef QDESWRAPPER_H
#define QDESWRAPPER_H

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

#include "ttkprivate.h"

/*! @brief The namespace of the algorithm.
 * @author Greedysky <greedysky@163.com>
 */
namespace QAlgorithm
{
class DesPrivate;

/*! @brief The class of the des wrapper.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT Des
{
public:
    enum class Mode
    {
        Encrypt = 0,
        Decrypt = 1
    };
    
    /*!
     * Object constructor.
     */
    Des();

    /*!
     * Encrypt des by input.
     */
    QByteArray encrypt(const QByteArray &in, const QByteArray &key);
    /*!
     * Decrypt des by input.
     */
    QByteArray decrypt(const QByteArray &in, const QByteArray &key);

private:
    TTK_DECLARE_PRIVATE(Des)

};
}

#endif // QDESWRAPPER_H
