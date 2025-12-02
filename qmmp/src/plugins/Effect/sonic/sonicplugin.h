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

#ifndef SONICLUGIN_H
#define SONICLUGIN_H

#include <QMutex>
#include <qmmp/effect.h>
#include <libsonic/sonic.h>

#define DEFAULT_RATIO 50

/*!
 * @author Greedysky <greedysky@163.com>
 */
class SonicPlugin : public Effect
{
public:
    SonicPlugin();
    virtual ~SonicPlugin();

    virtual void applyEffect(Buffer *b) override final;
    virtual void configure(quint32 srate, ChannelMap map) override final;

    void setRatio(int ratio);

    static SonicPlugin* instance();

private:
    void deinit();

    sonicStream m_stream = nullptr;
    static SonicPlugin *m_instance;

};

#endif
