#ifndef MUSICCLOUDDATAITEM_H
#define MUSICCLOUDDATAITEM_H

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

#include "musicglobaldefine.h"

#include "qsync/qsyncdataitem.h"

/*! @brief The class of the cloud data item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicCloudDataItem
{
    enum class State
    {
        Waited,         /*!< waited state*/
        Uploaded,       /*!< Uploaded state*/
        Successed,      /*!< successed state*/
        Errored         /*!< error state*/
    };

    QString m_id;
    QString m_path;
    State m_state;
    QSyncDataItem m_data;

    inline bool isValid() const
    {
        return !m_data.m_name.isEmpty();
    }

    inline void clear()
    {
        m_id.clear();
        m_path.clear();
        m_state = State::Waited;
        m_data.clear();
    }

    inline bool operator==(const MusicCloudDataItem &other) const
    {
        return m_data.m_name == other.m_data.m_name;
    }
};

#endif // MUSICCLOUDDATAITEM_H
