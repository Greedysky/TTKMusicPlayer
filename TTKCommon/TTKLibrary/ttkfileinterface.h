#ifndef TTKFILEINTERFACE_H
#define TTKFILEINTERFACE_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include <QFile>
#include "ttkmoduleexport.h"

/*! @brief The class of the ttk file interface.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKFileInterface
{
public:
    /*!
     * Object constructor.
     */
    TTKFileInterface() = default;
    /*!
     * Object destructor.
     */
    virtual ~TTKFileInterface() = default;

    /*!
     * Write datas from file by given name.
     */
    inline bool load(const QString &name)
    {
        m_file.setFileName(name);
        return m_file.open(QIODevice::WriteOnly);
    }

    /*!
     * Read datas from file by given name.
     */
    inline bool fromFile(const QString &name)
    {
        m_file.setFileName(name);
        return m_file.open(QIODevice::ReadOnly);
    }

protected:
    QFile m_file;

};

#endif // TTKFILEINTERFACE_H
