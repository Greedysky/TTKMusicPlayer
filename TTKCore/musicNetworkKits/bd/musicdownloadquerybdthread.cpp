#include "musicdownloadquerybdthread.h"
#include "musicdownloadqueryyytthread.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryBDThread::MusicDownLoadQueryBDThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "Baidu";
}

QString MusicDownLoadQueryBDThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryBDThread::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_SONG_SEARCH_URL, false).arg(text).arg(0).arg(50);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryBDThread::downLoadFinished()
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
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["error_code"].toInt() == 22000 && value.contains("result"))
            {
                value = value["result"].toMap();
                value = value["song_info"].toMap();
                QVariantList datas = value["song_list"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_singerName = value["author"].toString();
                    musicInfo.m_songName = value["title"].toString();
                    musicInfo.m_timeLength = "-";

                    if(m_currentType != MovieQuery)
                    {
                        musicInfo.m_songId = value["song_id"].toString();
                        musicInfo.m_artistId = value["ting_uid"].toString();
                        musicInfo.m_albumId = value["album_id"].toString();
                        if(!m_querySimplify)
                        {
                            musicInfo.m_lrcUrl = value["lrclink"].toString();
                            musicInfo.m_smallPicUrl = value["pic_small"].toString().replace(",w_90", ",w_500");

                            if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                            readFromMusicSongAttribute(&musicInfo, value["all_rate"].toString(), m_searchQuality, m_queryAllRecords);
                            if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                            if(musicInfo.m_songAttrs.isEmpty())
                            {
                                continue;
                            }

                            MusicSearchedItem item;
                            item.m_songname = musicInfo.m_songName;
                            item.m_artistname = musicInfo.m_singerName;
                            item.m_time = musicInfo.m_songAttrs.first().m_duration;
                            item.m_type = mapQueryServerString();
                            emit createSearchedItems(item);
                        }
                        m_musicSongInfos << musicInfo;
                    }
                    else
                    {
                        //MV
                        if(value["has_mv"].toInt() == 1)
                        {
                            musicInfo.m_songId = value["song_id"].toString();
                            if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                            readFromMusicMVAttribute(&musicInfo, musicInfo.m_songId);
                            if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        }

                        if(musicInfo.m_songAttrs.isEmpty())
                        {
                            continue;
                        }

                        MusicSearchedItem item;
                        item.m_songname = musicInfo.m_songName;
                        item.m_artistname = musicInfo.m_singerName;
                        item.m_time = musicInfo.m_songAttrs.first().m_duration;
                        item.m_type = mapQueryServerString();
                        emit createSearchedItems(item);
                        m_musicSongInfos << musicInfo;
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

void MusicDownLoadQueryBDThread::readFromMusicMVAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QString &id)
{
    if(id.isEmpty() || !m_manager)
    {
        return;
    }

    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_MV_INFO_URL, false).arg(id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
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
        if(value["error_code"].toInt() == 22000 && value.contains("result"))
        {
            value = value["result"].toMap();
            value = value["files"].toMap();
            value = value[value.keys().first()].toMap();
            QString path = value["file_link"].toString();
            if(path.contains("video-url"))
            {
                path = path.split("/").back();
            }
            else
            {
                path = path.split("/").back().split("_").front();
            }

            if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
            readFromMusicMVInfo(info, path);
            if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
        }
    }
}

void MusicDownLoadQueryBDThread::readFromMusicMVInfo(MusicObject::MusicSongInfomation *info,
                                                     const QString &id)
{
    if(id.isEmpty() || !m_manager)
    {
        return;
    }

    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_MV_INFO_ATTR_URL, false).arg(id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
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
        if(value.contains("error") && value["error"].toString() == "false")
        {
            QString duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt()*1000);
            if(value.contains("hdVideoUrl"))
            {
                readFromMusicMVInfoAttribute(info, value["hdVideoUrl"].toString(), duration);
            }
            if(value.contains("hcVideoUrl"))
            {
                readFromMusicMVInfoAttribute(info, value["hcVideoUrl"].toString(), duration);
            }
            if(value.contains("heVideoUrl"))
            {
                readFromMusicMVInfoAttribute(info, value["heVideoUrl"].toString(), duration);
            }
        }
    }
}

void MusicDownLoadQueryBDThread::readFromMusicMVInfoAttribute(MusicObject::MusicSongInfomation *info,
                                                              const QString &url, const QString &duration)
{
    if(url.isEmpty())
    {
        return;
    }

    QStringList datas = url.split("?");
    if(datas.count() == 2)
    {
        QString v = datas.front();
        MusicObject::MusicSongAttribute attr;
        attr.m_url = url;
        attr.m_size = "-";
        attr.m_format = v.right(v.length() - v.lastIndexOf(".") - 1);
        attr.m_duration = duration;
        v = datas.back();
        foreach(QString var, v.split("&"))
        {
            if(var.contains("br="))
            {
                int bitRate = var.remove("br=").toInt();
                if(bitRate > 375 && bitRate <= 625)
                    attr.m_bitrate = MB_500;
                else if(bitRate > 625 && bitRate <= 875)
                    attr.m_bitrate = MB_750;
                else if(bitRate > 875)
                    attr.m_bitrate = MB_1000;
                else
                    attr.m_bitrate = bitRate;
            }
        }
        info->m_songAttrs.append(attr);
    }
}
