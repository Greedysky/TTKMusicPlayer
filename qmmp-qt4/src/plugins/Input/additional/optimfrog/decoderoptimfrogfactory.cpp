/***************************************************************************
 *   Copyright (C) 2006-2019 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QtPlugin>
#include "decoderoptimfrogfactory.h"
#include "decoder_optimfrog.h"
#include "optimfroghelper.h"
#include "optimfrogmetadatamodel.h"

bool DecoderOptimFROGFactory::canDecode(QIODevice *input) const
{
    OptimFROGHelper helper(input);
    return helper.initialize();
}

DecoderProperties DecoderOptimFROGFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("OptimFROG Plugin");
    properties.shortName = "optimfrog";
    properties.filters << "*.ofr" << "*.ofs";
    properties.description = tr("OptimFROG Files");
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderOptimFROGFactory::create(const QString &, QIODevice *input)
{
    return new DecoderOptimFROG(input);
}

MetaDataModel *DecoderOptimFROGFactory::createMetaDataModel(const QString &path, bool)
{
    return new OptimFROGMetaDataModel(path);
}

QList<TrackInfo *> DecoderOptimFROGFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
    {
        return QList<TrackInfo *>() << info;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        OptimFROGHelper helper(&file);
        if(!helper.initialize())
        {
            return QList<TrackInfo *>();
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, helper.bitrate());
            info->setValue(Qmmp::SAMPLERATE, helper.rate());
            info->setValue(Qmmp::CHANNELS, helper.channels());
            info->setDuration(helper.length() / 1000);
        }

        if((parts & TrackInfo::MetaData) && helper.hasTags())
        {
            QString value;
            value = helper.getTag("title");
            info->setValue(Qmmp::TITLE, value.replace('\n', "<br>"));
            value = helper.getTag("artist");
            info->setValue(Qmmp::ARTIST, value.replace('\n', "<br>"));
            value = helper.getTag("album");
            info->setValue(Qmmp::ALBUM, value.replace('\n', "<br>"));
            value = helper.getTag("comment");
            info->setValue(Qmmp::COMMENT, value.replace('\n', "<br>"));
            value = helper.getTag("genre");
            info->setValue(Qmmp::GENRE, value.replace('\n', "<br>"));
            value = helper.getTag("composer");
            info->setValue(Qmmp::COMPOSER, value.replace('\n', "<br>"));
            value = helper.getTag("year");
            info->setValue(Qmmp::YEAR, value.replace('\n', "<br>"));
            value = helper.getTag("track");
            info->setValue(Qmmp::TRACK, value.replace('\n', "<br>"));
        }
    }

    return QList<TrackInfo *>() << info;
}

Q_EXPORT_PLUGIN2(optimfrog,DecoderOptimFROGFactory)
