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

#include "asaphelper.h"
#include "decoder_asap.h"
#include "decoderasapfactory.h"
#include "asapmetadatamodel.h"

bool DecoderAsapFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderAsapFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Asap Plugin");
    properties.filters << "*.sap";
    properties.filters << "*.cm3" << "*.cmc" << "*.cmr" << "*.cms";
    properties.filters << "*.dmc" << "*.dlt";
    properties.filters << "*.mpd" << "*.mpt" << "*.rmt" << "*.tm2" << "*.tm8" << "*.tmc" << "*.fc";
    properties.description = tr("Another Slight Atari Player Files");
    properties.shortName = "asap";
    properties.noInput = true;
    properties.protocols << "asap";
    return properties;
}

Decoder *DecoderAsapFactory::create(const QString &path, QIODevice *)
{
    return new DecoderAsap(path);
}

QList<TrackInfo *> DecoderAsapFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
    {
        return QList<TrackInfo *>() << info;
    }

    AsapHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo *>();
    }

    if(parts & TrackInfo::MetaData)
    {
        helper.readMetaTags();
        info->setValue(Qmmp::TITLE, helper.title());
        info->setValue(Qmmp::ARTIST, helper.artist());
        info->setValue(Qmmp::YEAR, helper.year());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.samplerate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.bitsPerSample());
        info->setDuration(helper.totalTime());
    }

    return QList<TrackInfo *>() << info;
}

MetaDataModel* DecoderAsapFactory::createMetaDataModel(const QString &path, bool)
{
    return new AsapMetaDataModel(path);
}
