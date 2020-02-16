#ifndef QOSSDATAITEM_H
#define QOSSDATAITEM_H

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

#include "musicextrasglobaldefine.h"

/*! @brief The class of the alioss data item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_EXTRAS_EXPORT QOSSDataItem
{
    QString m_name;
    QString m_hash;
    QString m_mimeType;
    QString m_putTime;
    int m_size;

    QOSSDataItem()
    {
        m_size = 0;
    }

    inline void clear()
    {
        m_name.clear();
        m_hash.clear();
        m_mimeType.clear();
        m_putTime.clear();
        m_size = 0;
    }
}QOSSDataItem;
TTK_DECLARE_LISTS(QOSSDataItem)

#endif // QOSSDATAITEM_H
