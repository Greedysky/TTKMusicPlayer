/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
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

#include "speexmetadatamodel.h"
#include "speexhelper.h"

SpeexMetaDataModel::SpeexMetaDataModel(const QString &path) : MetaDataModel(true)
{
    m_path = path;
}

QList<MetaDataItem> SpeexMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    QFile file(m_path);
    if(file.open(QIODevice::ReadOnly))
    {
        SpeexHelper helper(&file);
        if(!helper.initialize())
        {
            return ep;
        }

        for(int i = 0; i < helper.stream_get_tagcount(); ++i)
        {
            const speextags* tag = helper.stream_get_tags()[i];
            if(tag->item)
            {
                ep << MetaDataItem(tag->item, tag->value ? tag->value : QString());
            }
        }
    }

    return ep;
}
