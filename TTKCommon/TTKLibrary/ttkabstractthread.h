#ifndef TTKABSTRACTTHREAD_H
#define TTKABSTRACTTHREAD_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include <QThread>
#include <QStringList>
#include "ttkmoduleexport.h"

/*! @brief The class of the ttk abstract thread.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKAbstractThread : public QThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(TTKAbstractThread)
public:
    /*!
     * Object constructor.
     */
    explicit TTKAbstractThread(QObject *parent = nullptr);

public Q_SLOTS:
    /*!
     * Strat thread now.
     */
    void start();
    /*!
     * Stop and quit current thread.
     */
    void stop();

protected:
    /*!
     * Thread run now.
     */
    virtual void run() override;

protected:
    bool m_running;

};

#endif // TTKABSTRACTTHREAD_H
