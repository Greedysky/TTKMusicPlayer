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
const QString MUSIC_REQUERY_WY = "http://itwusun.com/search/wy/%1?p=1&f=json&sign=itwusun";     //wangyiMusic
const QString MUSIC_REQUERY_DX = "http://itwusun.com/search/dx/%1?p=1&f=json&sign=itwusun";     //dianxinMusic
const QString MUSIC_REQUERY_QQ = "http://itwusun.com/search/qq/%1?p=1&f=json&sign=itwusun";     //qqMusic
const QString MUSIC_REQUERY_XM = "http://itwusun.com/search/xm/%1?p=1&f=json&sign=itwusun";     //xiamiMusic
const QString MUSIC_REQUERY_TT = "http://itwusun.com/search/tt/%1?p=1&f=json&sign=itwusun";     //ttpodMusic
const QString MUSIC_REQUERY_BD = "http://itwusun.com/search/bd/%1?p=1&f=json&sign=itwusun";     //baiduMusic
const QString MUSIC_REQUERY_KW = "http://itwusun.com/search/kw/%1?p=1&f=json&sign=itwusun";     //kuwoMusic
const QString MUSIC_REQUERY_KG = "http://itwusun.com/search/kg/%1?p=1&f=json&sign=itwusun";     //kuhouMusic
const QString MUSIC_REQUERY_DM = "http://itwusun.com/search/dm/%1?p=1&f=json&sign=itwusun";     //duomiMusic
const QString MUSIC_REQUERY_MG = "http://itwusun.com/search/mg/%1?p=1&f=json&sign=itwusun";     //miguMusic
const QString MUSIC_REQUERY_MU = "http://itwusun.com/search/miui/%1?p=1&f=json&sign=itwusun";   //dingdongMusic
const QString MUSIC_REQUERY_EC = "http://itwusun.com/search/echo/%1?p=1&f=json&sign=itwusun";   //echoMusic
const QString MUSIC_REQUERY_YY = "http://itwusun.com/search/yyt/%1?p=1&f=json&sign=itwusun";    //yinyuetaiMusic


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
