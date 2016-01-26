/***************************************************************************
 *   Copyright (C) 2009-2013 by Ilya Kotov                                 *
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

#include <QtGlobal>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/oggflacfile.h>
#include "replaygainreader.h"

ReplayGainReader::ReplayGainReader(const QString &path)
{
    if(path.endsWith(".flac", Qt::CaseInsensitive))
    {
        TagLib::FLAC::File fileRef(path.toLocal8Bit ().constData());
        if(fileRef.xiphComment())
            readVorbisComment(fileRef.xiphComment());
    }
    else if(path.endsWith(".oga", Qt::CaseInsensitive))
    {
        TagLib::Ogg::FLAC::File fileRef(path.toLocal8Bit ().constData());
        if(fileRef.tag())
            readVorbisComment(fileRef.tag());
    }
}

QMap <Qmmp::ReplayGainKey, double> ReplayGainReader::replayGainInfo() const
{
    return m_values;
}

void ReplayGainReader::readVorbisComment(TagLib::Ogg::XiphComment *comment)
{
    TagLib::Ogg::FieldListMap items = comment->fieldListMap();
    if (items.contains("REPLAYGAIN_TRACK_GAIN"))
        setValue(Qmmp::REPLAYGAIN_TRACK_GAIN,TStringToQString(items["REPLAYGAIN_TRACK_GAIN"].front()));
    if (items.contains("REPLAYGAIN_TRACK_PEAK"))
        setValue(Qmmp::REPLAYGAIN_TRACK_PEAK,TStringToQString(items["REPLAYGAIN_TRACK_PEAK"].front()));
    if (items.contains("REPLAYGAIN_ALBUM_GAIN"))
        setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN,TStringToQString(items["REPLAYGAIN_ALBUM_GAIN"].front()));
    if (items.contains("REPLAYGAIN_ALBUM_PEAK"))
        setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK,TStringToQString(items["REPLAYGAIN_ALBUM_PEAK"].front()));
}

void ReplayGainReader::setValue(Qmmp::ReplayGainKey key, QString value)
{
    value.remove(" dB");
    if(value.isEmpty())
        return;
    bool ok;
    double v = value.toDouble(&ok);
    if(ok)
        m_values[key] = v;
}
