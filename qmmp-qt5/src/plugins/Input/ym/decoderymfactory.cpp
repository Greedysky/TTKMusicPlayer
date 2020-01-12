/******************************************************************************
 * Copyright (c) 2015-2016, Georges Thill
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "decoder_ym.h"
#include "decoderymfactory.h"

bool DecoderYmFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderYmFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("YM Plugin");
    properties.filters << "*.ym";
    properties.description = tr("YM Files");
    properties.shortName = "ym";
    properties.hasSettings = false;
    properties.noInput = true;
    properties.protocols << "file";
    return properties;
}

Decoder *DecoderYmFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderYm(path);
}

QList<TrackInfo *> DecoderYmFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    QList <TrackInfo *> list;
    CYmMusic *music = new CYmMusic;
    if(music->load(path.toLocal8Bit().constData()))
    {
        if(parts & (TrackInfo::MetaData | TrackInfo::Properties))
        {
            TrackInfo *info = new TrackInfo(path);
            ymMusicInfo_t musicInfo;
            music->getMusicInfo(&musicInfo);

            char* title = strdup(musicInfo.pSongName);
            char* composer = strdup(musicInfo.pSongAuthor);
            char* comment = strdup(musicInfo.pSongComment);

            info->setValue(Qmmp::TITLE, QString::fromUtf8(title).trimmed());
            info->setValue(Qmmp::COMPOSER, QString::fromUtf8(composer).trimmed());
            info->setValue(Qmmp::COMMENT, QString::fromUtf8(comment).trimmed());
            info->setDuration(musicInfo.musicTimeInSec);
        }
    }

    delete music;
    return list;
}

MetaDataModel* DecoderYmFactory::createMetaDataModel(const QString &, bool)
{
    return nullptr;
}
