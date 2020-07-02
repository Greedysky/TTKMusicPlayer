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

#include "decoderymfactory.h"
#include "decoder_ym.h"

bool DecoderYmFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderYmFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("YM Plugin");
    properties.filters << "*.ym";
    properties.description = tr("ST-Sound, ATARI-ST Audio Files");
    properties.shortName = "ym";
    properties.noInput = true;
    properties.protocols << "file";
    return properties;
}

Decoder *DecoderYmFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderYm(path);
}

QList<TrackInfo*> DecoderYmFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    CYmMusic *music = new CYmMusic;
    if(!music->load(path.toLocal8Bit().constData()))
    {
        delete info;
        return QList<TrackInfo*>();
    }

    ymMusicInfo_t musicInfo;
    music->getMusicInfo(&musicInfo);

    if(parts & TrackInfo::MetaData)
    {
        char* title = strdup(musicInfo.pSongName);
        char* composer = strdup(musicInfo.pSongAuthor);
        char* comment = strdup(musicInfo.pSongComment);

        info->setValue(Qmmp::TITLE, QString::fromUtf8(title).trimmed());
        info->setValue(Qmmp::COMPOSER, QString::fromUtf8(composer).trimmed());
        info->setValue(Qmmp::COMMENT, QString::fromUtf8(comment).trimmed());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setDuration(musicInfo.musicTimeInMs);
        info->setValue(Qmmp::FORMAT_NAME, "ym");
    }

    delete music;
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderYmFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(ym, DecoderYmFactory)
#endif
