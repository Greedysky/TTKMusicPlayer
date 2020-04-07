/* =================================================
 * This file is part of the TTK qmmp plugin project
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

#include "decoderfc14factory.h"
#include "decoder_fc14.h"
#include "fc14helper.h"

bool DecoderFC14Factory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderFC14Factory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("FC14 Plugin");
    properties.filters << "*.fc" << "*.fc13" << "*.fc14" << "*.smod";
    properties.description = tr("Future Composer Audio Files");
    properties.shortName = "fc14";
    properties.noInput = true;
    properties.protocols << "file";
    return properties;
}

Decoder *DecoderFC14Factory::create(const QString &path, QIODevice *)
{
    return new DecoderFC14(path);
}

QList<TrackInfo *> DecoderFC14Factory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
    {
        return QList<TrackInfo *>() << info;
    }

    FC14Helper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo *>();
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.samplerate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.bitsPerSample());
        info->setDuration(helper.totalTime() * 1000);
    }

    return QList<TrackInfo *>() << info;
}

MetaDataModel* DecoderFC14Factory::createMetaDataModel(const QString &, bool)
{
    return nullptr;
}
