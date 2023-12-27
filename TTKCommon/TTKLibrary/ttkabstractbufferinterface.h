#ifndef TTKABSTRACTBUFFERINTERFACE_H
#define TTKABSTRACTBUFFERINTERFACE_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2024 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "ttkmoduleexport.h"

/*! @brief The class of the read interface.
 * @author Greedysky <greedysky@163.com>
 */
template <typename T>
class TTK_MODULE_EXPORT TTKAbstractReadInterface
{
public:
    /*!
     * Object destructor.
     */
    virtual ~TTKAbstractReadInterface() = default;

    /*!
     * Read datas from buffer.
     * Subclass should implement this function.
     */
    virtual bool readBuffer(T &items) = 0;

};


/*! @brief The class of the write interface.
 * @author Greedysky <greedysky@163.com>
 */
template <typename T>
class TTK_MODULE_EXPORT TTKAbstractWriteInterface
{
public:
    /*!
     * Object destructor.
     */
    virtual ~TTKAbstractWriteInterface() = default;

    /*!
     * Write datas into buffer.
     * Subclass should implement this function.
     */
    virtual bool writeBuffer(const T &items) = 0;

};


/*! @brief The class of the read write interface.
 * @author Greedysky <greedysky@163.com>
 */
template <typename T>
class TTK_MODULE_EXPORT TTKAbstractReadWriteInterface : private TTKAbstractReadInterface<T>, private TTKAbstractWriteInterface<T>
{
public:
    /*!
     * Object destructor.
     */
    virtual ~TTKAbstractReadWriteInterface() = default;

};

#endif // TTKABSTRACTBUFFERINTERFACE_H
