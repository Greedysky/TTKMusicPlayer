/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#ifndef CRYSTALIZERPLUGIN_H
#define CRYSTALIZERPLUGIN_H

#include <QMutex>
#include <qmmp/effect.h>

#define DEFAULT_INTENSITY 10

/*!
 * @author Greedysky <greedysky@163.com>
 */
class CrystalizerPlugin : public Effect
{
public:
    CrystalizerPlugin();
    virtual ~CrystalizerPlugin();

    virtual void applyEffect(Buffer *b) override final;
    virtual void configure(quint32 freq, ChannelMap map) override final;

    void setIntensity(int intensity);

    static CrystalizerPlugin* instance();

private:
    QMutex m_mutex;
    int m_intensity = 0;
    float *m_buffer = nullptr;
    static CrystalizerPlugin *m_instance;

};

#endif
