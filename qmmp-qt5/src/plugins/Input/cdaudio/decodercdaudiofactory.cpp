/***************************************************************************
 *   Copyright (C) 2009-2019 by Ilya Kotov                                 *
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

#include <QSettings>
#include <cdio/version.h>
#include <cddb/version.h>
#include "decoder_cdaudio.h"
#include "decodercdaudiofactory.h"

DecoderCDAudioFactory::DecoderCDAudioFactory()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    if(settings.value("cdaudio/cddb_server").toByteArray() == "freedb.org")
    {
        settings.setValue("cdaudio/cddb_server", "gnudb.org");
    }
}

bool DecoderCDAudioFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderCDAudioFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("CD Audio Plugin");
    properties.shortName = "cdaudio";
    properties.description = tr("Compact Disc Input and Control Files");
    properties.protocols << "cdda";
    return properties;
}

Decoder *DecoderCDAudioFactory::create(const QString &path, QIODevice *)
{
    return new DecoderCDAudio(path);
}

QList<TrackInfo *> DecoderCDAudioFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    QList<TrackInfo *> list;

    if(path.contains("#"))
        return list;

    QString device_path = path;
    device_path.remove("cdda://");
    QList <CDATrack> tracks = DecoderCDAudio::generateTrackList(device_path, parts);
    foreach(CDATrack t, tracks)
    {
        list << new TrackInfo(t.info);
    }
    return list;
}

MetaDataModel* DecoderCDAudioFactory::createMetaDataModel(const QString &, bool)
{
    return nullptr;
}
