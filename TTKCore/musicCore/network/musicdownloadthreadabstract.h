#ifndef MUSICDOWNLOADTHREADABSTRACT_H
#define MUSICDOWNLOADTHREADABSTRACT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QFile>
#include <QTimer>
#include "musicobject.h"
#include "musicnetworkabstract.h"
#include "musicnumberdefine.h"

//single query
const QString MUSIC_REQUERY_URL = "http://search.dongting.com/song/search/old?q=%1&page=1&size=500";
const QString MUSIC_LRC_URL = "http://lp.music.ttpod.com/lrc/down?lrcid=&artist=%1&title=%2&song_id=%3";
const QString SML_BG_ART_URL = "http://lp.music.ttpod.com/pic/down?artist=%1";
const QString BIG_BG_ART_URL = "http://www.kuwo.cn/mingxing/%1/pic.htm";

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


/*! @brief The class of abstract downloading data.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadThreadAbstract : public MusicNetworkAbstract
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

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void deleteAll() override;
    /*!
     * Release the network object.
     */
    virtual void startToDownload() = 0;
    /*!
     * Start to download data.
     * Subclass should implement this function.
     */

public Q_SLOTS:
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    /*!
     * Get download received and total data.
     */
    virtual void replyError(QNetworkReply::NetworkError error) override;
    /*!
     * Download reply error.
     */
#ifndef QT_NO_SSL
    virtual void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) override;
    /*!
     * Download ssl reply error.
     */
#endif
    void updateDownloadSpeed();
    /*!
     * Updata download speed due the user mod the net speed limited.
     */

protected:
    QFile *m_file;
    QString m_url, m_savePathName;
    Download_Type m_downloadType;
    qint64 m_hasReceived, m_currentReceived;
    QTimer m_timer;

};

#endif // MUSICDOWNLOADTHREADABSTRACT_H
