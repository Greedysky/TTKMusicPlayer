/***************************************************************************
 *   Copyright (C) 2011-2016 by Ilya Kotov                                 *
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


#include <stdio.h>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include "decoder_ffap.h"

//callbacks
size_t ffap_read_cb(void *ptr, size_t size, size_t nmemb, void *client_data)
{
    DecoderFFap *dffap = (DecoderFFap *) client_data;
    return dffap->input()->read((char *) ptr, size * nmemb);
}

int ffap_seek_cb(int64_t offset, int whence, void *client_data)
{
    DecoderFFap *dffap = (DecoderFFap *) client_data;
    bool ok = false;
    switch(whence)
    {
    case SEEK_SET:
        ok = dffap->input()->seek(offset);
        break;
    case SEEK_CUR:
        ok = dffap->input()->seek(dffap->input()->pos() + offset);
        break;
    case SEEK_END:
        ok = dffap->input()->seek(dffap->input()->size() - offset);
        break;
    }
    return ok ? 0 : -1;
}

int64_t ffap_tell_cb(void *client_data)
{
    DecoderFFap *dffap = (DecoderFFap *) client_data;
    return dffap->input()->pos();
}

int64_t ffap_getlength_cb(void *client_data)
{
    DecoderFFap *dffap = (DecoderFFap *) client_data;
    return dffap->input()->size();
}

// Decoder class
DecoderFFap::DecoderFFap(const QString &path, QIODevice *i)
        : Decoder(i)
{
    //m_data = 0;
    m_path = path;
    m_ffap_decoder = 0;
    ffap_load();
}


DecoderFFap::~DecoderFFap()
{
    deinit();
}

bool DecoderFFap::initialize()
{
    m_ffap_decoder = ffap_new(ffap_read_cb, ffap_seek_cb, ffap_tell_cb, ffap_getlength_cb, this);

    if(ffap_init(m_ffap_decoder) == -1)
    {
        deinit();
        qWarning("DecoderFFap: unable to initialize decoder");
        return false;
    }
    Qmmp::AudioFormat format = Qmmp::PCM_UNKNOWM;
    switch(m_ffap_decoder->bps)
    {
    case 8:
        format = Qmmp::PCM_S8;
        break;
    case 16:
        format = Qmmp::PCM_S16LE;
        break;
    case 24:
        format = Qmmp::PCM_S24LE;
        break;
    case 32:
        format = Qmmp::PCM_S32LE;
        break;
    default:
        deinit();
        return false;
    }
    configure(m_ffap_decoder->samplerate, m_ffap_decoder->channels, format);
    qDebug("DecoderFFap: initialize succes");
    return true;
}

qint64 DecoderFFap::totalTime() const
{
    return m_ffap_decoder->duration * 1000;
}

int DecoderFFap::bitrate() const
{
    return m_ffap_decoder->bitrate;
}

void DecoderFFap::seek(qint64 time)
{
    ffap_seek(m_ffap_decoder, time / 1000.0);
}

qint64 DecoderFFap::read(unsigned char *data, qint64 size)
{
    return ffap_read(m_ffap_decoder, data, size);
}

void DecoderFFap::deinit()
{
    if (m_ffap_decoder)
        ffap_free(m_ffap_decoder);
    m_ffap_decoder = 0;
}



/*uint DecoderFFap::findID3v2(char *data, ulong size) //retuns ID3v2 tag size
{
    if (size < 10)
        return 0;
    if (!memcmp(data, "ID3", 3))
    {
        TagLib::ByteVector byteVector(data, size);
        TagLib::ID3v2::Header header(byteVector);
        return header.completeTagSize();
    }
    return 0;
}*/
