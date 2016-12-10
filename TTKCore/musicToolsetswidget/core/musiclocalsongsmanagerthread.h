#ifndef MUSICLOCALSONGSMANAGERTHREAD_H
#define MUSICLOCALSONGSMANAGERTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
public:
    explicit MusicLocalSongsManagerThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicLocalSongsManagerThread();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setFindFilePath(const QString &path);
    /*!
     * Set find file path by given path.
     */
    void setFindFilePath(const QStringList &path);
    /*!
     * Set find file path by given path lists.
     */
    void stopAndQuitThread();
    /*!
     * Stop and quit current thread.
     */

Q_SIGNALS:
    void setSongNamePath(const QFileInfoList &name);
    /*!
     * Send the searched file or path.
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
    QStringList m_path;

};

#endif // MUSICLOCALSONGSMANAGERTHREAD_H
