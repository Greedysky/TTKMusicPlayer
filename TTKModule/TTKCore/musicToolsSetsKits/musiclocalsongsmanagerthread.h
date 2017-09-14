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
class MUSIC_SUPER_EXPORT MusicLocalSongsManagerThread : public QThread
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongsManagerThread(QObject *parent = 0);

    ~MusicLocalSongsManagerThread();

    /*!
     * Get class object name.
     */
    static QString getClassName();

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
