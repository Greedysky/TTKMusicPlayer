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
#include "ttahelper.h"
#include "decoder_tta.h"
#include "decoderttafactory.h"
#include "ttametadatamodel.h"

bool DecoderTTAFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderTTAFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("TTA Plugin");
    properties.filters << "*.tta";
    properties.description = tr("TTA Music Files");
    properties.shortName = "tta";
    properties.noInput = true;
    properties.protocols << "tta";
    return properties;
}

Decoder *DecoderTTAFactory::create(const QString &path, QIODevice *)
{
    return new DecoderTTA(path);
}

QList<TrackInfo *> DecoderTTAFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
    {
        return QList<TrackInfo *>() << info;
    }

    TTAHelper helper(path);
    if(!helper.initialize())
    {
        return QList<TrackInfo *>();
    }

    if(parts & TrackInfo::MetaData)
    {
        const QVariantMap &meta = helper.readTags(stdio_id3v2);
        info->setValue(Qmmp::ALBUM, meta.value("album", QString()));
        info->setValue(Qmmp::ARTIST, meta.value("artist", QString()));
        info->setValue(Qmmp::COMMENT, meta.value("comment", QString()));
        info->setValue(Qmmp::GENRE, meta.value("genre", QString()));
        info->setValue(Qmmp::TITLE, meta.value("title", QString()));
        info->setValue(Qmmp::YEAR, meta.value("year", QString()));
        info->setValue(Qmmp::TRACK, meta.value("track", QString()));
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.samplerate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.bitsPerSample());
        info->setDuration(helper.totalTime() / 1000);
    }

    return QList<TrackInfo *>() << info;
}

MetaDataModel* DecoderTTAFactory::createMetaDataModel(const QString &path, bool)
{
    return new TTAMetaDataModel(path);
}

Q_EXPORT_PLUGIN2(tta,DecoderTTAFactory)
