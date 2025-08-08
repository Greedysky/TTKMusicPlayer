#ifndef MUSICBACKUPMODULE_H
#define MUSICBACKUPMODULE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
#include "ttkmoduleexport.h"

/*! @brief The class of the abstract backup module.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractBackup : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicAbstractBackup(int msec, QObject *parent = nullptr);

    /*!
     * Strat backup now.
     */
    void start();
    /*!
     * Stop backup now.
     */
    void stop();

public Q_SLOTS:
    /*!
     * Backup timeout.
     */
    virtual void runBackup() = 0;

private:
    QTimer m_timer;

};


/*! @brief The class of the playlist backup module.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylistBackupModule : public MusicAbstractBackup
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlaylistBackupModule(QObject *parent = nullptr);

public Q_SLOTS:
    /*!
     * Backup timeout.
     */
    virtual void runBackup() override final;

};


/*! @brief The class of the application backup module.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBackupModule
{
    TTK_DECLARE_MODULE(MusicBackupModule)
public:
    /*!
     * Object constructor.
     */
    MusicBackupModule();
    /*!
     * Object destructor.
     */
    ~MusicBackupModule();

    /*!
     * Execute module.
     */
    void execute();

private:
    QList<MusicAbstractBackup*> m_modules;

};

#endif // MUSICBACKUPMODULE_H
