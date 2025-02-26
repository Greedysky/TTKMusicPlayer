/***************************************************************************
 *   Copyright (C) 2011-2025 by Ilya Kotov                                 *
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

#ifndef QMMPEVENTS_P_H
#define QMMPEVENTS_P_H

#include <QMap>
#include <QHash>
#include <QEvent>
#include "trackinfo.h"

#define EVENT_STATE_CHANGED (QEvent::Type(QEvent::User)) /*!< @internal */
#define EVENT_NEXT_TRACK_REQUEST (QEvent::Type(QEvent::User + 1)) /*!< @internal */
#define EVENT_FINISHED (QEvent::Type(QEvent::User + 2)) /*!< @internal */
#define EVENT_TRACK_INFO_CHANGED (QEvent::Type(QEvent::User + 3)) /*!< @internal */
#define EVENT_STREAM_INFO_CHANGED (QEvent::Type(QEvent::User + 4)) /*!< @internal */

/*! @internal
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT StateChangedEvent : public QEvent
{
public:
    StateChangedEvent(Qmmp::State currentState, Qmmp::State previousState);
    virtual ~StateChangedEvent() = default;

    Qmmp::State currentState() const;
    Qmmp::State previousState() const;

private:
    Qmmp::State m_state;
    Qmmp::State m_prevState;

};

/*! @internal
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT StreamInfoChangedEvent : public QEvent
{
public:
    explicit StreamInfoChangedEvent(const QHash<QString, QString> &info);
    virtual ~StreamInfoChangedEvent() = default;

    /*!
     * Returns all stream information.
     */
    QHash<QString, QString> streamInfo() const;

private:
    QHash<QString, QString> m_streamInfo;

};

/*! @internal
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT TrackInfoEvent : public QEvent
{
public:
    explicit TrackInfoEvent(const TrackInfo &info);
    virtual ~TrackInfoEvent() = default;

    const TrackInfo &trackInfo() const;

private:
    TrackInfo m_info;

};

#endif
