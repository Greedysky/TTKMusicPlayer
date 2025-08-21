#ifndef MUSICSONGSMANAGERTHREAD_H
#define MUSICSONGSMANAGERTHREAD_H

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

#include "ttkabstractthread.h"

/*! @brief The class of the songs manager thread.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongsManagerThread : public TTKAbstractThread
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongsManagerThread(QObject *parent = nullptr);

    /*!
     * Set find file path by given path.
     */
    void setFindFilePath(const QString &path) noexcept;
    /*!
     * Set find file path by given path list.
     */
    void setFindFilePath(const QStringList &path) noexcept;

Q_SIGNALS:
    /*!
     * Send the searched file or path.
     */
    void searchFilePathChanged(const QStringList &name);

private:
    /*!
     * Thread run now.
     */
    virtual void run() override final;

    QStringList m_path;

};

#endif // MUSICSONGSMANAGERTHREAD_H
