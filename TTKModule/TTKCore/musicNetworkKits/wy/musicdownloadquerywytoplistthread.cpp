#include "musicdownloadquerywytoplistthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryWYToplistThread::MusicDownLoadQueryWYToplistThread(QObject *parent)
    : MusicDownLoadQueryToplistThread(parent)
{
    m_queryServer = "WangYi";
}

QString MusicDownLoadQueryWYToplistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWYToplistThread::startToSearch(QueryType type, const QString &toplist)
{
    if(type == MusicQuery)
    {
        startToSearch(toplist);
    }
    else
    {
        startToSearch(toplist.isEmpty() ? "19723756" : toplist);
    }
}

void MusicDownLoadQueryWYToplistThread::startToSearch(const QString &toplist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(toplist));
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    QByteArray parameter = makeTokenQueryUrl(&request,
               MusicUtils::Algorithm::mdII(WY_SG_TOPLIST_N_URL, false),
               MusicUtils::Algorithm::mdII(WY_SG_TOPLIST_NDT_URL, false).arg(toplist));
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryWYToplistThread::downLoadFinished()
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
            if(value["code"].toInt() == 200 && value.contains("playlist"))
            {
                value = value["playlist"].toMap();
                MusicPlaylistItem info;
                info.m_name = value["name"].toString();
                info.m_coverUrl = value["coverImgUrl"].toString();
                info.m_playCount = QString::number(value["playCount"].toULongLong());
                info.m_description = value["description"].toString();
                info.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["updateTime"].toULongLong()).toString("yyyy-MM-dd");
                emit createToplistInfoItem(info);
                ////////////////////////////////////////////////////////////
                QVariantList datas = value["tracks"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = value["name"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["dt"].toInt());
                    musicInfo.m_songId = QString::number(value["id"].toInt());
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(WY_SONG_LRC_URL, false).arg(musicInfo.m_songId);

                    QVariantMap albumObject = value["al"].toMap();
                    musicInfo.m_smallPicUrl = albumObject["picUrl"].toString();
                    musicInfo.m_albumId = QString::number(albumObject["id"].toInt());
                    musicInfo.m_albumName = albumObject["name"].toString();

                    QVariantList artistsArray = value["ar"].toList();
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

                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);
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
