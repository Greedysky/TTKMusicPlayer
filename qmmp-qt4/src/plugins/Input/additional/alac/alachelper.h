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

#ifndef ALACHELPER_H
#define ALACHELPER_H

#include <QObject>

extern "C" {
#include "alac_demux.h"
#include "alac_decomp.h"
#include "alac_stream.h"
}

class ALACHelper
{
public:
    ALACHelper(const QString &path);
    virtual ~ALACHelper();

    void close();

    bool initialize();
    qint64 totalTime() const;
    void seek(qint64 time);

    int bitrate() const;
    int samplerate() const;
    int channels() const;

    int read(unsigned char *data, int size);

private:
    FILE *m_file;
    unsigned char m_buffer[4096 * 4];
    unsigned char m_stream_buffer[4096 * 4 * 4];

    int m_decoded_frames;
    int m_output_bytes;
    alac_file *m_alac;
    demux_res_t m_demux;
    stream_t *m_stream;

    int m_bitrate;
    qint64 m_totalTime;
    QString m_path;

};

#endif
