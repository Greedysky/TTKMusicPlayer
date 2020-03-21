/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#ifndef SPEEXHELPER_H
#define SPEEXHELPER_H

extern "C" {
#include <speex/speex.h>
#include <speex/speex_header.h>
#include <speex/speex_stereo.h>
#include <speex/speex_callbacks.h>
#include <ogg/ogg.h>
}
#include <QFile>

typedef struct
{
    char *item;          // tag item name
    char *value;         // tag value (can be NULL)
} speextags;

typedef struct
{
    int frame_size;
    int packet_count;
    int stream_init;
    int enh_enabled;
    int nframes;
    int forceMode;
    int64_t samplepos;
    void *st;
    SpeexBits bits;
    SpeexStereoState stereo;
    int channels;
    ogg_sync_state oy;
    ogg_page og;
    ogg_packet op;
    ogg_stream_state os;

    char *data;
    int i;
    int j;
    uint32_t nb_read;

    bool init;
    bool eos;
    bool eof;

    int decoderpos;
} speexdecoder;

typedef struct
{
    int64_t offset;         // byte offset in file
    int64_t sample;         // sample number in chain
} speexseekinfo_t;

typedef struct
{
    SpeexHeader *header;        // Speex header
    speextags **tags;         // tag fields
    speexseekinfo_t **seekinfo;     // info to help seeking
    int32_t tagcount;       // number of tag fields
    int32_t sicount;        // number of seekpositions
    int64_t streamsize;     // size in bytes
} speexstream_t;


/*!
 * @author Greedysky <greedysky@163.com>
 */
class SpeexHelper
{
public:
    SpeexHelper(QIODevice *i);
    ~SpeexHelper();

    bool initialize();

    int read(void *buf, int size);
    int seek_sample(int64_t time);                     // seek to given sample position

    int32_t get_stream();                                 // return current stream
    int32_t get_streams();                                 // return number of streams
    int64_t get_samples();                                 // return samples in whole chain
    double get_duration();                                 // return duration of whole chain
    double get_bitrate();                                 // return average bitrate of whole chain

    int32_t stream_get_samplerate(int32_t stream = -1);   // return samplerate of given stream number (0 = first stream in chain)
    int32_t stream_get_channels(int32_t stream = -1);   // return number of channels
    int64_t stream_get_firstsample(int32_t stream = -1);   // return first sample in stream
    int64_t stream_get_lastsample(int32_t stream = -1);   // return last sample in stream
    int64_t stream_get_samples(int32_t stream = -1);   // return number of samples
    double stream_get_duration(int32_t stream = -1);   // return duration in seconds
    double stream_get_bitrate(int32_t stream = -1);   // return average bitrate in bits / second
    int64_t stream_get_size(int32_t stream = -1);   // return size in bytes
    int32_t stream_get_tagcount(int32_t stream = -1);   // return number of tag fields
    const speextags** stream_get_tags(int32_t _stream = -1); // return pointer to tag struct
    const SpeexHeader* stream_get_speexheader(int32_t stream = -1); // return pointer to header struct

private:
    class QueueBuffer
    {
    public:
        QueueBuffer()
        {
            m_capacity = 256;
            m_size = 0;
            m_buffer = (uint8_t*)malloc(m_capacity);
        }

        ~QueueBuffer()
        {
            m_buffer = (uint8_t*)realloc(m_buffer, 0);
        }

        int getSize()
        {
            return m_size;
        }

        void write(const void* buffer, int size)
        {
            bool need_realloc = false;
            while(size + m_size > m_capacity)
            {
                m_capacity *= 2;
                need_realloc = true;
            }

            if(need_realloc)
            {
                m_buffer = (uint8_t*)realloc(m_buffer, m_capacity);
            }

            memcpy(m_buffer + m_size, buffer, size);
            m_size += size;
        }

        int read(void* buffer, int size)
        {
            int to_read = std::min(size, m_size);
            memcpy(buffer, m_buffer, to_read);
            memmove(m_buffer, m_buffer + to_read, m_size - to_read);
            m_size -= to_read;
            return to_read;
        }

        void clear()
        {
          m_size = 0;
        }

    private:
        uint8_t* m_buffer;
        int m_capacity;
        int m_size;
        // private and unimplemented to prevent their use
        QueueBuffer(const QueueBuffer&);
        QueueBuffer& operator=(const QueueBuffer&);
    };

private:
    QIODevice *m_reader;       // reader
    speexstream_t **m_stream;       // stream info
    int32_t m_streamcount;    // number of streams
    int64_t m_offset;         // position in file
    int64_t m_seektosample;
    int32_t m_current_stream;

    speexdecoder *m_decoder;       // Speex decoder
    bool m_initialized;    // file init was successful
    long m_bitrate;        // temporary bitrate on vbr files
    char m_speex_last_error[512];
    int32_t m_current_serial;
    int m_position;
    QueueBuffer m_read_buffer;

private:
    bool is_speex();
    bool initfile();                                           // read file header and scan lengths and seek positions
    bool init_decoder();                                      // initialize Speex decoder
    int decode(float *buffer);                      // decode at maximum 2000 samples to buffer
    int close_decoder();                                      // free Speex decoder
    int readtags(char *tagdata, long size);                 // read tags
    void free_tags();                                          // free all tags from memory
    void stream_free_tags(int32_t stream);                  // free tags from specified stream
    void *header_to_decoder(SpeexHeader *header, int enh_enabled, int *frame_size, int *rate, int *nframes, int forceMode, int *channels, SpeexStereoState *stereo);

};

#endif // SPEEXHELPER_H
