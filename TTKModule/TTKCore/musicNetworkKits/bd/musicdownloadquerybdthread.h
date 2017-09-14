#ifndef MUSICDOWNLOADQUERYBDTHREAD_H
#define MUSICDOWNLOADQUERYBDTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadbdinterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to baidu query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryBDThread : public MusicDownLoadQueryThreadAbstract,
                                                        private MusicDownLoadBDInterface
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadQueryBDThread(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to search data from name and type.
     */
    virtual void startToSearch(QueryType type, const QString &text) override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    /*!
     * Read mv tags(size\bitrate\url) from query results.
     */
    void readFromMusicMVAttribute(MusicObject::MusicSongInformation *info,
                                  const QString &id);
    /*!
     * Read mv infos from query results.
     */
    void readFromMusicMVInfo(MusicObject::MusicSongInformation *info, const QString &id);
    /*!
     * Read mv info attribute from query results.
     */
    void readFromMusicMVInfoAttribute(MusicObject::MusicSongInformation *info,
                                      const QString &url, const QString &duration);

};

#endif // MUSICDOWNLOADQUERYBDTHREAD_H
