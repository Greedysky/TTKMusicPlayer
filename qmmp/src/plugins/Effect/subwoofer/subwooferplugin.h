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

#ifndef SUBWOOFERPLUGIN_H
#define SUBWOOFERPLUGIN_H

#include <QMutex>
#include <qmmp/effect.h>

#define STAGE_LENGTH  3

struct BiquadZCoefs
{
    float a0;
    float a1;
    float a2;
    float b0;
    float b1;
    float b2;
};

struct BiquadSCoefs
{
    float a;
    float b;
    float c;
    float d;
    float e;
    float f;
};

struct BiquadBuffer
{
    float x1;
    float x2;
    float y1;
    float y2;
};


/*!
 * @author Greedysky <greedysky@163.com>
 */
class SubwooferPlugin : public Effect
{
public:
    SubwooferPlugin();
    virtual ~SubwooferPlugin();

    virtual void applyEffect(Buffer *b) override final;
    virtual void configure(quint32 freq, ChannelMap map) override final;

    void setLevel(float level);
    void setCutOff(int value);
    void reset();

    static SubwooferPlugin* instance();

private:
    QMutex m_mutex;
    int m_level = 10;
    int m_cutoff = 250;
    BiquadZCoefs m_coefs[STAGE_LENGTH];
    BiquadBuffer m_buffer[2][STAGE_LENGTH];
    static SubwooferPlugin *m_instance;

};

#endif
