#ifndef MUSICTIMERAUTOMODULE_H
#define MUSICTIMERAUTOMODULE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
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

#include <QTimer>
#include "musicglobaldefine.h"

/*! @brief The class of the time auto run.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTimerAutoModule : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicTimerAutoModule(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicTimerAutoModule();

    /*!
     * Execute module.
     */
    void execute();

private Q_SLOTS:
    /*!
     * Check current state by one minute.
     */
    void timeout();

private:
    /*!
     * Set current system to shutdown.
     */
    void setShutdown();

private:
    struct MusicTimeData
    {
        int m_index;
        int m_hour;
        int m_minute;
        bool m_state;

        MusicTimeData() noexcept
            : m_index(-1),
              m_hour(-1),
              m_minute(-1),
              m_state(false)
        {

        }
    };
    TTK_DECLARE_LIST(MusicTimeData);

private:
    QTimer m_timer;
    MusicTimeDataList m_timeDatas;

};

#endif // MUSICTIMERAUTOMODULE_H
