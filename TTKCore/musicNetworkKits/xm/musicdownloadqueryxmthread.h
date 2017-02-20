#ifndef MUSICDOWNLOADQUERYXMTHREAD_H
#define MUSICDOWNLOADQUERYXMTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadxminterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to xiami query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryXMThread : public MusicDownLoadQueryThreadAbstract,
                                                        private MusicDownLoadXMInterface
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryXMThread(QObject *parent = 0);
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

protected:
    void startSongListQuery();
    /*!
     * Start to search song list by id.
     */
    void readFromMusicMVInfoAttribute(MusicObject::MusicSongInfomation *info,
                                      const QString &id, const QString &format);
    /*!
     * Read mv info attribute from query results.
     */
    void foundOtherMovie();
    /*!
     * Found other movie.
     */

    QSet<QString> m_songIds;
    int m_index;

};

#endif // MUSICDOWNLOADQUERYXMTHREAD_H
