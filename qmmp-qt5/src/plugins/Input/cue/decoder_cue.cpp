/***************************************************************************
 *   Copyright (C) 2008-2014 by Ilya Kotov                                 *
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

#include <QFile>
#include <QStringList>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include <qmmp/fileinfo.h>
#include <qmmp/decoderfactory.h>
#include <qmmp/soundcore.h>
#include "cueparser.h"
#include "decoder_cue.h"


DecoderCUE::DecoderCUE(const QString &url)
        : Decoder()
{
    m_path = url;
    m_decoder = 0;
    m_parser = 0;
    m_track = 0;
    m_buf = 0;
    m_input = 0;
}

DecoderCUE::~DecoderCUE()
{
    if(m_decoder)
        delete m_decoder;
    m_decoder = 0;
    if(m_parser)
        delete m_parser;
    m_parser = 0;
    if(m_buf)
        delete [] m_buf;
    m_buf = 0;
    if(m_input)
        m_input->deleteLater();
    m_input = 0;
}

bool DecoderCUE::initialize()
{
    m_parser = new CUEParser(m_path);
    if (m_parser->count() == 0)
    {
        qWarning("DecoderCUE: invalid cue file");
        return false;
    }
    m_track = m_path.section("#", -1).toInt();
    m_path = m_parser->filePath(m_track);
    if (!QFile::exists(m_path))
    {
        qWarning("DecoderCUE: file \"%s\" doesn't exist", qPrintable(m_path));
        return false;
    }
    DecoderFactory *df = Decoder::findByPath(m_path);
    if (!df)
    {
        qWarning("DecoderCUE: unsupported file format");
        return false;
    }
    m_length = m_parser->length(m_track);
    m_offset = m_parser->offset(m_track);
    if(!df->properties().noInput)
    {
        m_input = new QFile(m_path);
        if(!m_input->open(QIODevice::ReadOnly))
        {
            qWarning("DecoderCUE: error: %s", qPrintable(m_input->errorString()));
            return false;
        }
    }
    m_decoder = df->create(m_path, m_input);
    if(!m_decoder->initialize())
    {
        qWarning("DecoderCUE: invalid audio file");
        return false;
    }
    m_decoder->seek(m_offset);

    configure(m_decoder->audioParameters().sampleRate(),
              m_decoder->audioParameters().channelMap(),
              m_decoder->audioParameters().format());
    setReplayGainInfo(m_parser->replayGain(m_track), m_decoder->hasHeadroom());
    length_in_bytes = audioParameters().sampleRate() *
                      audioParameters().channels() *
                      audioParameters().sampleSize() * m_length/1000;
    m_totalBytes = 0;

    m_sz = audioParameters().sampleSize() * audioParameters().channels();

    addMetaData(m_parser->info(m_track)->metaData());
    return true;
}

qint64 DecoderCUE::totalTime()
{
    return m_decoder ? m_length : 0;
}

void DecoderCUE::seek(qint64 pos)
{
     m_decoder->seek(m_offset + pos);
     m_totalBytes = audioParameters().sampleRate() *
                    audioParameters().channels() *
                    audioParameters().sampleSize() * pos/1000;
}

qint64 DecoderCUE::read(char *data, qint64 size)
{
    if(length_in_bytes - m_totalBytes < m_sz) //end of cue track
        return 0;

    qint64 len = 0;

    if(m_buf) //read remaining data first
    {
        len = qMin(m_buf_size, size);
        memmove(data, m_buf, len);
        if(size >= m_buf_size)
        {
            delete[] m_buf;
            m_buf = 0;
            m_buf_size = 0;
        }
        else
            memmove(m_buf, m_buf + len, size - len);
    }
    else
        len = m_decoder->read(data, size);

    if(len <= 0) //end of file
        return 0;

    if(len + m_totalBytes <= length_in_bytes)
    {
        m_totalBytes += len;
        return len;
    }

    qint64 len2 = qMax(qint64(0), length_in_bytes - m_totalBytes);
    len2 = (len2 / m_sz) * m_sz; //whole of samples of each channel
    m_totalBytes += len2;
    //save data of the next track
    if(m_buf)
        delete[] m_buf;
    m_buf_size = len - len2;
    m_buf = new char[m_buf_size];
    memmove(m_buf, data + len2, m_buf_size);
    return len2;
}

int DecoderCUE::bitrate()
{
    return m_decoder->bitrate();
}

const QString DecoderCUE::nextURL() const
{
    if(m_track +1 <= m_parser->count() && m_parser->filePath(m_track) == m_parser->filePath(m_track + 1))
        return m_parser->trackURL(m_track + 1);
    else
        return QString();
}

void DecoderCUE::next()
{
    if(m_track +1 <= m_parser->count())
    {
        m_track++;
        m_length = m_parser->length(m_track);
        m_offset = m_parser->offset(m_track);
        length_in_bytes = audioParameters().sampleRate() *
                          audioParameters().channels() *
                          audioParameters().sampleSize() * m_length/1000;
        addMetaData(m_parser->info(m_track)->metaData());
        setReplayGainInfo(m_parser->replayGain(m_track));
        m_totalBytes = 0;
    }
}
