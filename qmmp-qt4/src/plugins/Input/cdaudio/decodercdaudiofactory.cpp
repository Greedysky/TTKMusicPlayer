/***************************************************************************
 *   Copyright (C) 2009-2016 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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
#include <QRegExp>
#include <cdio/version.h>
#include <cddb/version.h>
#include "decoder_cdaudio.h"
#include "decodercdaudiofactory.h"


// DecoderCDAudioFactory

bool DecoderCDAudioFactory::supports(const QString &source) const
{
    return source == "/" || source.startsWith("/dev");
}

bool DecoderCDAudioFactory::canDecode(QIODevice *) const
{
    return false;
}

const DecoderProperties DecoderCDAudioFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("CD Audio Plugin");
    properties.shortName = "cdaudio";
    properties.protocols << "cdda";
    properties.hasAbout = true;
    properties.noInput = true;
    properties.hasSettings = true;
    return properties;
}

Decoder *DecoderCDAudioFactory::create(const QString &url, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderCDAudio(url);
}

QList<FileInfo *> DecoderCDAudioFactory::createPlayList(const QString &url, bool useMetaData, QStringList *)
{
    Q_UNUSED(useMetaData);
    QList <FileInfo*> list;
    QString device_path = url;
    device_path.remove("cdda://");
    device_path.remove(QRegExp("#\\d+$"));
    QList <CDATrack> tracks = DecoderCDAudio::generateTrackList(device_path);
    foreach(CDATrack t, tracks)
    {
        list << new FileInfo(t.info);
    }
    return list;
}

MetaDataModel* DecoderCDAudioFactory::createMetaDataModel(const QString &path, QObject *parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(path);
    return 0;
}

Q_EXPORT_PLUGIN2(cdaudio, DecoderCDAudioFactory)
