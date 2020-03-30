/***************************************************************************
 *   Copyright (C) 2008-2019 by Ilya Kotov                                 *
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
      m_path(path)
{

}

bool DecoderAdplug::initialize()
{
    m_adplug = std::unique_ptr<AdplugHelper>(new AdplugHelper(m_path.toUtf8().constData()));
    if(!m_adplug->initialize())
    {
        return false;
    }
    configure(m_adplug->rate(), m_adplug->channels(), Qmmp::PCM_S16LE);

    m_length = m_adplug->length();
    m_divisor = (m_adplug->rate() * m_adplug->channels() * (m_adplug->depth() / 8)) / 1000.0;

    return true;
}

qint64 DecoderAdplug::totalTime() const
{
    return m_adplug->length();
}

int DecoderAdplug::bitrate() const
{
    return m_adplug->depth();
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
    if(m_time > m_length)
    {
        return 0;
    }

    if(m_buf_filled == 0)
    {
        AdplugHelper::Frame frame = m_adplug->read();
        if(frame.n == 0)
        {
            return copied;
        }

        m_bufptr = frame.buf;
        m_buf_filled += frame.n;
    }

    copied += copy(audio, max_size);
    m_time += copied / m_divisor;
    return copied;
}

qint64 DecoderAdplug::copy(unsigned char *audio, qint64 max_size)
{
    qint64 to_copy = qMin(m_buf_filled, max_size);
    memcpy(audio, m_bufptr, to_copy);

    m_bufptr += to_copy;
    m_buf_filled -= to_copy;

    return to_copy;
}

void DecoderAdplug::seek(qint64 pos)
{
    m_adplug->seek(pos);
    m_time = pos;
}
