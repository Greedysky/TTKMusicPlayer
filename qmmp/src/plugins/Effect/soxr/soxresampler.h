/***************************************************************************
 *   Copyright (C) 2016-2026 by Ilya Kotov                                 *
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

#ifndef SOXRESAMPLER_H
#define SOXRESAMPLER_H

#include <soxr.h>
#include <qmmp/effect.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class SoXResampler : public Effect
{
public:
    SoXResampler();
    virtual ~SoXResampler();

    virtual void applyEffect(Buffer *b) override final;
    virtual void configure(quint32 freq, ChannelMap map) override final;

private:
    void deinit();

    quint32 m_sampleRate;
    float *m_out = nullptr;
    size_t m_samples = 0;
    soxr_quality_spec_t m_quality;
    soxr_t m_soxr = nullptr;

};

#endif
