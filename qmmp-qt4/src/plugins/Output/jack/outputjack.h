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

#ifndef   OUTPUTJACK_H
#define   OUTPUTJACK_H

class OutputJACK;

#include <qmmp/output.h>

extern "C"
{
#include <jack/jack.h>
}

#include "bio2jack.h"

class OutputJACK : public Output
{
public:
    OutputJACK();
    ~OutputJACK();

    bool initialize(quint32, ChannelMap map, Qmmp::AudioFormat format);
    //output api
    qint64 latency();
    qint64 writeAudio(unsigned char *data, qint64 maxSize);
    void drain(){}
    void reset();

private:
    // helper functions
    void uninitialize();
    qint64 m, m_wait_time;
    bool m_inited;
    int jack_device;
};

#endif
