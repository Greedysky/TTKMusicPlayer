/***************************************************************************
 *   Copyright (C) 2006-2014 by Ilya Kotov                                 *
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

#ifndef DECODER_FLAC_H
#define DECODER_FLAC_H

#include <qmmp/decoder.h>

#include <FLAC/all.h>

#define MAX_SUPPORTED_CHANNELS 2
#define SAMPLES_PER_WRITE 512
#define SAMPLE_BUFFER_SIZE ((FLAC__MAX_BLOCK_SIZE + SAMPLES_PER_WRITE) * MAX_SUPPORTED_CHANNELS * (32/8))

class CUEParser;

struct flac_data
{
    //FLAC__SeekableStreamDecoder *decoder;
    FLAC__StreamDecoder *decoder;
    struct io_stream *stream;
    int bitrate;
    int abort; /* abort playing (due to an error) */

    qint64 length;
    qint64 total_samples;

    FLAC__byte sample_buffer[SAMPLE_BUFFER_SIZE];
    unsigned sample_buffer_fill;

    /* sound parameters */
    unsigned bits_per_sample;
    unsigned sample_rate;
    unsigned channels;

    FLAC__uint64 last_bytes;

    int ok; /* was this stream successfully opened? */
    //struct decoder_error error;
    QIODevice *input;
};

class DecoderFLAC : public Decoder
{
public:
    DecoderFLAC(const QString &path, QIODevice *i);
    virtual ~DecoderFLAC();

    // Standard Decoder API
    bool initialize();
    qint64 totalTime();
    int bitrate();
    qint64 read(char *data, qint64 maxSize);
    void seek(qint64 time);
    const QString nextURL() const;
    void next();

    struct flac_data *data()
    {
        return m_data;
    }

private:

    // helper functions
    void deinit();
    uint findID3v2(char *data, ulong size); //retuns ID3v2 tag size
    ChannelMap findChannelMap(int channels);

    struct flac_data *m_data;
    qint64 length_in_bytes;
    qint64 m_totalBytes;
    qint64 m_offset;
    qint64 m_length;
    QString m_path;
    CUEParser *m_parser;
    int m_track;
    char *m_buf; //buffer for remainig data
    qint64 m_buf_size;
    qint64 m_sz; //sample size
};


#endif // __decoder_flac_h
