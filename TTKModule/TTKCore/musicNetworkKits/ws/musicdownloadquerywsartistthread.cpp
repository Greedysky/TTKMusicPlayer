#include "musicdownloadquerywsartistthread.h"
#include "musicsemaphoreloop.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryWSArtistThread::MusicDownLoadQueryWSArtistThread(const QString &type, QObject *parent)
    : MusicDownLoadQueryArtistThread(parent)
{
    m_queryServer = "WuSing";
    m_querySearchType = type;
}

QString MusicDownLoadQueryWSArtistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWSArtistThread::startToSearch(const QString &artist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(artist));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WS_ARTIST_URL, false).arg(artist).arg(m_querySearchType).arg(1).arg(40);
    deleteAll();
    m_searchText = artist;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WS_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryWSArtistThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo
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
            if(value["code"].toInt() == 0 && value.contains("data"))
            {
                bool artistFlag = false;
                ////////////////////////////////////////////////////////////
                QVariantList datas = value["data"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = value["SN"].toString();
                    musicInfo.m_songId = QString::number(value["ID"].toULongLong());
                    musicInfo.m_lrcUrl = "-";
                    musicInfo.m_timeLength = "-";

                    QVariantMap artistsMap = value["user"].toMap();
                    musicInfo.m_artistId = QString::number(artistsMap["ID"].toULongLong());
                    musicInfo.m_singerName = artistsMap["NN"].toString();
                    musicInfo.m_smallPicUrl = artistsMap["I"].toString();

                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicSongAttribute(&musicInfo, m_querySearchType, m_searchQuality, m_queryAllRecords);
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    ////////////////////////////////////////////////////////////
                    if(!artistFlag)
                    {
                        artistFlag = true;
                        MusicPlaylistItem info;
                        if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        getDownLoadIntro(&info);
                        if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        info.m_id = m_searchText;
                        info.m_name = musicInfo.m_singerName;
                        info.m_coverUrl = musicInfo.m_smallPicUrl;
                        emit createArtistInfoItem(info);
                    }
                    ////////////////////////////////////////////////////////////
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

void MusicDownLoadQueryWSArtistThread::getDownLoadIntro(MusicPlaylistItem *item)
{
    if(!m_manager)
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(WS_ARTIST_INFO_URL, false).arg(m_searchText)));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WS_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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
        if(value["code"].toInt() == 0)
        {
            value = value["data"].toMap();
            item->m_tags = value["P"].toString();
            item->m_updateTime = value["B"].toString();
            item->m_nickname = "";
            item->m_description = value["M"].toString();
        }
    }
}
