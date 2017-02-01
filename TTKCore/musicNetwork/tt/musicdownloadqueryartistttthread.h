#ifndef MUSICDOWNLOADQUERYARTISTTTTHREAD_H
#define MUSICDOWNLOADQUERYARTISTTTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

const QString TT_SONG_SINGER_URL   = "aFozbkdtREt1Z3VLaU9MS2hEVDlxMUpCU1cxOExpZUpOelo4QmpMakdwWWxaTGIyWVlpMGpubU11QmgxNE5IaEUrbVJjazZ2TzVJVHRYeGNXaGx0cXc9PQ==";

/*! @brief The class to ttpod query artist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryArtistTTThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryArtistTTThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startSearchSong(QueryType type, const QString &artist) override;
    /*!
     * Start to Search data from name and type.
     */
    void startSearchSong(const QString &artist);
    /*!
     * Start to Search data from name and type.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

};

#endif // MUSICDOWNLOADQUERYARTISTTTTHREAD_H
