/***************************************************************************
 *   Copyright (C) 2011-2012 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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
#include "qmmp.h"

#define EVENT_STATE_CHANGED (QEvent::Type(QEvent::User)) /*!< @internal */
#define EVENT_NEXT_TRACK_REQUEST (QEvent::Type(QEvent::User + 1)) /*!< @internal */
#define EVENT_FINISHED (QEvent::Type(QEvent::User + 2)) /*!< @internal */
#define EVENT_METADATA_CHANGED (QEvent::Type(QEvent::User + 3)) /*!< @internal */
#define EVENT_STREAM_INFO_CHANGED (QEvent::Type(QEvent::User + 4)) /*!< @internal */

/*! @internal
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class Q_DECL_EXPORT StateChangedEvent : public QEvent
{
public:
    StateChangedEvent(Qmmp::State currentState, Qmmp::State previousState);
    virtual ~StateChangedEvent();

    Qmmp::State currentState() const;
    Qmmp::State previousState() const;

private:
    Qmmp::State m_state;
    Qmmp::State m_prevState;

};

/*! @internal
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class Q_DECL_EXPORT MetaDataChangedEvent : public QEvent
{
public:
    MetaDataChangedEvent(const QMap<Qmmp::MetaData, QString> &metaData);
    virtual ~MetaDataChangedEvent();
    /*!
     * Returns all meta data in map.
     */
    QMap <Qmmp::MetaData, QString> metaData() const;
    /*!
     * Returns the metdata string associated with the given \b key.
     */
    QString metaData(Qmmp::MetaData key) const;

private:
    QMap<Qmmp::MetaData, QString> m_metaData;
};

/*! @internal
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class Q_DECL_EXPORT StreamInfoChangedEvent : public QEvent
{
public:
    StreamInfoChangedEvent(const QHash<QString, QString> &info);
    virtual ~StreamInfoChangedEvent();
    /*!
     * Returns all stream information.
     */
    QHash <QString, QString> streamInfo() const;

private:
    QHash<QString, QString> m_streamInfo;
};

#endif // QMMPEVENTS_P_H
