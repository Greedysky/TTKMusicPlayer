#ifndef MUSICLOCALSONGSMANAGERTHREAD_H
#define MUSICLOCALSONGSMANAGERTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

#include <QThread>
#include <QStringList>
#include <QFileInfoList>
#include "musicglobaldefine.h"

/*! @brief The class of the local songs manager thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicLocalSongsManagerThread : public QThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalSongsManagerThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongsManagerThread(QObject *parent = nullptr);

    ~MusicLocalSongsManagerThread();

    /*!
     * Set find file path by given path.
     */
    void setFindFilePath(const QString &path);
    /*!
     * Set find file path by given path lists.
     */
    void setFindFilePath(const QStringList &path);
    /*!
     * Stop and quit current thread.
     */
    void stopAndQuitThread();

Q_SIGNALS:
    /*!
     * Send the searched file or path.
     */
    void setSongNamePath(const QFileInfoList &name);

public Q_SLOTS:
    /*!
     * Strat thread now.
     */
    void start();
    /*!
     * Thread run now.
     */
    virtual void run() override;

protected:
    bool m_run;
    QStringList m_path;

};

#endif // MUSICLOCALSONGSMANAGERTHREAD_H
