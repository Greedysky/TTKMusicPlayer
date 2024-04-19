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

ChannelMap::ChannelMap(int channels)
{
    generateMap(channels);
}

int ChannelMap::mask() const
{
    int mask = 0;
    for(const Qmmp::ChannelPosition channel : qAsConst(*this))
    {
        mask |= channel;
    }
    return mask;
}

ChannelMap ChannelMap::remaped() const
{
    ChannelMap map;
    for(int i = 0; i < 9; ++i)
    {
         if(contains(m_internal_map[i]))
             map.append(m_internal_map[i]);
    }
    while(map.count() < count())
    {
        map.append(Qmmp::CHAN_NULL);
    }
    return map;
}

QString ChannelMap::toString() const
{
    QHash<Qmmp::ChannelPosition, QString> names;
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

    QStringList list;
    for(const Qmmp::ChannelPosition channel : qAsConst(*this))
    {
       list << names.value(channel);
    }
    return list.join(",");
}

void ChannelMap::generateMap(int channels)
{
    int mask = 0;

    switch(channels)
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
        break;
    }

    for(int i = 0; i < 9; ++i)
    {
        if(mask & m_internal_map[i])
            append(m_internal_map[i]);
    }
}
