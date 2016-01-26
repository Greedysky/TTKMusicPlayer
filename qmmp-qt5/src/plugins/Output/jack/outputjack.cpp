/***************************************************************************
 *   Copyright (C) 2007-2008 by Zhuravlev Uriy                             *
 *   stalkerg@gmail.com                                                    *
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
#include <QApplication>
#include <QtGlobal>
#include <QDir>
#include <QSettings>

#include "outputjack.h"
#include <qmmp/audioparameters.h>
#include <qmmp/buffer.h>
#include <qmmp/visual.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define JACK_TIMEOUT 500000L

OutputJACK::OutputJACK() : Output(), m_inited(false)
{
    JACK_Init();
    m_wait_time = 0;
    jack_device = 0;
}

OutputJACK::~OutputJACK()
{
    uninitialize();
}

bool OutputJACK::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    qDebug("OutputJACK: initialize");
    int bits = 0;
    Qmmp::AudioFormat input_format = Qmmp::PCM_S8;
    switch(format)
    {
    case Qmmp::PCM_S8:
        bits = 8;
        input_format = Qmmp::PCM_S8;
        break;
    default:
        bits = 16;
        input_format = Qmmp::PCM_S16LE;
    }


    if(JACK_Open(&jack_device, bits, (unsigned long *)&freq, map.count()))
    {
        m_inited = false;
        return false;
    }
    m_inited = true;
    configure(freq, map, input_format);
    return true;
}

qint64 OutputJACK::latency()
{
    return 0;
}

qint64 OutputJACK::writeAudio(unsigned char *data, qint64 maxSize)
{
    if(!m_inited)
         return -1;
    m = JACK_Write(jack_device, (unsigned char*)data, maxSize);

    if(JACK_GetState(jack_device) != PLAYING && JACK_GetState(jack_device) != RESET)
    {
        qWarning("OutputJACK: jack is not in PLAYING or RESET states");
        return -1;
    }

    if (m == 0)
    {
        usleep(2000);
        m_wait_time += 2000;

        if(m_wait_time > JACK_TIMEOUT)
        {
            qWarning("OutputJACK: jack timeout error");
            return -1;
        }
    }
    else
        m_wait_time = 0;
    return m;
}

void OutputJACK::reset()
{
    JACK_Reset(jack_device);
}

void OutputJACK::uninitialize()
{
    if (!m_inited)
        return;
    JACK_Close(jack_device);
    m_inited = false;
}
