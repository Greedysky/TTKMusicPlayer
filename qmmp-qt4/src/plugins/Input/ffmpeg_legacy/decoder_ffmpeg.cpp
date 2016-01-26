/***************************************************************************
 *   Copyright (C) 2006-2013 by Ilya Kotov                                 *
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
#include "decoder_ffmpeg.h"

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
//legacy ffmpeg support
DecoderFFmpeg::DecoderFFmpeg(const QString &path, QIODevice *i)
        : Decoder(i)
{
    m_bitrate = 0;
    m_skip = false;
    m_totalTime = 0;
    ic = 0;
    m_path = path;
    m_temp_pkt.size = 0;
    m_pkt.size = 0;
    m_pkt.data = 0;
    m_output_buf = 0;
    m_output_at = 0;
    m_skipBytes = 0;
    m_stream = 0;
    av_init_packet(&m_pkt);
    av_init_packet(&m_temp_pkt);
}


DecoderFFmpeg::~DecoderFFmpeg()
{
    m_bitrate = 0;
    m_temp_pkt.size = 0;
    if (ic)
        av_close_input_stream(ic);
   if(m_pkt.data)
        av_free_packet(&m_pkt);
    if(m_output_buf)
        av_free(m_output_buf);
    if(m_stream)
        av_free(m_stream);
}

bool DecoderFFmpeg::initialize()
{
    m_bitrate = 0;
    m_skip = false;
    m_totalTime = 0;
    m_seekTime = -1;
    av_register_all();

    AVProbeData  pd;
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

#if (LIBAVFORMAT_VERSION_INT >= ((52<<16)+(105<<8)+0))
    m_stream = avio_alloc_context(m_input_buf, INPUT_BUFFER_SIZE, 0, this, ffmpeg_read, NULL, ffmpeg_seek);
    if(!m_stream)
    {
        qWarning("DecoderFFmpeg: unable to initialize I/O callbacks");
        return false;
    }
    m_stream->seekable = !input()->isSequential();
#else
    m_stream = (ByteIOContext *)av_malloc(sizeof(ByteIOContext));
    init_put_byte(m_stream, m_input_buf, INPUT_BUFFER_SIZE, 0, this, ffmpeg_read, NULL, ffmpeg_seek);
    m_stream->is_streamed = input()->isSequential();
#endif
    m_stream->max_packet_size = INPUT_BUFFER_SIZE;

    AVFormatParameters ap;
    memset(&ap, 0, sizeof(ap));

    if(av_open_input_stream(&ic, m_stream, m_path.toLocal8Bit(), fmt, &ap) != 0)
    {
        qDebug("DecoderFFmpeg: av_open_input_stream() failed");
        return false;
    }
    av_find_stream_info(ic);
    if(ic->pb)
        ic->pb->eof_reached = 0;

    if (input()->isSequential())
    {
        QMap<Qmmp::MetaData, QString> metaData;
        AVMetadataTag *album = av_metadata_get(ic->metadata,"album",0,0);
        if(!album)
            album = av_metadata_get(ic->metadata,"WM/AlbumTitle",0,0);
        AVMetadataTag *artist = av_metadata_get(ic->metadata,"artist",0,0);
        if(!artist)
            artist = av_metadata_get(ic->metadata,"author",0,0);
        AVMetadataTag *comment = av_metadata_get(ic->metadata,"comment",0,0);
        AVMetadataTag *genre = av_metadata_get(ic->metadata,"genre",0,0);
        AVMetadataTag *title = av_metadata_get(ic->metadata,"title",0,0);
        AVMetadataTag *year = av_metadata_get(ic->metadata,"WM/Year",0,0);
        if(!year)
            year = av_metadata_get(ic->metadata,"year",0,0);
        if(!year)
            year = av_metadata_get(ic->metadata,"date",0,0);
        AVMetadataTag *track = av_metadata_get(ic->metadata,"track",0,0);
        if(!track)
            track = av_metadata_get(ic->metadata,"WM/Track",0,0);
        if(!track)
            track = av_metadata_get(ic->metadata,"WM/TrackNumber",0,0);

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

    ic->flags |= AVFMT_FLAG_GENPTS;
    av_read_play(ic);
    for (wma_idx = 0; wma_idx < (int)ic->nb_streams; wma_idx++)
    {
        c = ic->streams[wma_idx]->codec;
#if LIBAVCODEC_VERSION_MAJOR < 53
        if (c->codec_type == CODEC_TYPE_AUDIO)
#else
        if (c->codec_type == AVMEDIA_TYPE_AUDIO)
#endif
            break;
    }

    if (c->channels > 0)
         c->request_channels = qMin(2, c->channels);
    else
         c->request_channels = 2;

#if (LIBAVCODEC_VERSION_INT >= ((52<<16)+(101<<8)+0))
    av_dump_format(ic,0,0,0);
#else
    dump_format(ic,0,0,0);
#endif
    AVCodec *codec = avcodec_find_decoder(c->codec_id);

    if (!codec)
    {
        qWarning("DecoderFFmpeg: unsupported codec for output stream");
        return false;
    }

    if (avcodec_open(c, codec) < 0)
    {
        qWarning("DecoderFFmpeg: error while opening codec for output stream");
        return false;
    }

    m_totalTime = input()->isSequential() ? 0 : ic->duration * 1000 / AV_TIME_BASE;
    m_output_buf = (uint8_t *)av_malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE*2);

#if (LIBAVCODEC_VERSION_INT >= ((52<<16)+(20<<8)+0))
    if(c->codec_id == CODEC_ID_SHORTEN) //ffmpeg bug workaround
        m_totalTime = 0;
#endif

#if (LIBAVUTIL_VERSION_INT >= ((50<<16)+(38<<8)+0))
    if(c->sample_fmt == AV_SAMPLE_FMT_S32)
        configure(c->sample_rate, c->request_channels, Qmmp::PCM_S32LE);
    else
        configure(c->sample_rate, c->request_channels, Qmmp::PCM_S16LE);
#else
    if(c->sample_fmt == SAMPLE_FMT_S32)
        configure(c->sample_rate, c->request_channels, Qmmp::PCM_S32LE);
    else
        configure(c->sample_rate, c->request_channels, Qmmp::PCM_S16LE);
#endif
    if(ic->bit_rate)
        m_bitrate = ic->bit_rate/1000;
    if(c->bit_rate)
        m_bitrate = c->bit_rate/1000;
    qDebug("DecoderFFmpeg: initialize succes");
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

qint64 DecoderFFmpeg::read(char *audio, qint64 maxSize)
{
    m_skipBytes = 0;
    if (m_skip)
    {
        while(m_temp_pkt.size)
            ffmpeg_decode(m_output_buf);
        m_output_at = 0;
        m_skip = false;
    }
    if(!m_output_at)
        fillBuffer();
    if(!m_output_at)
        return 0;
    qint64 len = qMin(m_output_at, maxSize);
    memcpy(audio, m_output_buf, len);
    m_output_at -= len;
    memmove(m_output_buf, m_output_buf + len, m_output_at);
    return len;
}

qint64 DecoderFFmpeg::ffmpeg_decode(uint8_t *audio)
{
    int out_size = AVCODEC_MAX_AUDIO_FRAME_SIZE * 2;
    if((m_pkt.stream_index == wma_idx))
    {
#if (LIBAVCODEC_VERSION_INT >= ((52<<16)+(23<<8)+0))
        int l = avcodec_decode_audio3(c, (int16_t *)(audio), &out_size, &m_temp_pkt);
#else
        int l = avcodec_decode_audio2(c, (int16_t *)(audio), &out_size, m_temp_pkt.data, m_temp_pkt.size);
#endif
        if(c->bit_rate)
            m_bitrate = c->bit_rate/1000;
        if(l < 0)
            return l;
        m_temp_pkt.data += l;
        m_temp_pkt.size -= l;
    }
    if (!m_temp_pkt.size && m_pkt.data)
        av_free_packet(&m_pkt);

    return out_size;
}

void DecoderFFmpeg::seek(qint64 pos)
{
   int64_t timestamp = int64_t(pos)*AV_TIME_BASE/1000;
    if (ic->start_time != (qint64)AV_NOPTS_VALUE)
        timestamp += ic->start_time;
    m_seekTime = timestamp;
    av_seek_frame(ic, -1, timestamp, AVSEEK_FLAG_BACKWARD);
    if(m_pkt.size)
        m_skip = true;
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
                    av_free_packet(&m_pkt);
                m_temp_pkt.size = 0;
                continue;
            }
            if(m_seekTime && c->codec_id == CODEC_ID_APE)
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

        if(m_skipBytes > 0 && c->codec_id == CODEC_ID_APE)
        {
            while (m_skipBytes > 0)
            {
                m_output_at = ffmpeg_decode(m_output_buf);
                if(m_output_at < 0)
                    break;
                m_skipBytes -= m_output_at;
            }

            if(m_skipBytes < 0)
            {
                qint64 size = m_output_at;
                m_output_at = - m_skipBytes;
                m_output_at = m_output_at/4*4;
                memmove(m_output_buf, (m_output_buf + size - m_output_at), m_output_at);
                m_skipBytes = 0;
            }
        }
        else
            m_output_at = ffmpeg_decode(m_output_buf);

        if(m_output_at < 0)
        {
            m_output_at = 0;
            m_temp_pkt.size = 0;

            if(c->codec_id == CODEC_ID_SHORTEN)
            {
                if(m_pkt.data)
                    av_free_packet(&m_pkt);
                m_pkt.data = 0;
                break;
            }
            continue;
        }
        else if(m_output_at == 0)
        {
            if(c->codec_id == CODEC_ID_SHORTEN)
                continue;

            if(m_pkt.data)
                av_free_packet(&m_pkt);
            m_pkt.data = 0;
            break;
        }
    }
}
