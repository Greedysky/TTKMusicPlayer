#ifndef MUSICDOWNLOADQUERYTHREAD_H
#define MUSICDOWNLOADQUERYTHREAD_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include <QNetworkReply>
#include "musiclibexportglobal.h"
#include "musicobject.h"

class QNetworkAccessManager;

class MUSIC_NETWORK_EXPORT MusicDownLoadQueryThread : public QObject
{
    Q_OBJECT
public:
    enum QueryType
    {
        MusicQuery,
        MovieQuery,
        LrcQuery
    };

    explicit MusicDownLoadQueryThread(QObject *parent = 0);
    ~MusicDownLoadQueryThread();

    void deleteAll();
    void startSearchSong(QueryType type, const QString &text);
    void setSearchQuality(const QString &qual) { m_searchQuality = qual;}
    void setQueryAllRecords(bool state) { m_queryAllRecords = state;}
    inline int getSongIdIndex() const { return m_musicSongInfos.size() + 1;}
    inline const MusicSongInfomations& getMusicSongInfos(){ return m_musicSongInfos;}

signals:
    void resolvedSuccess();
    void clearAllItems();
    void creatSearchedItems(const QString &songname,
                            const QString &artistname, const QString &time);

public slots:
    void searchFinshed();
    void replyError(QNetworkReply::NetworkError error);

protected:
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    MusicSongInfomations m_musicSongInfos;
    QString m_searchText, m_searchQuality;
    QueryType m_currentType;
    bool m_queryAllRecords;

};

#endif // MUSICDOWNLOADQUERYTHREAD_H
