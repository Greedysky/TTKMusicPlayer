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
#include "musiclibexportglobal.h"

class QNetworkAccessManager;

//const QString MUSIC_REQUERY_URL = "http://pcweb.ttpod.com/search/song?q=%1&page=1&size=500";
//const QString MUSIC_REQUERY_URL = "http://so.ard.iyyin.com/v2/songs/search?q=%1&page=1&size=500";
const QString MUSIC_REQUERY_URL = "http://search.dongting.com/song/search/old?q=%1&page=1&size=500";
const QString MV_REQUERY_URL = "http://pcweb.ttpod.com/mv/search?page=1&size=500&q=%1";
const QString MUSIC_LRC_URL = "http://lp.music.ttpod.com/lrc/down?lrcid=&artist=%1&title=%2&song_id=%3";
const QString SML_BG_ART_URL = "http://lp.music.ttpod.com/pic/down?artist=%1";
const QString BIG_BG_ART_URL = "http://www.kuwo.cn/mingxing/%1/pic.htm";

const QString MUSIC_REQUERY_WY = "http://api.musicuu.com/music/search/wy/%1/1?format=json"; //wangyiMusic
const QString MUSIC_REQUERY_DX = "http://api.musicuu.com/music/search/dx/%1/1?format=json"; //dianxinMusic
const QString MUSIC_REQUERY_QQ = "http://api.musicuu.com/music/search/qq/%1/1?format=json"; //qqMusic
const QString MUSIC_REQUERY_XM = "http://api.musicuu.com/music/search/xm/%1/1?format=json"; //xiamiMusic
const QString MUSIC_REQUERY_TT = "http://api.musicuu.com/music/search/tt/%1/1?format=json"; //ttpodMusic
const QString MUSIC_REQUERY_BD = "http://api.musicuu.com/music/search/bd/%1/1?format=json"; //baiduMusic
const QString MUSIC_REQUERY_KW = "http://api.musicuu.com/music/search/kw/%1/1?format=json"; //kuwoMusic
const QString MUSIC_REQUERY_KG = "http://api.musicuu.com/music/search/kg/%1/1?format=json"; //kuhouMusic
const QString MUSIC_REQUERY_DM = "http://api.musicuu.com/music/search/dm/%1/1?format=json"; //duomiMusic


class MUSIC_NETWORK_EXPORT MusicDownLoadThreadAbstract : public QObject
{
    Q_OBJECT
public:
    enum Download_Type
    {
        Download_Music,
        Download_Lrc,
        Download_SmlBG,
        Download_BigBG,
        Download_Video,
        Download_Other
    };

    MusicDownLoadThreadAbstract(const QString &url, const QString &save,
                                Download_Type type, QObject *parent = 0);
    virtual ~MusicDownLoadThreadAbstract();

    void deleteAll();
    virtual void startToDownload() = 0;

Q_SIGNALS:
    void musicDownLoadFinished(const QString &name);

public Q_SLOTS:
    virtual void downLoadFinished() = 0;
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void replyError(QNetworkReply::NetworkError error);
    void updateDownloadSpeed();

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
