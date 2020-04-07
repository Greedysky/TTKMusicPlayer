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

#include "ayflyhelper.h"
#include "decoder_ayfly.h"
#include "decoderayflyfactory.h"

bool DecoderAyflyFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderAyflyFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("ALFLY Plugin");
    properties.filters << "*.vtx" << "*.asc" << "*.sqt" << "*psg";
    properties.filters << "*.stc" << "*.stp";
    properties.filters << "*.pt1" << "*.pt2" << "*.pt3" << "*.psc";
    properties.filters << "*.ay" << "*.ym";
    properties.description = tr("AY/YM Audio Files");
    properties.shortName = "alfly";
    properties.noInput = true;
    properties.protocols << "alfly";
    return properties;
}

Decoder *DecoderAyflyFactory::create(const QString &path, QIODevice *)
{
    return new DecoderAyfly(path);
}

QList<TrackInfo *> DecoderAyflyFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
    {
        return QList<TrackInfo *>() << info;
    }

    AyflyHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo *>();
    }

    if(parts & TrackInfo::MetaData)
    {
        helper.readMetaTags();
        info->setValue(Qmmp::ARTIST, helper.artist());
        info->setValue(Qmmp::TITLE, helper.title());
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

MetaDataModel* DecoderAyflyFactory::createMetaDataModel(const QString &, bool)
{
    return nullptr;
}
