#ifndef MUSICDOWNLOADQUERYQQALBUMTHREAD_H
#define MUSICDOWNLOADQUERYQQALBUMTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadqqinterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to qq query album download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryQQAlbumThread : public MusicDownLoadQueryThreadAbstract,
                                                             private MusicDownLoadQQInterface
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryQQAlbumThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startSearchSong(QueryType type, const QString &album) override;
    /*!
     * Start to Search data from name and type.
     */
    void startSearchSong(const QString &album);
    /*!
     * Start to Search data from name and type.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

};

#endif // MUSICDOWNLOADQUERYQQALBUMTHREAD_H
