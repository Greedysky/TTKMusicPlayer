#ifndef MUSICDOWNLOADQUERYWYTHREAD_H
#define MUSICDOWNLOADQUERYWYTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadwyinterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to wangyi query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryWYThread : public MusicDownLoadQueryThreadAbstract,
                                                        private MusicDownLoadWYInterface
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryWYThread(QObject *parent = 0);
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
    void songListFinished();
    /*!
     * Query song list finished.
     */
    void mvListFinished();
    /*!
     * Query mv list finished.
     */

protected:
    void startSongListQuery();
    /*!
     * Start to search song list by id.
     */
    void startMVListQuery();
    /*!
     * Start to search mv list by id.
     */
    qint64 foundAblumIdBySongId(const QString &song);
    /*!
     * Found ablum id by song id.
     */
    void foundLostSongs(const QString &ablum);
    /*!
     * Found lost song by ablum id.
     */
    void foundOtherMovie();
    /*!
     * Found other movie.
     */

    QSet<QString> m_songIds;
    int m_index;

};

#endif // MUSICDOWNLOADQUERYWYTHREAD_H
