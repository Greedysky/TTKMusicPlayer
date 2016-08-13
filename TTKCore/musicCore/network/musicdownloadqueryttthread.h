#ifndef MUSICDOWNLOADQUERYTTTHREAD_H
#define MUSICDOWNLOADQUERYTTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

//single query
const QString MUSIC_REQUERY_URL = "http://search.dongting.com/song/search/old?q=%1&page=1&size=500";
const QString MUSIC_LRC_URL = "http://lp.music.ttpod.com/lrc/down?lrcid=&artist=%1&title=%2&song_id=%3";
const QString SML_BG_ART_URL = "http://lp.music.ttpod.com/pic/down?artist=%1";
const QString BIG_BG_ART_URL = "http://www.kuwo.cn/mingxing/%1/pic.htm";


/*! @brief The class to ttpod query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryTTThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryTTThread(QObject *parent = 0);
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

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

};

#endif // MUSICDOWNLOADQUERYTTTHREAD_H
