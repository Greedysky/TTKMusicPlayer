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

class MUSIC_CORE_EXPORT MusicNetworkTestThread : public QThread
{
    Q_OBJECT
public:
    explicit MusicNetworkTestThread(QObject *parent = 0);
    ~MusicNetworkTestThread();

    void stopAndQuitThread();
    void setAvailableNewtworkNames(const QStringList &names);
    QStringList getAvailableNewtworkNames() const;
    QStringList getNewtworkNames() const;

Q_SIGNALS:
    void networkData(ulong upload, ulong download);

public Q_SLOTS:
    void start();
    void run();

private Q_SLOTS:
    void outputRecieved();

protected:
    bool m_run;
    QStringList m_names;
    QProcess *m_process ;

};

#endif // MUSICNETWORKTESTTHREAD_H
