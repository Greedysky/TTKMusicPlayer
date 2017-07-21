#include "musicdownloadquerywythread.h"
#include "musicdownloadqueryyytthread.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryWYThread::MusicDownLoadQueryWYThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "WangYi";
}

QString MusicDownLoadQueryWYThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWYThread::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WY_SONG_SEARCH_URL, false);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->post(request, MusicUtils::Algorithm::mdII(WY_SONG_QUERY_URL, false).arg(text).arg(0).toUtf8());
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryWYThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("code") && value["code"].toInt() == 200)
            {
                value = value["result"].toMap();
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    if(m_currentType != MovieQuery)
                    {
                        MusicObject::MusicSongInfomation musicInfo;
                        musicInfo.m_songName = value["name"].toString();
                        musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt());
                        musicInfo.m_songId = QString::number(value["id"].toInt());
                        musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(WY_SONG_LRC_URL, false).arg(value["id"].toInt());

                        QVariantMap albumObject = value["album"].toMap();
                        musicInfo.m_smallPicUrl = albumObject["picUrl"].toString();
                        musicInfo.m_albumId = QString::number(albumObject["id"].toInt());

                        QVariantList artistsArray = value["artists"].toList();
                        foreach(const QVariant &artistValue, artistsArray)
                        {
                            if(artistValue.isNull())
                            {
                                continue;
                            }
                            QVariantMap artistMap = artistValue.toMap();
                            musicInfo.m_artistId = QString::number(artistMap["id"].toULongLong());
                            musicInfo.m_singerName = artistMap["name"].toString();
                        }

                        if(!m_querySimplify)
                        {
                            if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                            readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);
                            if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                            if(musicInfo.m_songAttrs.isEmpty())
                            {
                                continue;
                            }

                            MusicSearchedItem item;
                            item.m_songname = musicInfo.m_songName;
                            item.m_artistname = musicInfo.m_singerName;
                            item.m_time = musicInfo.m_timeLength;
                            item.m_type = mapQueryServerString();
                            emit createSearchedItems(item);
                        }
                        m_musicSongInfos << musicInfo;
                    }
                    else
                    {
                        int mvid = value["mvid"].toLongLong();
                        if(mvid == 0)
                        {
                            continue;
                        }

                        if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        startMVListQuery(mvid);
                        if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    }
                }
            }
        }
    }

    ///extra yyt movie
    if(m_queryExtraMovie && m_currentType == MovieQuery)
    {
        MusicSemaphoreLoop loop;
        MusicDownLoadQueryYYTThread *yyt = new MusicDownLoadQueryYYTThread(this);
        connect(yyt, SIGNAL(createSearchedItems(MusicSearchedItem)), SIGNAL(createSearchedItems(MusicSearchedItem)));
        connect(yyt, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
        yyt->startToSearch(MusicDownLoadQueryYYTThread::MovieQuery, m_searchText);
        loop.exec();
        m_musicSongInfos << yyt->getMusicSongInfos();
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicDownLoadQueryWYThread::startMVListQuery(int id)
{
    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(WY_SONG_MV_URL, false).arg(id)));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("code") && value["code"].toInt() == 200)
        {
            value = value["data"].toMap();
            MusicObject::MusicSongInfomation musicInfo;
            musicInfo.m_songName = value["name"].toString();
            musicInfo.m_singerName = value["artistName"].toString();
            musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt());

            value = value["brs"].toMap();
            foreach(const QString &key, value.keys())
            {
                int bit = key.toInt();
                MusicObject::MusicSongAttribute attr;
                if(bit > 375 && bit <= 625)
                    attr.m_bitrate = MB_500;
                else if(bit > 625 && bit <= 875)
                    attr.m_bitrate = MB_750;
                else if(bit > 875)
                    attr.m_bitrate = MB_1000;
                else
                    attr.m_bitrate = bit;

                attr.m_url = value[key].toString();
                attr.m_format = attr.m_url.right(3);
                attr.m_size = QString();
                musicInfo.m_songAttrs.append(attr);
            }

            if(musicInfo.m_songAttrs.isEmpty())
            {
                return;
            }

            MusicSearchedItem item;
            item.m_songname = musicInfo.m_songName;
            item.m_artistname = musicInfo.m_singerName;
            item.m_time = musicInfo.m_timeLength;
            item.m_type = mapQueryServerString();
            emit createSearchedItems(item);
            m_musicSongInfos << musicInfo;
        }
    }
}
