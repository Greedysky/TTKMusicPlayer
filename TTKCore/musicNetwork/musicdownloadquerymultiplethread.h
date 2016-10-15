#ifndef MUSICDOWNLOADQUERYMULTIPLETHREAD_H
#define MUSICDOWNLOADQUERYMULTIPLETHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

//mulity query
const QString MUSIC_REQUERY_WY = "http://api.itwusun.com/music/search/wy/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";     //wangyiMusic
const QString MUSIC_REQUERY_DX = "http://api.itwusun.com/music/search/dx/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";     //dianxinMusic
const QString MUSIC_REQUERY_QQ = "http://api.itwusun.com/music/search/qq/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";     //qqMusic
const QString MUSIC_REQUERY_XM = "http://api.itwusun.com/music/search/xm/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";     //xiamiMusic
const QString MUSIC_REQUERY_TT = "http://api.itwusun.com/music/search/tt/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";     //ttpodMusic
const QString MUSIC_REQUERY_BD = "http://api.itwusun.com/music/search/bd/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";     //baiduMusic
const QString MUSIC_REQUERY_KW = "http://api.itwusun.com/music/search/kw/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";     //kuwoMusic
const QString MUSIC_REQUERY_KG = "http://api.itwusun.com/music/search/kg/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";     //kuhouMusic
const QString MUSIC_REQUERY_DM = "http://api.itwusun.com/music/search/dm/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";     //duomiMusic
const QString MUSIC_REQUERY_MG = "http://api.itwusun.com/music/search/mg/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";     //miguMusic
const QString MUSIC_REQUERY_MU = "http://api.itwusun.com/music/search/miui/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";   //dingdongMusic
const QString MUSIC_REQUERY_EC = "http://api.itwusun.com/music/search/echo/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";   //echoMusic
const QString MUSIC_REQUERY_YY = "http://api.itwusun.com/music/search/yyt/1?format=json&keyword=%1&sign=a5cc0a8797539d3a1a4f7aeca5b695b9";    //yinyuetaiMusic


/*! @brief The class to multiple query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryMultipleThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryMultipleThread(QObject *parent = 0);
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

protected:
    QString getCurrentURL() const;
    /*!
     * Get current url the user selected from mutiple queue containers.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation &info, const QString &size,
                                    int bit, const QString &url);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */

};

#endif // MUSICDOWNLOADQUERYMULTIPLETHREAD_H
