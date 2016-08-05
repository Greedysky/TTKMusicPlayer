#ifndef MUSICDOWNLOADQUERYFACTORY_H
#define MUSICDOWNLOADQUERYFACTORY_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsingleton.h"

#define M_DOWNLOAD_QUERY_PTR (MusicSingleton<MusicDownLoadQueryFactory>::createInstance())

class MusicDownLoadQueryThreadAbstract;

/*! @brief The class to produce the downlaod query class by type.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryFactory
{
public:
    MusicDownLoadQueryThreadAbstract *getQueryThread(QObject *parent);
    /*!
     * Get query thread by type.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

protected:
    DECLARE_SINGLETON_CLASS(MusicDownLoadQueryFactory)

};

#endif // MUSICDOWNLOADQUERYFACTORY_H
