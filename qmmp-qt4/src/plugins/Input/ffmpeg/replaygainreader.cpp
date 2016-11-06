/***************************************************************************
 *   Copyright (C) 2016 by Ilya Kotov                                      *
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

#include "replaygainreader.h"

ReplayGainReader::ReplayGainReader(AVFormatContext *ic)
{
    AVDictionaryEntry *t = 0;
    while((t = av_dict_get(ic->metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    {
        if(!strcmp(t->key, "replaygain_album_gain"))
            setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN, t->value);
        else if(!strcmp(t->key, "replaygain_album_peak"))
            setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK, t->value);
        else if(!strcmp(t->key, "replaygain_track_gain"))
            setValue(Qmmp::REPLAYGAIN_TRACK_GAIN, t->value);
        else if(!strcmp(t->key, "replaygain_track_peak"))
            setValue(Qmmp::REPLAYGAIN_TRACK_PEAK, t->value);
    }
}

QMap <Qmmp::ReplayGainKey, double> ReplayGainReader::replayGainInfo() const
{
    return m_values;
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
