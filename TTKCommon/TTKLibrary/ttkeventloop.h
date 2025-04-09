#ifndef TTKEVENTLOOP_H
#define TTKEVENTLOOP_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2025 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QTimer>
#include <QEventLoop>
#include "ttkmoduleexport.h"

/*! @brief The class of the ttk event loop.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKEventLoop : public QEventLoop
{
    Q_OBJECT
    TTK_DECLARE_MODULE(TTKEventLoop)
public:
    /*!
     * Object constructor.
     */
    explicit TTKEventLoop(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~TTKEventLoop();

    /*!
     * Set timeout msec time.
     */
    void setTimeout(int msec);
    /*!
     * Event loop start.
     */
    int exec(ProcessEventsFlags flags = AllEvents);

public Q_SLOTS:
    /*!
     * Event loop quit.
     */
    void quit();
    /*!
     * Event loop exit.
     */
    void exit();

private:
    QTimer m_timer;

};

#endif // TTKEVENTLOOP_H
