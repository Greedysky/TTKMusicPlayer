/* =================================================
 * This file is part of the TTK qmmp plugin project
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

#include "speexhelper.h"

#define readint(buf, base) ( ((buf[base+3] << 24) & 0xff000000) | \
                             ((buf[base+2] << 16) & 0x00ff0000) | \
                             ((buf[base+1] <<  8) & 0x0000ff00) | \
                             ((buf[base+0] <<  0) & 0x000000ff) )

int speex_tagfield_lengths(const char *str, int len, int *itemlen, int *valuelen)
{
    const char *p   = str;
    const char *end = (char *)(str+len);

    while(p < end && *p )
    {
        char ch = *p++;
        if(ch == '=' ) {
            *itemlen = p - str - 1;
            *valuelen = len - (p - str);
            return 1;
        }
    }

    *itemlen  = len;
    *valuelen = 0;

    return 0;
}

SpeexHelper::SpeexHelper(QIODevice *i)
{
    m_initialized = false;
    m_stream  = nullptr;
    m_streamcount = 0;
    m_offset = 0;
    m_decoder = nullptr;
    m_bitrate = 0;
    m_seektosample = -1;
    m_current_stream = 0;
    m_speex_last_error[0] = '\0';
    m_current_serial = -1;
    m_position = 0;
    m_reader = i;
}

SpeexHelper::~SpeexHelper()
{
    close_decoder();
    free_tags();

    for(int32_t i = 0; i < m_streamcount; i++)
    {
        for(int32_t j = 0; j < m_stream[i]->sicount; j++)
        {
            if(m_stream[i]->seekinfo[j])
            {
                free(m_stream[i]->seekinfo[j]);
            }
        }

        if(m_stream[i]->seekinfo)
        {
            free(m_stream[i]->seekinfo);
        }

        if(m_stream[i]->header)
        {
            free(m_stream[i]->header);
        }

        free(m_stream[i]);
    }

    if(m_stream)
    {
        free(m_stream);
        m_stream = nullptr;
    }
}

bool SpeexHelper::initialize()
{
    if(!is_speex())
    {
        return false;
    }

    if(!init_decoder())
    {
        return false;
    }

    if(!initfile())
    {
        return false;
    }

    if(get_streams() != 1)
    {
        return false;
    }

    return m_initialized = true;
}

int SpeexHelper::read(unsigned char *buf, int size)
{
    uint16_t* out = (uint16_t*)buf;

    int total_read = 0;
    while(size > 0)
    {
      // If the buffer is empty, decode a little from the speex file.
      if(m_read_buffer.getSize() == 0)
      {
        float decode_buffer[2000];  // Size defined by speexfile API.
        int speex_read = decode(decode_buffer);
        if(speex_read == 0)
        {
          break;
        }
        m_read_buffer.write(decode_buffer, speex_read * sizeof(float));
      }

      float read_buffer[1024];
      int should_read = std::min(size, 1024);
      int actual_read = m_read_buffer.read(read_buffer, should_read * sizeof(float)) / sizeof(float);

      for(int i = 0; i < actual_read; ++i)
      {
        out[i] = uint16_t(read_buffer[i] * 32767);
      }

      size -= actual_read;
      total_read += actual_read;
      out += actual_read;
    }
    m_position += total_read;
    return total_read;
}

void SpeexHelper::seek(int64_t time)
{
    m_position = time;
    m_reader->seek(0);
    m_offset = 0;
    m_decoder->decoderpos = 0;
    m_decoder->samplepos = 0;
    m_seektosample = time;
    m_current_stream = 0;

    int64_t offs = 0;
    int32_t streamno = 0;
    int64_t sample = 0;
    bool found = false;

    while(streamno < m_streamcount)
    {
        if(time >= stream_get_firstsample(streamno) && time <= stream_get_lastsample(streamno))
        {
            found = true;
            break;
        }
        streamno++;
    }

    if(!found)
    {
        return;
    }

    if(m_stream[streamno]->sicount == 0)
    {
        return;
    }
    found = false;

    if(time >= stream_get_firstsample(streamno) && time <= m_stream[streamno]->seekinfo[0]->sample)
    {
        offs = m_stream[streamno]->seekinfo[0]->offset;
        sample = stream_get_firstsample(streamno);
        found = true;
    }

    if(!found)
    {
        for(int32_t i = 1; i < m_stream[streamno]->sicount; i++)
        {
            if(time > m_stream[streamno]->seekinfo[i - 1]->sample &&
               time <= m_stream[streamno]->seekinfo[i]->sample)
            {
                offs = m_stream[streamno]->seekinfo[i - 1]->offset;
                sample = m_stream[streamno]->seekinfo[i - 1]->sample;
                found = true;
                break;
            }
        }
    }

    if(!found)
    {
        return;
    }

    m_reader->seek(offs);

    m_offset = offs;
    m_decoder->decoderpos = 0;
    m_decoder->samplepos  = sample;
    m_seektosample = time;

    if(m_decoder && (m_current_stream != streamno))
    {
        if(m_decoder->st)    // free old decoder
        {
            speex_decoder_destroy(m_decoder->st);
        }

        int rate;
        m_decoder->st = header_to_decoder(m_stream[streamno]->header, m_decoder->enh_enabled, &m_decoder->frame_size, &rate, &m_decoder->nframes, m_decoder->forceMode, &m_decoder->channels, &m_decoder->stereo);
        if(!m_decoder->st)
        {
            return;
        }
    }

    m_current_stream = streamno;

}

int32_t SpeexHelper::get_stream()
{
    return m_current_stream;
}

int32_t SpeexHelper::get_streams()
{
    return m_streamcount;
}

int64_t SpeexHelper::get_samples()
{
    return stream_get_lastsample(get_streams() - 1);
}

double SpeexHelper::get_duration()
{
    double dur = 0;

    for(int32_t i = 0; i < m_streamcount; i++)
    {
        dur += stream_get_duration(i);
    }

    return dur;
}

double SpeexHelper::get_bitrate()
{
    int64_t size = 0;
    for(int32_t i = 0; i < m_streamcount; i++)
    {
        size += stream_get_size(i);
    }

    double duration = get_duration();
    if(duration == 0.0)
    {
        return 0.0;
    }

    return (double)(size * 8.0) / (double)duration;
}

int32_t SpeexHelper::stream_get_samplerate(int32_t stream)
{
    if(stream < 0)
    {
        stream = get_stream();
    }

    if(stream >= m_streamcount)
    {
        return 0;
    }

    if(!m_stream[stream]->header)
    {
        return 0;
    }

    return m_stream[stream]->header->rate;
}

int32_t SpeexHelper::stream_get_channels(int32_t stream)
{
    if(stream < 0)
    {
        stream = get_stream();
    }

    if(stream >= m_streamcount)
    {
        return 0;
    }

    if(!m_stream[stream]->header)
    {
        return 0;
    }

    return m_stream[stream]->header->nb_channels;
}

int64_t SpeexHelper::stream_get_firstsample(int32_t stream)
{
    if(stream < 0)
    {
        stream = get_stream();
    }

    if(stream >= m_streamcount)
    {
        return 0;
    }

    if(stream == 0)
    {
        return 0; // first sample in first stream is 0
    }

    if(m_stream[stream - 1]->sicount == 0)
    {
        return 0;
    }

    return m_stream[stream - 1]->seekinfo[m_stream[stream - 1]->sicount - 1]->sample;
}

int64_t SpeexHelper::stream_get_lastsample(int32_t stream)
{
    if(stream < 0)
    {
        stream = get_stream();
    }

    if(stream >= m_streamcount)
    {
        return 0;
    }

    if(m_stream[stream]->sicount == 0)
    {
        return 0;
    }

    return m_stream[stream]->seekinfo[m_stream[stream]->sicount - 1]->sample;
}

int64_t SpeexHelper::stream_get_samples(int32_t stream)
{
    return stream_get_lastsample(stream) - stream_get_firstsample(stream);
}

double SpeexHelper::stream_get_duration(int32_t stream)
{
    if(stream < 0)
    {
        stream = get_stream();
    }

    if(stream >= m_streamcount)
    {
        return 0;
    }

    if(stream_get_samplerate(stream) == 0)
    {
        return 0;
    }

    return double(stream_get_samples(stream) / stream_get_samplerate(stream));
}

double SpeexHelper::stream_get_bitrate(int32_t stream)
{
    if(stream < 0)
    {
        stream = get_stream();
    }

    if(stream >= m_streamcount)
    {
        return 0.0;
    }

    double dur = stream_get_duration(stream);
    if(dur == 0.0)
    {
        return 0.0;
    }

    int64_t size = stream_get_size(stream);
    return (double)(size * 8.0) / (double)dur;
}

int64_t SpeexHelper::stream_get_size(int32_t stream)
{
    if(stream < 0)
    {
        stream = get_stream();
    }

    if(stream >= m_streamcount)
    {
        return 0;
    }

    return m_stream[stream]->streamsize;
}

const SpeexHeader *SpeexHelper::stream_get_speexheader(int32_t stream)
{
    if(stream < 0)
    {
        stream = get_stream();
    }

    if(stream >= m_streamcount)
    {
        return 0;
    }

    return m_stream[stream]->header;
}

const speextags **SpeexHelper::stream_get_tags(int32_t stream)
{
    if(stream < 0)
    {
        stream = get_stream();
    }

    if(stream >= m_streamcount )
    {
        return nullptr;
    }

    return (const speextags **)m_stream[stream]->tags;
}

int32_t SpeexHelper::stream_get_tagcount(int32_t stream)
{
    if(stream < 0)
    {
        stream = get_stream();
    }

    if(stream >= m_streamcount)
    {
        return 0;
    }

    return m_stream[stream]->tagcount;
}

bool SpeexHelper::is_speex()
{
    ogg_sync_state oy;
    ogg_stream_state os;
    ogg_page og;
    ogg_packet op;

    ogg_sync_init(&oy);

    char *data = ogg_sync_buffer(&oy, 200);
    if(!data)
    {
        ogg_sync_clear(&oy);
        return false;
    }

    int read = m_reader->read(data, 200);
    if(read <= 0)
    {
        ogg_sync_clear(&oy);
        return false;
    }

    ogg_sync_wrote(&oy, read);

    bool init = false;
    bool is_our_file = false;

    while(ogg_sync_pageout(&oy, &og) == 1 && !is_our_file)
    {
        if(!init)
        {
            ogg_stream_init(&os, ogg_page_serialno(&og));
            init = true;
        }

        ogg_stream_pagein(&os, &og);

        while(ogg_stream_packetout(&os, &op) == 1)
        {
            if(!memcmp(op.packet, "Speex ", 6))
            {
                is_our_file = true;
                break;
            }
        }
    }

    if(init)
    {
        ogg_stream_clear(&os);
    }

    ogg_sync_clear(&oy);
    m_reader->seek(0);

    return is_our_file;
}

bool SpeexHelper::initfile()
{
    char *data;
    uint32_t nb_read;

    bool init = false;
    bool eos = true;
    bool eof = false;

    ogg_sync_state oy;
    ogg_stream_state os;
    ogg_page og;
    ogg_packet op;
    int64_t chainsamples = 0;

    ogg_sync_init(&oy);

    while(!eof)
    {
        // Get the ogg buffer for writing
        data = ogg_sync_buffer(&oy, 200);
        // Read bitstream from input file
        nb_read = m_reader->read(data, 200);
        if(nb_read < 200)
        {
            eof = true;
        }

        ogg_sync_wrote(&oy, nb_read);
        while(ogg_sync_pageout(&oy, &og) == 1)
        {
            if(!init)
            {
                ogg_stream_init(&os, ogg_page_serialno(&og));
                init = true;
            }

            // Add page to the bitstream
            ogg_stream_pagein(&os, &og);
            // Extract all available packets
            while(ogg_stream_packetout(&os, &op) == 1)
            {
                if(op.b_o_s)
                {
                    if(!eos)
                    {
                        // previous stream in chain was broken
                    }
                    eos = false;

                    void *stream_b = (void *)m_stream;
                    m_stream = (speexstream_t **)realloc(m_stream, (m_streamcount + 1) * sizeof(speexstream_t*));
                    if(!m_stream)
                    {
                        m_stream = (speexstream_t **)stream_b;
                        strcpy(m_speex_last_error, "Memory allocation failed");
                        return -1;
                    }

                    m_stream[m_streamcount] = (speexstream_t *)calloc(1, sizeof(speexstream_t));
                    if(!m_stream[m_streamcount])
                    {
                        strcpy(m_speex_last_error, "Memory allocation failed");
                        return -1;
                    }

                    m_stream[m_streamcount]->seekinfo = (speexseekinfo_t **)calloc(1, sizeof(speexseekinfo_t*));
                    if(!m_stream[m_streamcount]->seekinfo)
                    {
                        strcpy(m_speex_last_error, "Memory allocation failed");
                        return -1;
                    }

                    m_stream[m_streamcount]->header = speex_packet_to_header((char *)op.packet, op.bytes);
                    if(!m_stream[m_streamcount]->header)
                    {
                        strcpy(m_speex_last_error, "Cannot read header");
                        return -1;
                    }

                    m_stream[m_streamcount]->tags = nullptr;
                    m_stream[m_streamcount]->tagcount = 0;

                    if((m_streamcount > 0) && (m_stream[m_streamcount - 1]->sicount > 0))
                    {
                        chainsamples += m_stream[m_streamcount - 1]->seekinfo[m_stream[m_streamcount - 1]->sicount - 1]->sample;
                    }
                    m_streamcount++;
                }

                if(op.e_o_s )
                {
                    eos  = true;
                    if(init)
                    {
                        ogg_stream_clear(&os);
                    }
                    init = false;
                }

                if(op.packetno == 1)
                {
                    // extract tags
                    if(readMetaTags((char *)op.packet, op.bytes) != 0)
                    {
                        stream_free_tags(m_streamcount - 1);
                    }
                }
                else if((op.packetno > 1) && (op.granulepos > 0) && (m_streamcount > 0))
                {
                    const int32_t spos = m_streamcount - 1;
                    void *seekinfo_b = (void *)m_stream[spos]->seekinfo;

                    m_stream[spos]->seekinfo = (speexseekinfo_t **)realloc(m_stream[spos]->seekinfo, (m_stream[spos]->sicount + 1) * sizeof(speexseekinfo_t*));
                    if(!m_stream[spos]->seekinfo)
                    {
                        m_stream[spos]->seekinfo = (speexseekinfo_t **)seekinfo_b;
                        strcpy(m_speex_last_error, "Memory allocation failed");
                        return -1;
                    }

                    m_stream[spos]->seekinfo[m_stream[spos]->sicount] = (speexseekinfo_t*)malloc(sizeof(speexseekinfo_t));
                    if(!m_stream[spos]->seekinfo[m_stream[spos]->sicount])
                    {
                        strcpy(m_speex_last_error, "Memory allocation failed");
                        return -1;
                    }

                    m_stream[spos]->seekinfo[m_stream[spos]->sicount]->offset = m_offset;
                    m_stream[spos]->seekinfo[m_stream[spos]->sicount]->sample = chainsamples + op.granulepos;

                    m_stream[spos]->streamsize += og.header_len + og.body_len;
                    m_stream[spos]->sicount++;
                }
            }
        }

        m_offset += nb_read;
    }

    if(init)
    {
        ogg_stream_clear(&os);
    }
    ogg_sync_clear(&oy);

    if(m_streamcount == 0)
    {
        strcpy(m_speex_last_error, "Not a Speex stream");
        return -1;
    }

    m_reader->seek(0);
    m_offset = 0;

    return 0;
}

bool SpeexHelper::init_decoder()
{
    if(m_decoder != nullptr)
    {
        return false;
    }

    m_decoder = (speexdecoder *)calloc(1, sizeof(speexdecoder));
    if(!m_decoder)
    {
        strcpy(m_speex_last_error, "Memory allocation failed");
        return false;
    }

    m_decoder->st = nullptr;
    m_decoder->enh_enabled = 1;    // default since 1.0beta4
    m_decoder->nframes = 2;
    m_decoder->forceMode = -1;
    m_decoder->channels = -1;
    m_decoder->stereo.balance = 1;
    m_decoder->stereo.e_ratio = 0.5;

    speex_bits_init(&m_decoder->bits);

    return true;
}

int SpeexHelper::decode(float *buffer)
{
    float decode_buffer[2000];

    if(m_decoder->decoderpos == 1)
    {
        goto decpos1;
    }
    else if(m_decoder->decoderpos == 2)
    {
        goto decpos2;
    }

    ogg_sync_init(&m_decoder->oy);

    m_decoder->init = false;
    m_decoder->eos = true;
    m_decoder->eof = false;

    while(!m_decoder->eof)
    {
        // Get the ogg buffer for writing
        m_decoder->data = ogg_sync_buffer(&m_decoder->oy, 200);
        // Read bitstream from input file
        m_decoder->nb_read = m_reader->read(m_decoder->data, 200);
        if(m_decoder->nb_read < 200)
        {
            m_decoder->eof = true;
        }

        ogg_sync_wrote(&m_decoder->oy, m_decoder->nb_read);

        while(ogg_sync_pageout(&m_decoder->oy, &m_decoder->og) == 1)
        {
            if(!m_decoder->init)
            {
                m_current_serial = ogg_page_serialno(&m_decoder->og);
                ogg_stream_init(&m_decoder->os, m_current_serial);
                m_decoder->init = true;
            }

            // Add page to the bitstream
            ogg_stream_pagein(&m_decoder->os, &m_decoder->og);

            // Extract all available packets
            while(ogg_stream_packetout(&m_decoder->os, &m_decoder->op) == 1)
            {
                if(m_decoder->op.b_o_s || (m_current_serial != ogg_page_serialno(&m_decoder->og)))
                {
                    if(!m_decoder->eos)
                    {
                        // previous stream in chain was broken
                        m_decoder->init = false;
                        m_current_stream++;
                    }
                    m_decoder->eos = false;

                    int rate;
                    if(m_decoder->st)    // free old decoder
                    {
                        speex_decoder_destroy(m_decoder->st);
                    }

                    m_decoder->st = header_to_decoder(m_stream[m_current_stream]->header, m_decoder->enh_enabled, &m_decoder->frame_size, &rate, &m_decoder->nframes, m_decoder->forceMode, &m_decoder->channels, &m_decoder->stereo);
                    if(!m_decoder->st)
                    {
                        return -1;
                    }
                }

                if(m_decoder->op.packetno > 1)
                {
                    // header + comments skipped
                    // Copy Ogg packet to Speex bitstream
                    speex_bits_read_from(&m_decoder->bits, (char *)m_decoder->op.packet, m_decoder->op.bytes);
                    m_decoder->decoderpos = 1;
                    m_decoder->j = 0;
decpos1:
                    while(m_decoder->j < m_decoder->nframes)
                    {
                        m_decoder->j++;
                        m_decoder->samplepos += m_decoder->frame_size;

                        long skipsamples = 0;
                        if(m_seektosample > 0 )
                        {
                            skipsamples = (long)(m_seektosample + m_decoder->frame_size - m_decoder->samplepos);
                        }

                        if(skipsamples >= m_decoder->frame_size)
                        {
                            skipsamples -= m_decoder->frame_size;
                            continue;
                        }

                        //Decode frame
                        speex_decode(m_decoder->st, &m_decoder->bits, decode_buffer);
                        if(m_decoder->channels == 2)
                        {
                            speex_decode_stereo(decode_buffer, m_decoder->frame_size, &m_decoder->stereo);
                        }

                        //speex_decoder_ctl ( decoder->st, SPEEX_GET_BITRATE, &bitrate );

                        for(int i = 0; i < m_decoder->frame_size * m_decoder->channels; i++)
                        {
                            buffer[i] = (float)decode_buffer[i] / 32768.0f;
                        }

                        if(skipsamples > 0)
                        {
                            long ret = (m_decoder->frame_size - skipsamples);
                            float *bufptr = (float *)(buffer + skipsamples * m_decoder->channels);
                            memmove(buffer, bufptr, (m_decoder->frame_size - skipsamples) * m_decoder->channels * sizeof(float));
                            skipsamples = 0;
                            return ret;
                        }
                        return m_decoder->frame_size;
                    }

                    m_decoder->decoderpos = 2;
decpos2:
                    // End of stream condition
                    if(m_decoder->op.e_o_s)
                    {
                        m_decoder->eos  = true;
                        m_decoder->init = false;
                        m_current_stream++;
                    }
                }
            }
        }
        m_offset += m_decoder->nb_read;
    }

    if(m_decoder->init)
    {
        ogg_stream_clear(&m_decoder->os);
    }

    ogg_sync_clear(&m_decoder->oy);
    m_decoder->eof = true;
    return 0;
}

int SpeexHelper::close_decoder()
{
    if(!m_decoder)
    {
        return 0;
    }

    speex_bits_destroy(&m_decoder->bits);
    if(m_decoder->st)
    {
        speex_decoder_destroy(m_decoder->st);
        m_decoder->st = nullptr;
    }

    free(m_decoder);
    m_decoder = nullptr;

    return 0;
}

int SpeexHelper::readMetaTags(char *tagdata, long size)
{
    if(m_streamcount < 1)
    {
        return -1;
    }

    if(size <= 0)
    {
        return -1;
    }

    if(m_stream[m_streamcount - 1]->tags)
    {
        free(m_stream[m_streamcount - 1]->tags);
    }

    m_stream[m_streamcount - 1]->tagcount = 0;
    m_stream[m_streamcount - 1]->tags = (speextags**)malloc(sizeof(speextags *));
    if(m_stream[m_streamcount - 1]->tags == nullptr)
    {
        strcpy(m_speex_last_error, "Memory allocation failed");
        return -1;
    }

    char *c = tagdata;
    char *item, *value;
    char *end = (char *)(tagdata+size);
    int len, ilen, vlen, nb_fields;

    if(c + 4 > end)
    {
        return -1;
    }

    len = readint(c, 0);
    if(c + len > end)
    {
        return -1;
    }
    c += 4;

    if(speex_tagfield_lengths(c, len, &ilen, &vlen) != 0)
    {
        item  = c;
        value = c + ilen + 1;
    }
    else
    {
        // first tag field is comment
        item  = "Comment";
        value = c;
        vlen  = len;
        ilen  = strlen(item);
    }

    c += len;
    if(c + 4 > end)
    {
        return -1;
    }
    nb_fields = readint(c, 0);
    c += 4;

    if(nb_fields > 0)
    {
        m_stream[m_streamcount - 1]->tags = (speextags **)realloc(m_stream[m_streamcount - 1]->tags, (nb_fields + 1) * sizeof(speextags *));
        if(m_stream[m_streamcount - 1]->tags == nullptr)
        {
            strcpy(m_speex_last_error, "Memory allocation failed");
            return -1;
        }

        for(int i = 0; i < nb_fields; i++)
        {
            if(c + 4 > end)
            {
                return -1;
            }

            len = readint(c, 0);
            if(c + len > end)
            {
                return -1;
            }
            c += 4;

            m_stream[m_streamcount - 1]->tags[m_stream[m_streamcount - 1]->tagcount] = (speextags *)calloc(1, sizeof(speextags));
            if(m_stream[m_streamcount - 1]->tags[m_stream[m_streamcount - 1]->tagcount] == nullptr)
            {
                strcpy(m_speex_last_error, "Memory allocation failed");
                return -1;
            }

            if(speex_tagfield_lengths(c, len, &ilen, &vlen) != 0)
            {
                item  = c;
                value = c + ilen + 1;
            }
            else
            {
                item  = c;
                value = nullptr;
            }

            if(item)
            {
                m_stream[m_streamcount - 1]->tags[m_stream[m_streamcount - 1]->tagcount]->item = (char *)malloc(ilen + 1);
                if(m_stream[m_streamcount - 1]->tags[m_stream[m_streamcount - 1]->tagcount]->item == nullptr)
                {
                    strcpy(m_speex_last_error, "Memory allocation failed");
                    return -1;
                }
                memcpy(m_stream[m_streamcount - 1]->tags[m_stream[m_streamcount - 1]->tagcount]->item, item, ilen);
                m_stream[m_streamcount - 1]->tags[m_stream[m_streamcount - 1]->tagcount]->item[ilen] = '\0';
            }
            else
            {
                m_stream[m_streamcount - 1]->tags[m_stream[m_streamcount - 1]->tagcount]->item = nullptr;
            }

            if(value)
            {
                m_stream[m_streamcount - 1]->tags[m_stream[m_streamcount - 1]->tagcount]->value = (char *)malloc(vlen + 1);
                if(m_stream[m_streamcount - 1]->tags[m_stream[m_streamcount - 1]->tagcount]->value == nullptr)
                {
                    strcpy(m_speex_last_error, "Memory allocation failed");
                    return -1;
                }
                memcpy(m_stream[m_streamcount - 1]->tags[m_stream[m_streamcount - 1]->tagcount]->value, value, vlen);
                m_stream[m_streamcount - 1]->tags[m_stream[m_streamcount - 1]->tagcount]->value[vlen] = '\0';
            }
            else
            {
                m_stream[m_streamcount - 1]->tags[m_stream[m_streamcount - 1]->tagcount]->value = nullptr;
            }
            c += len;
            m_stream[m_streamcount - 1]->tagcount++;
        }
    }

    return 0;
}

void SpeexHelper::free_tags()
{
    for(int32_t i = 0; i < m_streamcount; i++)
    {
        stream_free_tags(i);
    }
}

void SpeexHelper::stream_free_tags(int32_t stream)
{
    if(stream >= m_streamcount || !m_stream)
    {
        return;
    }

    for(int32_t i = 0; i < m_stream[stream]->tagcount; i++)
    {
        if(m_stream[stream]->tags[i])
        {
            if(m_stream[stream]->tags[i]->item)
            {
                free(m_stream[stream]->tags[i]->item);
                m_stream[stream]->tags[i]->item = nullptr;
            }

            if(m_stream[stream]->tags[i]->value)
            {
                free(m_stream[stream]->tags[i]->value);
                m_stream[stream]->tags[i]->value = nullptr;
            }

            free(m_stream[stream]->tags[i]);
            m_stream[stream]->tags[i] = nullptr;
        }
    }

    if(m_stream[stream]->tags)
    {
        free(m_stream[stream]->tags);
        m_stream[stream]->tags = nullptr;
    }
}

void *SpeexHelper::header_to_decoder(SpeexHeader *header, int enh_enabled, int *frame_size, int *rate, int *nframes, int forceMode, int *channels, SpeexStereoState *stereo)
{
    void *st;
    const SpeexMode *mode;
    SpeexCallback callback;
    int modeID;

    if(!header)
    {
        strcpy(m_speex_last_error, "Cannot read header");
        return nullptr;
    }

    if(header->mode >= SPEEX_NB_MODES)
    {
        strcpy(m_speex_last_error, "Mode does not exist (any longer) in this version");
        return nullptr;
    }

    modeID = header->mode;
    if(forceMode != -1)
    {
        modeID = forceMode;
    }
    mode = speex_mode_list[modeID];

    if(mode->bitstream_version < header->mode_bitstream_version)
    {
        strcpy(m_speex_last_error, "The file was encoded with a newer version of Speex.\nYou need to upgrade in order to play it.");
        return nullptr;
    }
    if(mode->bitstream_version > header->mode_bitstream_version)
    {
        strcpy(m_speex_last_error, "The file was encoded with an older version of Speex.\nYou would need to downgrade the version in order to play it.");
        return nullptr;
    }

    st = speex_decoder_init(mode);
    speex_decoder_ctl(st, SPEEX_SET_ENH, &enh_enabled);
    speex_decoder_ctl(st, SPEEX_GET_FRAME_SIZE, frame_size);

    callback.callback_id = SPEEX_INBAND_STEREO;
    callback.func = speex_std_stereo_request_handler;
    callback.data = stereo;
    speex_decoder_ctl(st, SPEEX_SET_HANDLER, &callback);

    *rate = header->rate;
    if(forceMode != -1)
    {
        if(header->mode < forceMode)
        {
            *rate <<= (forceMode - header->mode);
        }

        if(header->mode > forceMode)
        {
            *rate >>= (header->mode - forceMode);
        }
    }

    *nframes = header->frames_per_packet;
    if(*channels == -1)
    {
        *channels = header->nb_channels;
    }

    return st;
}
