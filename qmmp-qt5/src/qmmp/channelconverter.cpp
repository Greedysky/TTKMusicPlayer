/***************************************************************************
 *   Copyright (C) 2014 by Ilya Kotov                                      *
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

#include "channelconverter_p.h"

ChannelConverter::ChannelConverter(ChannelMap out_map)
{
    m_disabled = true;
    m_tmp_buf = 0;
    m_format = Qmmp::PCM_UNKNOWM;
    m_frame_size = 0;
    m_channels = 0;
    m_out_map = out_map;
    memset(m_reorder_array, 0, sizeof(m_reorder_array));
}

ChannelConverter::~ChannelConverter()
{
    if(m_tmp_buf)
    {
        delete [] m_tmp_buf;
        m_tmp_buf = 0;
    }
}

void ChannelConverter::configure(quint32 srate, ChannelMap in_map, Qmmp::AudioFormat f)
{
    Effect::configure(srate, m_out_map, f);

    if((m_disabled = (in_map == m_out_map)))
        return;

    m_channels = channels();
    m_frame_size = audioParameters().sampleSize() * channels();
    m_tmp_buf = new unsigned char[m_frame_size];
    m_format = f;

    QStringList reorderStringList;
    for(int i = 0; i < m_channels; ++i)
    {
        m_reorder_array[i] = m_out_map.indexOf(in_map.at(i));
        reorderStringList << QString("%1").arg(m_reorder_array[i]);
    }

    qDebug("ChannelConverter: {%s} ==> {%s}; {%s}", qPrintable(in_map.toString()),
           qPrintable(m_out_map.toString()), qPrintable(reorderStringList.join(",")));
}

void ChannelConverter::applyEffect(Buffer *b)
{
    if(m_disabled)
        return;

    unsigned long i = 0;
    int j = 0;

    switch(m_format)
    {
    case Qmmp::PCM_S8:
    {
        unsigned char *data = b->data;
        unsigned char *prev_data = m_tmp_buf;
        for(i = 0; i < b->nbytes/m_frame_size; ++i)
        {
            memcpy(m_tmp_buf, data, m_frame_size);
            for(j = 0; j < m_channels; ++j)
                data[j] = m_reorder_array[j] < 0 ? 0 : prev_data[m_reorder_array[j]];
            data += m_channels;
        }
        break;
    }
    case Qmmp::PCM_S16LE:
    {
        quint16 *data = (quint16 *) b->data;
        quint16 *prev_data = (quint16 *) m_tmp_buf;
        for(i = 0; i < b->nbytes/m_frame_size; ++i)
        {
            memcpy(m_tmp_buf, data, m_frame_size);
            for(j = 0; j < m_channels; ++j)
                data[j] = m_reorder_array[j] < 0 ? 0 : prev_data[m_reorder_array[j]];
            data += m_channels;
        }
        break;
    }
    case Qmmp::PCM_S24LE:
    case Qmmp::PCM_S32LE:
    {
        quint32 *data = (quint32 *) b->data;
        quint32 *prev_data = (quint32 *) m_tmp_buf;
        for(i = 0; i < b->nbytes/m_frame_size; ++i)
        {
            memcpy(m_tmp_buf, data, m_frame_size);
            for(j = 0; j < m_channels; ++j)
                data[j] = m_reorder_array[j] < 0 ? 0 : prev_data[m_reorder_array[j]];
            data += m_channels;
        }
        break;
    }
    default:
        ;
    }
}
