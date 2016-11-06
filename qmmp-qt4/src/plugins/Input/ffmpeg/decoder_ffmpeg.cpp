/***************************************************************************
 *   Copyright (C) 2006-2016 by Ilya Kotov                                 *
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

#include <QObject>
#include <QFile>
#include "replaygainreader.h"
#include "decoder_ffmpeg.h"
#if (LIBAVCODEC_VERSION_INT >= ((55<<16)+(34<<8)+0)) //libav-10: 55.34.1; ffmpeg-2.1: 55.39.100
extern "C"{
#include <libavutil/channel_layout.h>
#include <libavutil/frame.h>
}
#endif

// callbacks

static int ffmpeg_read(void *data, uint8_t *buf, int size)
{
    DecoderFFmpeg *d = (DecoderFFmpeg*)data;
    return (int)d->input()->read((char*)buf, size);
}

static int64_t ffmpeg_seek(void *data, int64_t offset, int whence)
{
    DecoderFFmpeg *d = (DecoderFFmpeg*)data;
    int64_t absolute_pos = 0;
    /*if(d->input()->isSequential())
        return -1;*/
    switch( whence )
    {
    case AVSEEK_SIZE:
        return d->input()->size();
    case SEEK_SET:
        absolute_pos = offset;
        break;
    case SEEK_CUR:
        absolute_pos = d->input()->pos() + offset;
        break;
    case SEEK_END:
        absolute_pos = d->input()->size() - offset;
    default:
        return -1;
    }
    if(absolute_pos < 0 || absolute_pos > d->input()->size())
        return -1;
    return d->input()->seek(absolute_pos);
}

// Decoder class
DecoderFFmpeg::DecoderFFmpeg(const QString &path, QIODevice *i)
        : Decoder(i)
{
    m_bitrate = 0;
    m_totalTime = 0;
    ic = 0;
    m_path = path;
    m_temp_pkt.size = 0;
    m_pkt.size = 0;
    m_pkt.data = 0;
    m_output_at = 0;
    m_skipBytes = 0;
    m_stream = 0;
    m_decoded_frame = 0;
    m_channels = 0;
    c = 0;
    av_init_packet(&m_pkt);
    av_init_packet(&m_temp_pkt);
}


DecoderFFmpeg::~DecoderFFmpeg()
{
    m_bitrate = 0;
    m_temp_pkt.size = 0;
#if (LIBAVCODEC_VERSION_INT >= ((57<<16)+(48<<8)+0)) //ffmpeg-3.1:  57.48.101
    if(c)
        avcodec_free_context(&c);
#endif
    if (ic)
        avformat_free_context(ic);
    if(m_pkt.data)
#if (LIBAVCODEC_VERSION_INT >= ((57<<16)+(24<<8)+102)) //ffmpeg-3.0
        av_packet_unref(&m_pkt);
#else
        av_free_packet(&m_pkt);
#endif
    if(m_stream)
        av_free(m_stream);

    if(m_decoded_frame)
#if (LIBAVCODEC_VERSION_INT >= ((55<<16)+(34<<8)+0)) //libav-10: 55.34.1; ffmpeg-2.1:  55.39.100
        av_frame_free(&m_decoded_frame);
#else
        av_free(m_decoded_frame);
#endif

}

bool DecoderFFmpeg::initialize()
{
    m_bitrate = 0;
    m_totalTime = 0;
    m_seekTime = -1;

    ic = avformat_alloc_context();

    AVProbeData  pd;
    memset(&pd, 0, sizeof(pd));
    uint8_t buf[PROBE_BUFFER_SIZE + AVPROBE_PADDING_SIZE];
    pd.filename = m_path.toLocal8Bit().constData();
    pd.buf_size = input()->peek((char*)buf, sizeof(buf) - AVPROBE_PADDING_SIZE);
    pd.buf = buf;
    if(pd.buf_size < PROBE_BUFFER_SIZE)
    {
        qWarning("DecoderFFmpeg: too small buffer size: %d bytes", pd.buf_size);
        return false;
    }
    AVInputFormat *fmt = av_probe_input_format(&pd, 1);
    if(!fmt)
    {
        qWarning("DecoderFFmpeg: usupported format");
        return false;
    }
    qDebug("DecoderFFmpeg: detected format: %s", fmt->long_name);
    qDebug("=%s=", fmt->name);

    m_stream = avio_alloc_context(m_input_buf, INPUT_BUFFER_SIZE, 0, this, ffmpeg_read, NULL, ffmpeg_seek);
    if(!m_stream)
    {
        qWarning("DecoderFFmpeg: unable to initialize I/O callbacks");
        return false;
    }
    m_stream->seekable = !input()->isSequential();
    m_stream->max_packet_size = INPUT_BUFFER_SIZE;
    ic->pb = m_stream;

    if(avformat_open_input(&ic, 0, fmt, 0) != 0)
    {
        qDebug("DecoderFFmpeg: avformat_open_input() failed");
        return false;
    }
    avformat_find_stream_info(ic, 0);
    if(ic->pb)
        ic->pb->eof_reached = 0;

    if (input()->isSequential())
    {
        QMap<Qmmp::MetaData, QString> metaData;
        AVDictionaryEntry *album = av_dict_get(ic->metadata,"album",0,0);
        if(!album)
            album = av_dict_get(ic->metadata,"WM/AlbumTitle",0,0);
        AVDictionaryEntry *artist = av_dict_get(ic->metadata,"artist",0,0);
        if(!artist)
            artist = av_dict_get(ic->metadata,"author",0,0);
        AVDictionaryEntry *comment = av_dict_get(ic->metadata,"comment",0,0);
        AVDictionaryEntry *genre = av_dict_get(ic->metadata,"genre",0,0);
        AVDictionaryEntry *title = av_dict_get(ic->metadata,"title",0,0);
        AVDictionaryEntry *year = av_dict_get(ic->metadata,"WM/Year",0,0);
        if(!year)
            year = av_dict_get(ic->metadata,"year",0,0);
        if(!year)
            year = av_dict_get(ic->metadata,"date",0,0);
        AVDictionaryEntry *track = av_dict_get(ic->metadata,"track",0,0);
        if(!track)
            track = av_dict_get(ic->metadata,"WM/Track",0,0);
        if(!track)
            track = av_dict_get(ic->metadata,"WM/TrackNumber",0,0);

        if(album)
            metaData.insert(Qmmp::ALBUM, QString::fromUtf8(album->value).trimmed());
        if(artist)
            metaData.insert(Qmmp::ARTIST, QString::fromUtf8(artist->value).trimmed());
        if(comment)
            metaData.insert(Qmmp::COMMENT, QString::fromUtf8(comment->value).trimmed());
        if(genre)
            metaData.insert(Qmmp::GENRE, QString::fromUtf8(genre->value).trimmed());
        if(title)
            metaData.insert(Qmmp::TITLE, QString::fromUtf8(title->value).trimmed());
        if(year)
            metaData.insert(Qmmp::YEAR, year->value);
        if(track)
            metaData.insert(Qmmp::TRACK, track->value);
        metaData.insert(Qmmp::URL, m_path);
        addMetaData(metaData);
    }

    //replay gain
    ReplayGainReader rg(ic);
    setReplayGainInfo(rg.replayGainInfo());

#if (LIBAVCODEC_VERSION_INT >= ((57<<16)+(48<<8)+0)) //ffmpeg-3.1:  57.48.101
    c = avcodec_alloc_context3(NULL);
#endif

    ic->flags |= AVFMT_FLAG_GENPTS;
    av_read_play(ic);
    for (wma_idx = 0; wma_idx < (int)ic->nb_streams; wma_idx++)
    {
#if (LIBAVCODEC_VERSION_INT >= ((57<<16)+(48<<8)+0)) //ffmpeg-3.1:  57.48.101
        avcodec_parameters_to_context(c, ic->streams[wma_idx]->codecpar);
#else
        c = ic->streams[wma_idx]->codec;
#endif
        if (c->codec_type == AVMEDIA_TYPE_AUDIO)
            break;
    }

#if (LIBAVCODEC_VERSION_INT >= ((55<<16)+(34<<8)+0)) //libav-10: 55.34.1; ffmpeg-2.1:  55.39.100
    if (c->channels == 1)
    {
        c->request_channel_layout = AV_CH_LAYOUT_MONO;
        m_channels = c->channels;
    }
    else
    {
        c->request_channel_layout = AV_CH_LAYOUT_STEREO;
        m_channels = 2;
    }
#else
    if (c->channels > 0)
         c->request_channels = qMin(2, c->channels);
    else
         c->request_channels = 2;
    m_channels = c->request_channels;
#endif

    av_dump_format(ic,0,0,0);

    AVCodec *codec = avcodec_find_decoder(c->codec_id);

    if (!codec)
    {
        qWarning("DecoderFFmpeg: unsupported codec for output stream");
        return false;
    }

    if (avcodec_open2(c, codec, 0) < 0)
    {
        qWarning("DecoderFFmpeg: error while opening codec for output stream");
        return false;
    }

#if (LIBAVCODEC_VERSION_INT >= ((55<<16)+(34<<8)+0)) //libav-10: 55.34.1; ffmpeg-2.1:  55.39.100
    m_decoded_frame = av_frame_alloc();
#else
    m_decoded_frame = avcodec_alloc_frame();
#endif

    m_totalTime = input()->isSequential() ? 0 : ic->duration * 1000 / AV_TIME_BASE;

#if (LIBAVCODEC_VERSION_INT >= ((55<<16)+(34<<8)+0)) //libav 10
    if(c->codec_id == AV_CODEC_ID_SHORTEN) //ffmpeg bug workaround
        m_totalTime = 0;
#else
    if(c->codec_id == CODEC_ID_SHORTEN) //ffmpeg bug workaround
        m_totalTime = 0;
#endif

    Qmmp::AudioFormat format = Qmmp::PCM_UNKNOWM;

    switch(c->sample_fmt)
    {
    case AV_SAMPLE_FMT_U8:
    case AV_SAMPLE_FMT_U8P:
        format = Qmmp::PCM_S8;
        break;
    case AV_SAMPLE_FMT_S16:
    case AV_SAMPLE_FMT_S16P:
        format = Qmmp::PCM_S16LE;
        break;
    case AV_SAMPLE_FMT_S32:
    case AV_SAMPLE_FMT_S32P:
        format = Qmmp::PCM_S32LE;
        break;
    case AV_SAMPLE_FMT_FLT:
    case AV_SAMPLE_FMT_FLTP:
        format = Qmmp::PCM_FLOAT;
        break;
    default:
        qWarning("DecoderFFmpeg: unsupported audio format");
        return false;
    }

    configure(c->sample_rate, m_channels, format);

    if(ic->bit_rate)
        m_bitrate = ic->bit_rate/1000;
    if(c->bit_rate)
        m_bitrate = c->bit_rate/1000;
    qDebug("DecoderFFmpeg: initialize succes");

#ifdef Q_OS_WIN
    qDebug("total time = %I64d", m_totalTime);
#else
    qDebug("total time = %lld ", m_totalTime);
#endif
    return true;
}

qint64 DecoderFFmpeg::totalTime()
{
    return m_totalTime;
}

int DecoderFFmpeg::bitrate()
{
    return m_bitrate;
}

qint64 DecoderFFmpeg::read(unsigned char *audio, qint64 maxSize)
{
    m_skipBytes = 0;

    if(!m_output_at)
        fillBuffer();

    if(!m_output_at)
        return 0;
    qint64 len = qMin(m_output_at, maxSize);

    if(av_sample_fmt_is_planar(c->sample_fmt) && m_channels > 1)
    {
        int bps = av_get_bytes_per_sample(c->sample_fmt);

        for(int i = 0; i < len / bps; i++)
        {
            memcpy(audio + i * bps, m_decoded_frame->extended_data[i % m_channels] + i / m_channels * bps, bps);
        }

        m_output_at -= len;
        for(int i = 0; i < m_channels; i++)
        {
            memmove(m_decoded_frame->extended_data[i],
                    m_decoded_frame->extended_data[i] + len/m_channels, m_output_at/m_channels);
        }
    }
    else
    {
        memcpy(audio, m_decoded_frame->extended_data[0], len);
        m_output_at -= len;
        memmove(m_decoded_frame->extended_data[0], m_decoded_frame->extended_data[0] + len, m_output_at);
    }

    return len;
}

qint64 DecoderFFmpeg::ffmpeg_decode()
{
    int out_size = 0;
    int got_frame = 0;
    if(m_pkt.stream_index == wma_idx)
    {

#if (LIBAVCODEC_VERSION_INT >= ((55<<16)+(34<<8)+0)) //libav-10: 55.34.1; ffmpeg-2.1:  55.39.100

#else
        avcodec_get_frame_defaults(m_decoded_frame);
#endif

#if (LIBAVCODEC_VERSION_INT >= ((57<<16)+(48<<8)+0)) //ffmpeg-3.1:  57.48.101
        int err = 0;
        if((err = avcodec_send_packet(c, &m_temp_pkt)) < 0)
        {
            if(err == EAGAIN) //try again
                return 0;
            else
            {
                qWarning("DecoderFFmpeg: avcodec_send_packet error: %d", err);
                return -1;
            }
        }
        if((err = avcodec_receive_frame(c, m_decoded_frame)) < 0)
        {
            if(err == EAGAIN) //try again
                return 0;
            qWarning("DecoderFFmpeg: avcodec_receive_frame error: %d", err);
            return -1;
        }
        got_frame = av_frame_get_pkt_size(m_decoded_frame);
        int l = m_temp_pkt.size;
#else
        int l = avcodec_decode_audio4(c, m_decoded_frame, &got_frame, &m_temp_pkt);
#endif

        if(got_frame)
            out_size = av_samples_get_buffer_size(0, c->channels, m_decoded_frame->nb_samples,
                                                  c->sample_fmt, 1);
        else
            out_size = 0;

        if(c->bit_rate)
            m_bitrate = c->bit_rate/1000;
        if(l < 0)
        {
            return l;
        }
        m_temp_pkt.data += l;
        m_temp_pkt.size -= l;
    }
    if (!m_temp_pkt.size && m_pkt.data)
#if (LIBAVCODEC_VERSION_INT >= ((57<<16)+(24<<8)+102)) //ffmpeg-3.0
        av_packet_unref(&m_pkt);
#else
        av_free_packet(&m_pkt);
#endif

    return out_size;
}

void DecoderFFmpeg::seek(qint64 pos)
{
   int64_t timestamp = int64_t(pos)*AV_TIME_BASE/1000;
    if (ic->start_time != (qint64)AV_NOPTS_VALUE)
        timestamp += ic->start_time;
    m_seekTime = timestamp;
    av_seek_frame(ic, -1, timestamp, AVSEEK_FLAG_BACKWARD);
    avcodec_flush_buffers(c);
#if (LIBAVCODEC_VERSION_INT >= ((57<<16)+(24<<8)+102)) //ffmpeg-3.0
    av_packet_unref(&m_pkt);
#else
    av_free_packet(&m_pkt);
#endif
    m_temp_pkt.size = 0;
}

void DecoderFFmpeg::fillBuffer()
{
    while(!m_output_at)
    {
        if(!m_temp_pkt.size)
        {
            if (av_read_frame(ic, &m_pkt) < 0)
            {
                m_temp_pkt.size = 0;
                break;
            }
            m_temp_pkt.size = m_pkt.size;
            m_temp_pkt.data = m_pkt.data;

            if(m_pkt.stream_index != wma_idx)
            {
                if(m_pkt.data)
#if (LIBAVCODEC_VERSION_INT >= ((57<<16)+(24<<8)+102)) //ffmpeg-3.0
                    av_packet_unref(&m_pkt);
#else
                    av_free_packet(&m_pkt);
#endif
                m_temp_pkt.size = 0;
                continue;
            }
#if (LIBAVCODEC_VERSION_INT >= ((55<<16)+(34<<8)+0)) //libav 10
            if(m_seekTime && c->codec_id == AV_CODEC_ID_APE)
#else
            if(m_seekTime && c->codec_id == CODEC_ID_APE)
#endif
            {
                int64_t rescaledPts = av_rescale(m_pkt.pts,
                                                 AV_TIME_BASE * (int64_t)
                                                 ic->streams[m_pkt.stream_index]->time_base.num,
                                                 ic->streams[m_pkt.stream_index]->time_base.den);
                m_skipBytes =  (m_seekTime - rescaledPts) * c->sample_rate * 4 / AV_TIME_BASE;
            }
            else
                m_skipBytes = 0;
            m_seekTime = 0;
        }
#if (LIBAVCODEC_VERSION_INT >= ((55<<16)+(34<<8)+0)) //libav 10
        if(m_skipBytes > 0 && c->codec_id == AV_CODEC_ID_APE)
#else
        if(m_skipBytes > 0 && c->codec_id == CODEC_ID_APE)
#endif
        {
            while (m_skipBytes > 0)
            {
                m_output_at = ffmpeg_decode();
                if(m_output_at < 0)
                    break;
                m_skipBytes -= m_output_at;
            }

            if(m_skipBytes < 0)
            {
                qint64 size = m_output_at;
                m_output_at = - m_skipBytes;
                m_output_at = m_output_at - (m_output_at % 4);

                if(av_sample_fmt_is_planar(c->sample_fmt) && m_channels > 1)
                {
                    memmove(m_decoded_frame->extended_data[0],
                            m_decoded_frame->extended_data[0] + (size - m_output_at)/2, m_output_at/2);
                    memmove(m_decoded_frame->extended_data[1],
                            m_decoded_frame->extended_data[1] + (size - m_output_at)/2, m_output_at/2);
                }
                else
                {
                    memmove(m_decoded_frame->extended_data[0],
                            m_decoded_frame->extended_data[0] + size - m_output_at, m_output_at);
                }
                m_skipBytes = 0;
            }
        }
        else
            m_output_at = ffmpeg_decode();

        if(m_output_at < 0)
        {
            m_output_at = 0;
            m_temp_pkt.size = 0;

#if (LIBAVCODEC_VERSION_INT >= ((55<<16)+(34<<8)+0)) //libav 10
            if(c->codec_id == AV_CODEC_ID_SHORTEN || c->codec_id == AV_CODEC_ID_TWINVQ)
#else
            if(c->codec_id == CODEC_ID_SHORTEN || c->codec_id == CODEC_ID_TWINVQ)
#endif
            {
                if(m_pkt.data)
#if (LIBAVCODEC_VERSION_INT >= ((57<<16)+(24<<8)+102)) //ffmpeg-3.0
                    av_packet_unref(&m_pkt);
#else
                    av_free_packet(&m_pkt);
#endif
                m_pkt.data = 0;
                m_temp_pkt.size = 0;
                break;
            }
            continue;
        }
        else if(m_output_at == 0)
        {
            if(m_pkt.data)
#if (LIBAVCODEC_VERSION_INT >= ((57<<16)+(24<<8)+102)) //ffmpeg-3.0
                av_packet_unref(&m_pkt);
#else
                av_free_packet(&m_pkt);
#endif
            m_pkt.data = 0;
            m_temp_pkt.size = 0;
            continue;
        }
    }
}
