#ifndef TTKSEMAPHORELOOP_H
#define TTKSEMAPHORELOOP_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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
#include <QEventLoop>
#include "ttkglobaldefine.h"

/*! @brief The class of the semaphore event loop.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKSemaphoreLoop : public QEventLoop
{
    Q_OBJECT
    TTK_DECLARE_MODULE(TTKSemaphoreLoop)
public:
    /*!
     * Object contsructor.
     */
    explicit TTKSemaphoreLoop(QObject *parent = nullptr);
    ~TTKSemaphoreLoop();

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

#endif // TTKSEMAPHORELOOP_H
