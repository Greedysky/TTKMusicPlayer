/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
 ***************************************************************************/

#ifndef ECHOPLUGIN_H
#define ECHOPLUGIN_H

#include <QMutex>
#include <qmmp/effect.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class EchoPlugin : public Effect
{
public:
    EchoPlugin();
    virtual ~EchoPlugin();

    virtual void applyEffect(Buffer *b) override final;
    virtual void configure(quint32 freq, ChannelMap map) override final;

    void setDelay(int delay);
    void setFeedback(int feedback);
    void setVolume(int volume);

    static EchoPlugin* instance();

private:
    QMutex m_mutex;
    int m_offset = 0;
    int m_delay = 0, m_feedback = 0, m_volume = 0;
    int m_size = 0;
    float *m_buffer = nullptr;
    static EchoPlugin *m_instance;

};

#endif
