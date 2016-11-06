/***************************************************************************
 *   Copyright (C) 2014 by Ilya Kotov                                      *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QStringList>
#include <QHash>
#include "channelmap.h"

Qmmp::ChannelPosition ChannelMap::m_internal_map[9] = { Qmmp::CHAN_FRONT_LEFT,
                                                        Qmmp::CHAN_FRONT_RIGHT,
                                                        Qmmp::CHAN_REAR_LEFT,
                                                        Qmmp::CHAN_REAR_RIGHT,
                                                        Qmmp::CHAN_FRONT_CENTER,
                                                        Qmmp::CHAN_REAR_CENTER,
                                                        Qmmp::CHAN_LFE,
                                                        Qmmp::CHAN_SIDE_LEFT,
                                                        Qmmp::CHAN_SIDE_RIGHT };
ChannelMap::ChannelMap() {}

ChannelMap::ChannelMap(int channels)
{
    generateMap(channels);
}

int ChannelMap::mask() const
{
    int mask = 0;
    foreach (Qmmp::ChannelPosition channel, *this)
    {
        mask |= channel;
    }
    return mask;
}

const ChannelMap ChannelMap::remaped() const
{
    ChannelMap map;
    for(int i = 0; i < 9; ++i)
    {
         if(contains(m_internal_map[i]))
             map.append(m_internal_map[i]);
    }
    while (map.count() < count())
    {
        map.append(Qmmp::CHAN_NULL);
    }
    return map;
}

const QString ChannelMap::toString() const
{
    QStringList list;
    QHash <Qmmp::ChannelPosition, QString> names;
    names.insert(Qmmp::CHAN_NULL, "NA");
    names.insert(Qmmp::CHAN_FRONT_LEFT, "FL");
    names.insert(Qmmp::CHAN_FRONT_RIGHT, "FR");
    names.insert(Qmmp::CHAN_REAR_LEFT, "RL");
    names.insert(Qmmp::CHAN_REAR_RIGHT, "RR");
    names.insert(Qmmp::CHAN_FRONT_CENTER, "FC");
    names.insert(Qmmp::CHAN_REAR_CENTER, "RC");
    names.insert(Qmmp::CHAN_LFE, "LFE");
    names.insert(Qmmp::CHAN_SIDE_LEFT, "SL");
    names.insert(Qmmp::CHAN_SIDE_RIGHT, "SR");


    foreach (Qmmp::ChannelPosition channel, *this)
    {
       list << names.value(channel);
    }
    return list.join(",");
}

void ChannelMap::generateMap(int channels)
{
    int mask = 0;

    switch (channels)
    {
    case 1:
        mask = Qmmp::CHAN_FRONT_LEFT;
        break;
    case 2:
        mask = Qmmp::CHAN_FRONT_LEFT
                | Qmmp::CHAN_FRONT_RIGHT;
        break;
    case 3:
        mask = Qmmp::CHAN_FRONT_LEFT
                | Qmmp::CHAN_FRONT_RIGHT
                | Qmmp::CHAN_FRONT_CENTER;
        break;
    case 4:
        mask = Qmmp::CHAN_FRONT_LEFT
                | Qmmp::CHAN_FRONT_RIGHT
                | Qmmp::CHAN_REAR_LEFT
                | Qmmp::CHAN_REAR_RIGHT;
        break;
    case 5:
        mask = Qmmp::CHAN_FRONT_LEFT
                | Qmmp::CHAN_FRONT_RIGHT
                | Qmmp::CHAN_FRONT_CENTER
                | Qmmp::CHAN_REAR_LEFT
                | Qmmp::CHAN_REAR_RIGHT;
        break;
    case 6:
        mask = Qmmp::CHAN_FRONT_LEFT
                | Qmmp::CHAN_FRONT_RIGHT
                | Qmmp::CHAN_FRONT_CENTER
                | Qmmp::CHAN_LFE
                | Qmmp::CHAN_REAR_LEFT
                | Qmmp::CHAN_REAR_RIGHT;
        break;
    case 7:
        mask = Qmmp::CHAN_FRONT_LEFT
                | Qmmp::CHAN_FRONT_RIGHT
                | Qmmp::CHAN_FRONT_CENTER
                | Qmmp::CHAN_LFE
                | Qmmp::CHAN_REAR_CENTER
                | Qmmp::CHAN_SIDE_LEFT
                | Qmmp::CHAN_SIDE_RIGHT;
        break;
    case 8:
        mask = Qmmp::CHAN_FRONT_LEFT
                | Qmmp::CHAN_FRONT_RIGHT
                | Qmmp::CHAN_FRONT_CENTER
                | Qmmp::CHAN_LFE
                | Qmmp::CHAN_REAR_LEFT
                | Qmmp::CHAN_REAR_RIGHT
                | Qmmp::CHAN_SIDE_LEFT
                | Qmmp::CHAN_SIDE_RIGHT;
        break;
    default:
        ;
    }

    for(int i = 0; i < 9; ++i)
    {
        if(mask & m_internal_map[i])
            append(m_internal_map[i]);
    }
}
