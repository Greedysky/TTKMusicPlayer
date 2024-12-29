/***************************************************************************
 *   Copyright (C) 2010-2025 by Ilya Kotov                                 *
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

#ifndef BS2BPLUGIN_H
#define BS2BPLUGIN_H

#include <QMutex>
#include <bs2b/bs2b.h>
#include <qmmp/effect.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class Bs2bPlugin : public Effect
{
public:
    Bs2bPlugin();
    virtual ~Bs2bPlugin();

    virtual void applyEffect(Buffer *b) override final;
    virtual void configure(quint32 freq, ChannelMap map) override final;

    void setCrossfeedLevel(uint32_t level);

    static Bs2bPlugin* instance();

private:
    QMutex m_mutex;
    t_bs2bdp m_handler;
    static Bs2bPlugin *m_instance;

};

#endif
