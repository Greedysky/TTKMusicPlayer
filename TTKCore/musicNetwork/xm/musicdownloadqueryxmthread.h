#ifndef MUSICDOWNLOADQUERYXMTHREAD_H
#define MUSICDOWNLOADQUERYXMTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

const QString XM_SEARCH_URL     = "N0JRblEwUURDaml2cjBlV3VtSTBhTUFvT1dIZVhoMmdsRm9BZGFHck1jOEpaWmVXNlZ1NjdxR2JoUG93OVZISjdZaVdPZURZMGVyeWJNV0lDVm9VS2l4bmg3TC9iOXFDVGJvdjI5TGZrelZoVVFOTA==";
const QString XM_SONG_PIC_URL   = "T1JqV3kvRksxSW5QUTdWeW9NOGY3L0ppazlDckM1RXYvRElGRlE9PQ==";

const QByteArray XM_BASE_URL    = "bUhKRWJpc0lkTi9sK2lpWU82YUhqdTZnYzZ6cmpJdkg=";

/*! @brief The class to xiami query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryXMThread : public MusicDownLoadQueryThreadAbstract
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

protected:
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                    const QVariantMap &key, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    int map2NormalBitrate(int bitrate);
    /*!
     * Map json bitrate to normal bitrate.
     */

};

#endif // MUSICDOWNLOADQUERYXMTHREAD_H
