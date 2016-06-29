#ifndef MUSICUPDATEUNZIPTHREAD_H
#define MUSICUPDATEUNZIPTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QThread>
#include "musicupdateglobaldefine.h"

/*! @brief The class of the update unzip thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_UPDATE_EXPORT MusicUpdateUnzipThread : public QThread
{
    Q_OBJECT
public:
    explicit MusicUpdateUnzipThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicUpdateUnzipThread();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setFilePath(const QString &in, const QString &out);
    /*!
     * Set input and output file path.
     */
    void stopAndQuitThread();
    /*!
     * Stop and quit current thread.
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

protected:
    bool m_run;
    QString m_input, m_output;

};

#endif // MUSICUPDATEUNZIPTHREAD_H
