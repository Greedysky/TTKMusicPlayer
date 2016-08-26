#ifndef MUSICDOWNLOADQUERYALBUMTHREAD_H
#define MUSICDOWNLOADQUERYALBUMTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

//mulity query
const QString MUSIC_ALBUM_WY = "http://itwusun.com/search/wy/album_%1.html?f=json&sign=itwusun";     //wangyiMusic
const QString MUSIC_ALBUM_DX = "http://itwusun.com/search/dx/album_%1.html?f=json&sign=itwusun";     //dianxinMusic
const QString MUSIC_ALBUM_QQ = "http://itwusun.com/search/aa/album_%1.html?f=json&sign=itwusun";     //qqMusic
const QString MUSIC_ALBUM_XM = "http://itwusun.com/search/xm/album_%1.html?f=json&sign=itwusun";     //xiamiMusic
const QString MUSIC_ALBUM_TT = "http://itwusun.com/search/tt/album_%1.html?f=json&sign=itwusun";     //ttpodMusic
const QString MUSIC_ALBUM_BD = "http://itwusun.com/search/bd/album_%1.html?f=json&sign=itwusun";     //baiduMusic
const QString MUSIC_ALBUM_KW = "http://itwusun.com/search/kw/album_%1.html?f=json&sign=itwusun";     //kuwoMusic
const QString MUSIC_ALBUM_KG = "http://itwusun.com/search/kg/album_%1.html?f=json&sign=itwusun";     //kuhouMusic
const QString MUSIC_ALBUM_DM = "http://itwusun.com/search/dm/album_%1.html?f=json&sign=itwusun";     //duomiMusic
const QString MUSIC_ALBUM_MG = "http://itwusun.com/search/mg/album_%1.html?f=json&sign=itwusun";     //miguMusic
const QString MUSIC_ALBUM_MU = "http://itwusun.com/search/miui/album_%1.html?f=json&sign=itwusun";   //dingdongMusic
const QString MUSIC_ALBUM_EC = "http://itwusun.com/search/echo/album_%1.html?f=json&sign=itwusun";   //echoMusic
const QString MUSIC_ALBUM_YY = "http://itwusun.com/search/yyt/album_%1.html?f=json&sign=itwusun";    //yinyuetaiMusic


/*! @brief The class to multiple query album download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MusicDownLoadQueryAlbumThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryAlbumThread(QObject *parent = 0);
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

protected:
    QString getCurrentURL() const;
    /*!
     * Get current url the user selected from mutiple queue containers.
     */

};

#endif // MUSICDOWNLOADQUERYALBUMTHREAD_H
