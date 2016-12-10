#ifndef MUSICDOWNLOADQUERYALBUMTTTHREAD_H
#define MUSICDOWNLOADQUERYALBUMTTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

const QString TT_SONG_SINGER_URL   = "aFozbkdtREt1Z3VLaU9MS2hEVDlxMUpCU1cxOExpZUpOelo4QmpMakdwWWxaTGIyWVlpMGpubU11QmgxNE5IaEUrbVJjazZ2TzVJVHRYeGNXaGx0cXc9PQ==";
const QString TT_SONG_ALBUM_URL    = "bEc2U0x6dUkxSUE3MkJMdzBUN1l2aFowMDFGUFV1Y2F3MVA3SDdDdzlIY2JxMCtsZ3BXZkpGSWhSY1JsanVGM2F4NldrQlQrRytkMXphT2JmNnIxWEE9PQ==";

/*! @brief The class to ttpod query album download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryAlbumTTThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryAlbumTTThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startSearchSong(QueryType type, const QString &text) override;
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

#endif // MUSICDOWNLOADQUERYALBUMTTTHREAD_H
