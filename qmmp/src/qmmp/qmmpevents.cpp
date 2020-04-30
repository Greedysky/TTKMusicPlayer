/***************************************************************************
 *   Copyright (C) 2011-2020 by Ilya Kotov                                 *
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

#include "qmmpevents_p.h"

StateChangedEvent::StateChangedEvent(Qmmp::State currentState, Qmmp::State previousState)
    : QEvent(EVENT_STATE_CHANGED)
{
    m_state = currentState;
    m_prevState = previousState;
}

StateChangedEvent::~StateChangedEvent(){}

Qmmp::State StateChangedEvent::currentState() const
{
    return m_state;
}

Qmmp::State StateChangedEvent::previousState() const
{
    return m_prevState;
}

StreamInfoChangedEvent::StreamInfoChangedEvent(const QHash<QString, QString> &info)
    : QEvent (EVENT_STREAM_INFO_CHANGED)
{
    m_streamInfo = info;
}

StreamInfoChangedEvent::~StreamInfoChangedEvent(){}

QHash<QString, QString>StreamInfoChangedEvent::streamInfo() const
{
    return m_streamInfo;
}

TrackInfoEvent::TrackInfoEvent(const TrackInfo &info) : QEvent(EVENT_TRACK_INFO_CHANGED)
{
    m_info = info;
}

TrackInfoEvent::~TrackInfoEvent(){}

const TrackInfo &TrackInfoEvent::trackInfo() const
{
    return m_info;
}
