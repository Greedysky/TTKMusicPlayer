#ifndef MUSICDOWNLOADTHREADABSTRACT_H
#define MUSICDOWNLOADTHREADABSTRACT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QNetworkReply>
#include <QFile>
#include <QTimer>
#include "musicobject.h"
#include "musicglobaldefine.h"

class QNetworkAccessManager;

//single query
const QString MUSIC_REQUERY_URL = "http://search.dongting.com/song/search/old?q=%1&page=1&size=500";
const QString MV_REQUERY_URL = "http://pcweb.ttpod.com/mv/search?page=1&size=500&q=%1";
const QString MUSIC_LRC_URL = "http://lp.music.ttpod.com/lrc/down?lrcid=&artist=%1&title=%2&song_id=%3";
const QString SML_BG_ART_URL = "http://lp.music.ttpod.com/pic/down?artist=%1";
const QString BIG_BG_ART_URL = "http://www.kuwo.cn/mingxing/%1/pic.htm";

//mulity query
const QString MUSIC_REQUERY_WY = "http://api.musicuu.com/music/search/wy/%1/1?format=json"; //wangyiMusic
const QString MUSIC_REQUERY_DX = "http://api.musicuu.com/music/search/dx/%1/1?format=json"; //dianxinMusic
const QString MUSIC_REQUERY_QQ = "http://api.musicuu.com/music/search/qq/%1/1?format=json"; //qqMusic
const QString MUSIC_REQUERY_XM = "http://api.musicuu.com/music/search/xm/%1/1?format=json"; //xiamiMusic
const QString MUSIC_REQUERY_TT = "http://api.musicuu.com/music/search/tt/%1/1?format=json"; //ttpodMusic
const QString MUSIC_REQUERY_BD = "http://api.musicuu.com/music/search/bd/%1/1?format=json"; //baiduMusic
const QString MUSIC_REQUERY_KW = "http://api.musicuu.com/music/search/kw/%1/1?format=json"; //kuwoMusic
const QString MUSIC_REQUERY_KG = "http://api.musicuu.com/music/search/kg/%1/1?format=json"; //kuhouMusic
const QString MUSIC_REQUERY_DM = "http://api.musicuu.com/music/search/dm/%1/1?format=json"; //duomiMusic

/*! @brief The class of abstract downloading data.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadThreadAbstract : public QObject
{
    Q_OBJECT
public:
    enum Download_Type
    {
        Download_Music, ///*type of dwonlaod music*/
        Download_Lrc,   ///*type of dwonlaod lrc*/
        Download_SmlBG, ///*type of dwonlaod small background*/
        Download_BigBG, ///*type of dwonlaod big background*/
        Download_Video, ///*type of dwonlaod video*/
        Download_Other  ///*type of dwonlaod other user mod*/
    };

    MusicDownLoadThreadAbstract(const QString &url, const QString &save,
                                Download_Type type, QObject *parent = 0);
    /*!
     * Object contsructor provide download URL\ save local path and download type.
     */
    virtual ~MusicDownLoadThreadAbstract();

    void deleteAll();
    /*!
     * Release the network object.
     */
    virtual void startToDownload() = 0;
    /*!
     * Start to download data.
     * Subclass should implement this function.
     */

Q_SIGNALS:
    void musicDownLoadFinished(const QString &name);
    /*!
     * Data download is finished, get the type of download type.
     */

public Q_SLOTS:
    virtual void downLoadFinished() = 0;
    /*!
     * Download data from net finished.
     * Subclass should implement this function.
     */
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    /*!
     * Get download received and total data.
     */
    void replyError(QNetworkReply::NetworkError error);
    /*!
     * Download reply error.
     */
    void updateDownloadSpeed();
    /*!
     * Updata download speed due the user mod the net speed limited.
     */

protected:
    QNetworkAccessManager *m_manager;
    QNetworkReply* m_reply;
    QFile *m_file;
    QString m_url, m_savePathName;
    Download_Type m_downloadType;
    qint64 m_hasRecevied, m_currentRecevied;
    QTimer m_timer;

};

#endif // MUSICDOWNLOADTHREADABSTRACT_H
