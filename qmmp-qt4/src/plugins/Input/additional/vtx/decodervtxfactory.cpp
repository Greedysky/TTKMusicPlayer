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
#include "vtxhelper.h"
#include "decoder_vtx.h"
#include "decodervtxfactory.h"
#include "vtxmetadatamodel.h"

bool DecoderVTXFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderVTXFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("VTX Plugin");
    properties.filters << "*.vtx";
    properties.description = tr("AY/YM Music Files");
    properties.shortName = "vtx";
    properties.noInput = true;
    properties.protocols << "vtx";
    return properties;
}

Decoder *DecoderVTXFactory::create(const QString &path, QIODevice *)
{
    return new DecoderVTX(path);
}

QList<TrackInfo *> DecoderVTXFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
    {
        return QList<TrackInfo *>() << info;
    }

    VTXHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo *>();
    }

    if(parts & TrackInfo::MetaData)
    {
        helper.readMetaTags();
        info->setValue(Qmmp::ALBUM, helper.album());
        info->setValue(Qmmp::ARTIST, helper.artist());
        info->setValue(Qmmp::COMMENT, helper.comment());
        info->setValue(Qmmp::TITLE, helper.title());
        info->setValue(Qmmp::TRACK, helper.tracker());
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

MetaDataModel* DecoderVTXFactory::createMetaDataModel(const QString &path, bool)
{
    return new VTXMetaDataModel(path);
}

Q_EXPORT_PLUGIN2(vtx,DecoderVTXFactory)
