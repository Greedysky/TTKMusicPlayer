/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#ifndef BITCRUSHERPLUGIN_H
#define BITCRUSHERPLUGIN_H

#include <QMutex>
#include <qmmp/effect.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class BitcrusherPlugin : public Effect
{
public:
    BitcrusherPlugin();
    virtual ~BitcrusherPlugin();

    virtual void applyEffect(Buffer *b) override final;
    virtual void configure(quint32 freq, ChannelMap map) override final;

    void setDepth(double depth);
    void setDownsample(double downsample);

    static BitcrusherPlugin* instance();

private:
    QMutex m_mutex;
    double m_depth = 0.0f;
    double m_downsample = 0.0f;
    double m_accumulator = 0.0f;
    float *m_buffer = nullptr;
    static BitcrusherPlugin *m_instance;

};

#endif
