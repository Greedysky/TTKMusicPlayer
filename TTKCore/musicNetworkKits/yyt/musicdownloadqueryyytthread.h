#ifndef MUSICDOWNLOADQUERYYYTTHREAD_H
#define MUSICDOWNLOADQUERYYYTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to yinyuetai query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryYYTThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryYYTThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startSearchSong(QueryType type, const QString &text) override;
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
    void readFromMusicMVInfoAttribute(MusicObject::MusicSongInfomation *info, const QString &url,
                                      const QString &size, const QString &duration);
    /*!
     * Read mv info attribute from query results.
     */

};

#endif // MUSICDOWNLOADQUERYYYTTHREAD_H
