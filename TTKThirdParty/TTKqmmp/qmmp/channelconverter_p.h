/***************************************************************************
 *   Copyright (C) 2014-2025 by Ilya Kotov                                 *
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

#ifndef CHANNELCONVERTER_P_H
#define CHANNELCONVERTER_P_H

#include "effect.h"

/*! @internal
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT ChannelConverter : public Effect
{
public:
    explicit ChannelConverter(ChannelMap out_map);
    ~ChannelConverter();

    virtual void configure(quint32 srate, ChannelMap in_map) override final;
    virtual void applyEffect(Buffer *b) override final;

private:
    bool m_disabled = true;
    int m_reorder_array[9] = { 0 };
    float *m_tmp_buf = nullptr;
    size_t m_tmp_size  = 0;
    ChannelMap m_out_map, m_in_map;

};

#endif
