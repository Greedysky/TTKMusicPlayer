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
    explicit MusicDownLoadQueryBDThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startToSearch(QueryType type, const QString &text) override;
    /*!
     * Start to search data from name and type.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

protected:
    void readFromMusicMVAttribute(MusicObject::MusicSongInfomation *info,
                                  const QString &id);
    /*!
     * Read mv tags(size\bitrate\url) from query results.
     */
    void readFromMusicMVInfo(MusicObject::MusicSongInfomation *info, const QString &id);
    /*!
     * Read mv infos from query results.
     */
    void readFromMusicMVInfoAttribute(MusicObject::MusicSongInfomation *info,
                                      const QString &url, const QString &duration);
    /*!
     * Read mv info attribute from query results.
     */

};

#endif // MUSICDOWNLOADQUERYBDTHREAD_H
