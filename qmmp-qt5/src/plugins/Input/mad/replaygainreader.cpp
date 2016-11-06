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

#include <QtGlobal>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/tag.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2header.h>
#include <taglib/textidentificationframe.h>
#include <taglib/tfilestream.h>
#include <taglib/id3v2framefactory.h>
#include "replaygainreader.h"

ReplayGainReader::ReplayGainReader(const QString &path)
{
    TagLib::FileStream stream(QStringToFileName(path), true);
    TagLib::MPEG::File fileRef(&stream, TagLib::ID3v2::FrameFactory::instance());
    if(fileRef.ID3v2Tag())
        readID3v2(fileRef.ID3v2Tag());
    if(m_values.isEmpty() && fileRef.APETag())
        readAPE(fileRef.APETag());
}

QMap <Qmmp::ReplayGainKey, double> ReplayGainReader::replayGainInfo() const
{
    return m_values;
}

void ReplayGainReader::readID3v2(TagLib::ID3v2::Tag *tag)
{
    TagLib::ID3v2::UserTextIdentificationFrame* frame = 0;
    TagLib::ID3v2::FrameList frames = tag->frameList("TXXX");
    for(TagLib::ID3v2::FrameList::Iterator it = frames.begin(); it != frames.end(); ++it)
    {
        frame = dynamic_cast<TagLib::ID3v2::UserTextIdentificationFrame*>(*it);
        if(frame && frame->fieldList().size() >= 2)
        {
            TagLib::String desc = frame->description().upper();
            if (desc == "REPLAYGAIN_TRACK_GAIN")
                setValue(Qmmp::REPLAYGAIN_TRACK_GAIN, TStringToQString(frame->fieldList()[1]));
            else if (desc == "REPLAYGAIN_TRACK_PEAK")
                setValue(Qmmp::REPLAYGAIN_TRACK_PEAK, TStringToQString(frame->fieldList()[1]));
            else if (desc == "REPLAYGAIN_ALBUM_GAIN")
                setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN, TStringToQString(frame->fieldList()[1]));
            else if (desc == "REPLAYGAIN_ALBUM_PEAK")
                setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK, TStringToQString(frame->fieldList()[1]));
        }
    }
}

void ReplayGainReader::readAPE(TagLib::APE::Tag *tag)
{
    TagLib::APE::ItemListMap items = tag->itemListMap();
    if (items.contains("REPLAYGAIN_TRACK_GAIN"))
        setValue(Qmmp::REPLAYGAIN_TRACK_GAIN,TStringToQString(items["REPLAYGAIN_TRACK_GAIN"].values()[0]));
    if (items.contains("REPLAYGAIN_TRACK_PEAK"))
        setValue(Qmmp::REPLAYGAIN_TRACK_PEAK,TStringToQString(items["REPLAYGAIN_TRACK_PEAK"].values()[0]));
    if (items.contains("REPLAYGAIN_ALBUM_GAIN"))
        setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN,TStringToQString(items["REPLAYGAIN_ALBUM_GAIN"].values()[0]));
    if (items.contains("REPLAYGAIN_ALBUM_PEAK"))
        setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK,TStringToQString(items["REPLAYGAIN_ALBUM_PEAK"].values()[0]));
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
