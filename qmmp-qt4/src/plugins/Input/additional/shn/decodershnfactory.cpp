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

#include <QtPlugin>
#include "shnhelper.h"
#include "decoder_shn.h"
#include "decodershnfactory.h"
#include "shnmetadatamodel.h"

bool DecoderSHNFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderSHNFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("SHN Plugin");
    properties.filters << "*.tta";
    properties.description = tr("Shorten Music Files");
    properties.shortName = "shn";
    properties.noInput = true;
    properties.protocols << "shn";
    return properties;
}

Decoder *DecoderSHNFactory::create(const QString &path, QIODevice *)
{
    return new DecoderSHN(path);
}

QList<TrackInfo *> DecoderSHNFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
    {
        return QList<TrackInfo *>() << info;
    }

    SHNHelper helper(path);
    if(!helper.initialize())
    {
        return QList<TrackInfo *>();
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.samplerate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setDuration(helper.totalTime() / 1000);
    }

    return QList<TrackInfo *>() << info;
}

MetaDataModel* DecoderSHNFactory::createMetaDataModel(const QString &path, bool)
{
    return new SHNMetaDataModel(path);
}

Q_EXPORT_PLUGIN2(shn,DecoderSHNFactory)
