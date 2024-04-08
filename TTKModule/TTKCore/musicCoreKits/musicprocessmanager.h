#ifndef MUSICPROCESSMANAGER_H
#define MUSICPROCESSMANAGER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
#include <QSharedMemory>
#include "musicglobaldefine.h"

/*! @brief The class of the process client.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicProcessClient
{
    TTK_DECLARE_MODULE(MusicProcessClient)
public:
    /*!
     * Run.
     */
    void run(const QStringList &args) const;

};


/*! @brief The class of the process server.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicProcessServer : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicProcessServer)
public:
    /*!
     * Object constructor.
     */
    explicit MusicProcessServer(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicProcessServer();

    /*!
     * Run.
     */
    void run(const QStringList &args);

private Q_SLOTS:
    /*!
     * Thread run now.
     */
    void timeout();

private:
    QTimer m_timer;
    QSharedMemory m_memory;

};

#endif // MUSICPROCESSMANAGER_H
