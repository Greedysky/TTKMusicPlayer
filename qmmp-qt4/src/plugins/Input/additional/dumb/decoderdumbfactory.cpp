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
#include "dumbhelper.h"
#include "decoder_dumb.h"
#include "decoderdumbfactory.h"

bool DecoderDumbFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderDumbFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("DUMB Plugin");

    const char **exts = dumb_exts();
    for(const char *q = *exts; q != nullptr; )
    {
        properties.filters << QString("*.") + q;
        q = *(++exts);
    }
    properties.description = tr("Dynamic Universal Music Bibliotheque Files");
    properties.shortName = "dumb";
    properties.noInput = true;
    properties.protocols << "dumb";
    return properties;
}

Decoder *DecoderDumbFactory::create(const QString &path, QIODevice *)
{
    return new DecoderDumb(path);
}

QList<TrackInfo *> DecoderDumbFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
    {
        return QList<TrackInfo *>() << info;
    }

    DumbHelper helper(path);
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
        info->setDuration(helper.totalTime() / 1000);
    }

    return QList<TrackInfo *>() << info;
}

MetaDataModel* DecoderDumbFactory::createMetaDataModel(const QString &, bool)
{
    return nullptr;
}

Q_EXPORT_PLUGIN2(dumb,DecoderDumbFactory)
