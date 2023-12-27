/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
 ***************************************************************************/

#ifndef DCAHELPER_H
#define DCAHELPER_H

#include <QFile>
extern "C" {
#include <stdio_file.h>
#include <libdca/dca.h>
#include <libdca/gettimeofday.h>
}

#define BUFFER_SIZE 65536
#define OUT_BUFFER_SIZE 25000   // one block may be up to 22K samples, which is 88Kb for stereo
#define HEADER_SIZE 14

struct decode_info
{
    FILE *file;
    int64_t offset;
    int64_t end_sample;
    int64_t current_sample;
    dca_state_t *state;
    int disable_adjust;// = 0;
    float gain;// = 1;
    int disable_dynrng;// = 0;
    uint8_t inbuf[BUFFER_SIZE]; // input data buffer
    uint8_t buf[BUFFER_SIZE]; // decoder data buffer (inbuf gets appended here)
    uint8_t *bufptr;// = buf;
    uint8_t *bufpos;// = buf + HEADER_SIZE;
    int sample_rate;
    int frame_length;
    int flags;
    int bitrate;
    int frame_byte_size;
    int16_t output_buffer[OUT_BUFFER_SIZE*6]; // output samples
    int remaining;
    int samples_to_skip;
    int length;
    int channels;
    int bits_per_sample;
};

/*!
 * @author Greedysky <greedysky@163.com>
 */
class DCAHelper
{
public:
    explicit DCAHelper(const QString &path);
    ~DCAHelper();

    void deinit();
    bool initialize();

    void seek(qint64 time);
    inline qint64 totalTime() const { return m_info->length; }

    inline int bitrate() const { return m_info->bitrate / 1000; }
    inline int sampleRate() const { return m_info->sample_rate; }
    inline int channels() const { return m_info->channels; }
    inline int depth() const { return m_info->bits_per_sample; }

    qint64 read(unsigned char *data, qint64 maxSize);

private:
    QString m_path;
    decode_info *m_info = nullptr;

};

#endif
