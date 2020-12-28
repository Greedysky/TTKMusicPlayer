#ifndef MUSICCONNECTTRANSFERTHREAD_H
#define MUSICCONNECTTRANSFERTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#include "musicabstractthread.h"

/*! @brief The class of the connect transfer thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicConnectTransferThread : public MusicAbstractThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicConnectTransferThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicConnectTransferThread(QObject *parent = nullptr);

    /*!
     * Set copy file path lists.
     */
    void setCopyFilePath(const QString &target, const QStringList &path);

Q_SIGNALS:
    /*!
     * Send the transfer file or path.
     */
    void transferFileFinished(const QString &name);

protected:
    /*!
     * Thread run now.
     */
    virtual void run() override;

protected:
    QString m_target;
    QStringList m_path;

};

#endif // MUSICCONNECTTRANSFERTHREAD_H
