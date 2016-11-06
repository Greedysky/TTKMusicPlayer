/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov <forkotov02@hotmail.ru>         *
 *   Copyright (C) 2009 by Sebastian Pipping <sebastian@pipping.org>       *
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

#include <QSettings>
#include <math.h>
#include <stdlib.h>
#include <qmmp/qmmp.h>
#include "bs2bplugin.h"

Bs2bPlugin *Bs2bPlugin::m_instance = 0;

Bs2bPlugin::Bs2bPlugin() : Effect()
{
    m_instance = this;
    m_bs2b_handler = bs2b_open();
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    bs2b_set_level(m_bs2b_handler, settings.value("bs2b/level", BS2B_DEFAULT_CLEVEL).toUInt());
    m_chan = 0;
}

Bs2bPlugin::~Bs2bPlugin()
{
    m_instance = 0;
    bs2b_close(m_bs2b_handler);
}

void Bs2bPlugin::applyEffect(Buffer *b)
{
    if(m_chan != 2)
        return;

    m_mutex.lock();
    bs2b_cross_feed_f(m_bs2b_handler, b->data, b->samples/2);
    m_mutex.unlock();
}

void Bs2bPlugin::configure(quint32 freq, ChannelMap map)
{
    m_chan = map.count();
    Effect::configure(freq, map);
    bs2b_set_srate(m_bs2b_handler,freq);
}

void Bs2bPlugin::setCrossfeedLevel(uint32_t level)
{
    m_mutex.lock();
    bs2b_set_level(m_bs2b_handler, level);
    m_mutex.unlock();
}

Bs2bPlugin* Bs2bPlugin::instance()
{
    return m_instance;
}
