/***************************************************************************
 *   Copyright (C) 2011-2020 by Ilya Kotov                                 *
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

#include <QObject>
#include <QFile>
#include <QRegExp>
#include <qmmp/cueparser.h>
#include "decoder_ffmpeg.h"
#include "decoder_ffmpegcue.h"

DecoderFFmpegCue::DecoderFFmpegCue(const QString &url)
    : Decoder()
{
    m_url = url;
}

DecoderFFmpegCue::~DecoderFFmpegCue()
{
    if(m_decoder)
        delete m_decoder;
    m_decoder = nullptr;
    if(m_parser)
        delete m_parser;
    m_parser = nullptr;
    if(m_buf)
        delete [] m_buf;
    m_buf = nullptr;
    if(m_input)
        m_input->deleteLater();
    m_input = nullptr;
}

bool DecoderFFmpegCue::initialize()
{
    QString filePath = m_url;
    if(!m_url.startsWith("ffmpeg://"))
    {
        qWarning("DecoderFFmpegCue: invalid url.");
        return false;
    }
    filePath.remove("ffmpeg://");
    filePath.remove(QRegExp("#\\d+$"));
    m_track = m_url.section("#", -1).toInt();

    AVFormatContext *in = nullptr;
#ifdef Q_OS_WIN
    if(avformat_open_input(&in, filePath.toUtf8().constData(), nullptr, nullptr) < 0)
#else
    if(avformat_open_input(&in, filePath.toLocal8Bit().constData(), nullptr, nullptr) < 0)
#endif
    {
        qDebug("DecoderFFmpegCue: unable to open file");
        return false;
    }

    avformat_find_stream_info(in, nullptr);
    AVDictionaryEntry *cuesheet = av_dict_get(in->metadata, "cuesheet", nullptr, 0);
    if(!cuesheet)
    {
        avformat_close_input(&in);
        qWarning("DecoderFFmpegCue: unable to find cuesheet comment.");
        return false;
    }

    m_parser = new CueParser(cuesheet->value);
    m_parser->setDuration(in->duration * 1000 / AV_TIME_BASE);
    m_parser->setUrl("ffmpeg", filePath);

    if(m_track > m_parser->count() || m_parser->isEmpty())
    {
        qWarning("DecoderFFmpegCue: invalid cuesheet");
        return false;
    }
    m_input = new QFile(filePath);
    if(!m_input->open(QIODevice::ReadOnly))
    {
        qWarning("DecoderFFmpegCue:: %s", qPrintable(m_input->errorString()));
        return false;
    }
    QMap<Qmmp::MetaData, QString> metaData = m_parser->info(m_track)->metaData();
    addMetaData(metaData); //send metadata

    m_duration = m_parser->duration(m_track);
    m_offset = m_parser->offset(m_track);

    m_decoder = new DecoderFFmpeg(filePath, m_input);
    if(!m_decoder->initialize())
    {
        qWarning("DecoderFFapCUE: invalid audio file");
        return false;
    }
    m_decoder->seek(m_offset);

    configure(m_decoder->audioParameters());

    m_trackSize = audioParameters().sampleRate() * audioParameters().channels() *
            audioParameters().sampleSize() * m_duration / 1000;
    m_written = 0;

    m_frameSize = audioParameters().sampleSize() * audioParameters().channels();

    setReplayGainInfo(m_parser->info(m_track)->replayGainInfo()); //send ReplayGaing info
    addMetaData(m_parser->info(m_track)->metaData()); //send metadata
    return true;
}

qint64 DecoderFFmpegCue::totalTime() const
{
    return m_decoder ? m_duration : 0;
}

void DecoderFFmpegCue::seek(qint64 pos)
{
    m_decoder->seek(m_offset + pos);
    m_written = audioParameters().sampleRate() *
            audioParameters().channels() *
            audioParameters().sampleSize() * pos/1000;
}

qint64 DecoderFFmpegCue::read(unsigned char *data, qint64 size)
{
    if(m_trackSize - m_written < m_frameSize) //end of cue track
        return 0;

    qint64 len = 0;

    if(m_buf) //read remaining data first
    {
        len = qMin(m_bufSize, size);
        memmove(data, m_buf, len);
        if(size >= m_bufSize)
        {
            delete[] m_buf;
            m_buf = nullptr;
            m_bufSize = 0;
        }
        else
            memmove(m_buf, m_buf + len, size - len);
    }
    else
        len = m_decoder->read(data, size);

    if(len <= 0) //end of file
        return 0;

    if(len + m_written <= m_trackSize)
    {
        m_written += len;
        return len;
    }

    qint64 len2 = qMax(qint64(0), m_trackSize - m_written);
    len2 = (len2 / m_frameSize) * m_frameSize; //integer number of samples
    m_written += len2;
    //save data of the next track
    if(m_buf)
        delete[] m_buf;
    m_bufSize = len - len2;
    m_buf = new char[m_bufSize];
    memmove(m_buf, data + len2, m_bufSize);
    return len2;
}

int DecoderFFmpegCue::bitrate() const
{
    return m_decoder->bitrate();
}

const QString DecoderFFmpegCue::nextURL() const
{
    if(m_track +1 <= m_parser->count())
        return m_parser->url(m_track + 1);
    else
        return QString();
}

void DecoderFFmpegCue::next()
{
    if(m_track +1 <= m_parser->count())
    {
        m_track++;
        m_duration = m_parser->duration(m_track);
        m_offset = m_parser->offset(m_track);
        m_trackSize = audioParameters().sampleRate() *
                audioParameters().channels() *
                audioParameters().sampleSize() * m_duration/1000;
        addMetaData(m_parser->info(m_track)->metaData());
        setReplayGainInfo(m_parser->info(m_track)->replayGainInfo());
        m_written = 0;
    }
}
