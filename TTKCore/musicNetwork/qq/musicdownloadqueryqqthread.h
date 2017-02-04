#ifndef MUSICDOWNLOADQUERYQQTHREAD_H
#define MUSICDOWNLOADQUERYQQTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadqqinterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to qq query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryQQThread : public MusicDownLoadQueryThreadAbstract,
                                                        private MusicDownLoadQQInterface
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryQQThread(QObject *parent = 0);
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
    QString getMovieKey(int id, const QString &videoId);
    /*!
     * Get movie key.
     */

};

#endif // MUSICDOWNLOADQUERYQQTHREAD_H
