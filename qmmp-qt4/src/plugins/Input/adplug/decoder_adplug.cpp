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

#include "decoder_adplug.h"

DecoderAdplug::DecoderAdplug(const QString &path)
    : Decoder(),
      path(path)
{

}

bool DecoderAdplug::initialize()
{
    try
    {
        adplug = std::unique_ptr<AdplugWrap>(new AdplugWrap(path.toUtf8().constData()));
    }
    catch(const AdplugWrap::InvalidFile &)
    {
        return false;
    }

    configure(adplug->rate(), adplug->channels(), Qmmp::PCM_S16LE);

    length = adplug->length();
    divisor = (adplug->rate() * adplug->channels() * (adplug->depth() / 8)) / 1000.0;

    return true;
}

qint64 DecoderAdplug::totalTime() const
{
    return adplug->length();
}

int DecoderAdplug::bitrate() const
{
    return adplug->depth();
}

qint64 DecoderAdplug::read(unsigned char *audio, qint64 max_size)
{
    qint64 copied;

    copied = copy(audio, max_size);
    audio += copied;
    max_size -= copied;

    /* Some songs loop endlessly.  If we pass the length threshold (Adplug
    * caps the reported length at 10 minutes), then report EOF.
    */
    if(time > length)
    {
        return 0;
    }

    if(buf_filled == 0)
    {
        AdplugWrap::Frame frame = adplug->read();
        if(frame.n == 0)
        {
            return copied;
        }

        bufptr = frame.buf;
        buf_filled += frame.n;
    }

    copied += copy(audio, max_size);
    time += copied / divisor;
    return copied;
}

qint64 DecoderAdplug::copy(unsigned char *audio, qint64 max_size)
{
    qint64 to_copy = qMin(buf_filled, max_size);
    memcpy(audio, bufptr, to_copy);

    bufptr += to_copy;
    buf_filled -= to_copy;

    return to_copy;
}

void DecoderAdplug::seek(qint64 pos)
{
    adplug->seek(pos);
    time = pos;
}
