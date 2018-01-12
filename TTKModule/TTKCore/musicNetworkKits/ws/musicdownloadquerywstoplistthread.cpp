#include "musicdownloadquerywstoplistthread.h"
#include "musicsemaphoreloop.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryWSToplistThread::MusicDownLoadQueryWSToplistThread(QObject *parent)
    : MusicDownLoadQueryToplistThread(parent)
{
    m_queryServer = "WuSing";
}

QString MusicDownLoadQueryWSToplistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWSToplistThread::startToSearch(QueryType type, const QString &toplist)
{
    if(type == MusicQuery)
    {
        startToSearch(toplist);
    }
    else
    {
        startToSearch(toplist.isEmpty() ? "yc" : toplist);
    }
}

void MusicDownLoadQueryWSToplistThread::startToSearch(const QString &toplist)
{
    if(!m_manager)
    {
        return;
    }

    m_interrupt = true;
    getUpdateTime();

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(toplist));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WS_SONG_TOPLIST_URL, false).arg(toplist).arg(1).arg(100).arg(m_updateTime);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WS_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryWSToplistThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                MusicPlaylistItem info;
                info.m_name = value["name"].toString();
                info.m_coverUrl = value["photo"].toString();
                info.m_playCount = "-";
                info.m_description = "-";
                info.m_updateTime = m_updateTime;
                emit createToplistInfoItem(info);
                ////////////////////////////////////////////////////////////
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = value["SN"].toString();
                    musicInfo.m_songId = QString::number(value["ID"].toInt());
                    musicInfo.m_lrcUrl = "-";
                    musicInfo.m_timeLength = "-";

                    QVariantMap artistsMap = value["user"].toMap();
                    musicInfo.m_artistId = QString::number(artistsMap["ID"].toULongLong());
                    musicInfo.m_singerName = artistsMap["NN"].toString();
                    musicInfo.m_smallPicUrl = artistsMap["I"].toString();

                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicSongAttribute(&musicInfo, value["SK"].toString(), m_searchQuality, m_queryAllRecords);
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName = musicInfo.m_albumName;
                    item.m_time = musicInfo.m_timeLength;
                    item.m_type = mapQueryServerString();
                    emit createSearchedItems(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryWSToplistThread::getUpdateTime()
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 getUpdateTime").arg(getClassName()));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WS_SONG_TOPLIST_T_URL, false);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WS_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 0 && value.contains("data"))
        {
            value = value["data"].toMap();
            MusicObject::MIntList years;
            foreach(const QString &year, value.keys())
            {
                years << year.toInt();
            }
            qSort(years);

            QVariantList datas = value[QString::number(years.last())].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                m_updateTime = var.toString();
                break;
            }
        }
    }
}
