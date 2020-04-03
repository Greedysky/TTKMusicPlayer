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

#include "sc68helper.h"
#include "decoder_sc68.h"
#include "decodersc68factory.h"

bool DecoderSC68Factory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderSC68Factory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("SC68 Plugin");
    properties.filters << "*.sndh" << "*.snd" << "*.sc68";
    properties.description = tr("Audio Overload Files");
    properties.shortName = "sc68";
    properties.noInput = true;
    properties.protocols << "sc68";
    return properties;
}

Decoder *DecoderSC68Factory::create(const QString &path, QIODevice *)
{
    return new DecoderSC68(path);
}

QList<TrackInfo *> DecoderSC68Factory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
    {
        return QList<TrackInfo *>() << info;
    }

    SC68Helper helper(path);
    if(!helper.initialize())
    {
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

MetaDataModel* DecoderSC68Factory::createMetaDataModel(const QString &, bool)
{
    return nullptr;
}
