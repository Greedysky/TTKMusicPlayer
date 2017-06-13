#ifndef MUSICDOWNLOADQUERYBDLEARNTHREAD_H
#define MUSICDOWNLOADQUERYBDLEARNTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to baidu query learn download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryBDLearnThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryBDLearnThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startToSearch(QueryType type, const QString &text) override;
    /*!
     * Start to Search data from name and type.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

protected:
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicLrcAttribute(MusicObject::MusicSongInfomation *info);
    /*!
     * Read lrc tag from query results.
     */

};

#endif // MUSICDOWNLOADQUERYBDLEARNTHREAD_H
