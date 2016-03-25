#ifndef MUSICNETWORKTESTTHREAD_H
#define MUSICNETWORKTESTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QThread>
#include <QStringList>
#include "musicglobaldefine.h"

class QProcess;

/*! @brief The class of the network test thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicNetworkTestThread : public QThread
{
    Q_OBJECT
public:
    explicit MusicNetworkTestThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicNetworkTestThread();

    void stopAndQuitThread();
    /*!
     * Stop and quit current thread.
     */
    void setAvailableNewtworkNames(const QStringList &names);
    /*!
     * Set available newtwork names.
     */
    QStringList getAvailableNewtworkNames() const;
    /*!
     * Get available newtwork names.
     */
    QStringList getNewtworkNames() const;
    /*!
     * Get newtwork names.
     */

Q_SIGNALS:
    void networkData(ulong upload, ulong download);
    /*!
     * Send current upload and download speed data.
     */

public Q_SLOTS:
    void start();
    /*!
     * Strat thread now.
     */
    void run();
    /*!
     * Thread run now.
     */

private Q_SLOTS:
    void outputRecieved();
    /*!
     * Get output recieved data on linux network.
     */

protected:
    bool m_run;
    QStringList m_names;
    QProcess *m_process ;

};

#endif // MUSICNETWORKTESTTHREAD_H
