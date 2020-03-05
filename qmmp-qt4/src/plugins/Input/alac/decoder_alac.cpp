/*
 * ALAC (Apple Lossless Audio Codec) decoder
 * Copyright (c) 2005 David Hammerton
 * All rights reserved.
 *
 * This is the actual decoder.
 *
 * http://crazney.net/programs/itunes/alac.html
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "decoder_alac.h"
#include "alacwrap.h"

DecoderALAC::DecoderALAC(const QString &path)
    : Decoder()
{
    m_alac = new ALACWrap(path);
}

DecoderALAC::~DecoderALAC()
{
    delete m_alac;
}

bool DecoderALAC::initialize()
{
    if(!m_alac->initialize())
    {
        qDebug("ALACWrap: initialize false");
        return false;
    }

    configure(m_alac->samplerate(), 2, Qmmp::PCM_S16LE);

    qDebug("ALACWrap: initialize success");
    return true;
}

qint64 DecoderALAC::totalTime() const
{
    return m_alac->totalTime();
}

int DecoderALAC::bitrate() const
{
    return m_alac->bitrate();
}

qint64 DecoderALAC::read(unsigned char *data, qint64 size)
{
    return m_alac->read(data, size);
}

void DecoderALAC::seek(qint64 pos)
{
    m_alac->seek(pos);
}
