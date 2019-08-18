#ifndef MUSICCONNECTTRANSFERTHREAD_H
#define MUSICCONNECTTRANSFERTHREAD_H

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
#include "musicglobaldefine.h"

/*! @brief The class of the connect transfer thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicConnectTransferThread : public QThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicConnectTransferThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicConnectTransferThread(QObject *parent = nullptr);

    ~MusicConnectTransferThread();

    /*!
     * Set copy file path lists.
     */
    void setCopyFilePath(const QString &target, const QStringList &path);
    /*!
     * Stop and quit current thread.
     */
    void stopAndQuitThread();

Q_SIGNALS:
    /*!
     * Send the transfer file or path.
     */
    void transferFileFinished(const QString &name);

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
    QString m_target;
    QStringList m_path;

};

#endif // MUSICCONNECTTRANSFERTHREAD_H
