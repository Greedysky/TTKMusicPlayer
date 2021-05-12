/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2021 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#ifndef DCAHELPER_H
#define DCAHELPER_H

#include <QObject>

extern "C" {
#include "dca.h"
#include "gettimeofday.h"
#include "stdio_file.h"
}

#define BUFFER_SIZE 24576
#define OUT_BUFFER_SIZE 25000
// one block may be up to 22K samples, which is 88Kb for stereo
#define HEADER_SIZE 14

typedef struct {
    FILE *file;
    int offset;
    dca_state_t * state;
    int disable_adjust;// = 0;
    float gain;// = 1;
    int disable_dynrng;// = 0;
    uint8_t inbuf[BUFFER_SIZE]; // input data buffer
    uint8_t buf[BUFFER_SIZE]; // decoder data buffer (inbuf gets appended here)
    uint8_t * bufptr;// = buf;
    uint8_t * bufpos;// = buf + HEADER_SIZE;
    int sample_rate;
    int frame_length;
    int flags;
    int bitrate;
    int frame_byte_size;
    int bits_per_sample;
    int channels;
    int16_t output_buffer[OUT_BUFFER_SIZE*6]; // output samples
    int remaining;
    int length;
    int startsample;
    int endsample;
    int currentsample;
    int samples_to_skip;
} dca_info;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class DCAHelper
{
public:
    explicit DCAHelper(const QString &path);
    ~DCAHelper();

    void close();

    bool initialize();
    int totalTime() const;
    void seek(qint64 time);

    int bitrate() const;
    int sampleRate() const;
    int channels() const;
    int bitsPerSample() const;

    int read(unsigned char *buf, int size);

private:
    QString m_path;
    dca_info *m_info;

};

#endif
