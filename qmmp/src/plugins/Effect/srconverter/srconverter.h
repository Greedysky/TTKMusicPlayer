/***************************************************************************
 *   Copyright (C) 2007-2026 by Ilya Kotov                                 *
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

#ifndef SRCONVERTER_H
#define SRCONVERTER_H

#include <samplerate.h>
#include <qmmp/effect.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class SRConverter : public Effect
{
public:
    SRConverter();
    virtual ~SRConverter();

    virtual void applyEffect(Buffer *b) override final;
    virtual void configure(quint32 freq, ChannelMap map) override final;

private:
    void deinit();

    SRC_STATE *m_state = nullptr;
    SRC_DATA m_data;
    quint32 m_sampleRate;
    int m_engine;

};

#endif
