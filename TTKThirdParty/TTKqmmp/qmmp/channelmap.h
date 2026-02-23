/***************************************************************************
 *   Copyright (C) 2014-2026 by Ilya Kotov                                 *
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

#ifndef CHANNELMAP_H
#define CHANNELMAP_H

#include <QList>
#include "qmmp.h"

/*! @brief This class represents audio channel mapping.
+ * @author Ilya Kotov <forkotov02@ya.ru>
+ */
class QMMP_EXPORT ChannelMap : public QList<Qmmp::ChannelPosition>
{
public:
    /*!
     * Constructs channel mapping with internal order.
     * \param channels Number channels
     */
    explicit ChannelMap(int channels = 0);

    /*!
     * Returns a mask with used channels.
     */
    int mask() const;
    /*!
     * Converts current channel order to internal.
     */
    ChannelMap remaped() const;
    /*!
     * Converts channel channel mapping to string representation (for debug purposes).
     */
    QString toString() const;

private:
    static Qmmp::ChannelPosition m_internal_map[9];
    void generateMap(int channels);

};

#endif
