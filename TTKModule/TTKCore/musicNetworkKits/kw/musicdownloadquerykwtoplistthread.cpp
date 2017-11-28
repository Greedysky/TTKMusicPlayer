#include "musicdownloadquerykwtoplistthread.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKWToplistThread::MusicDownLoadQueryKWToplistThread(QObject *parent)
    : MusicDownLoadQueryToplistThread(parent)
{
    m_queryServer = "Kuwo";
}

QString MusicDownLoadQueryKWToplistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryKWToplistThread::startToSearch(QueryType type, const QString &toplist)
{
    if(type == MusicQuery)
    {
        startToSearch(toplist);
    }
    else
    {
        startToSearch(toplist.isEmpty() ? "93" : toplist);
    }
}

void MusicDownLoadQueryKWToplistThread::startToSearch(const QString &toplist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch").arg(getClassName()));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_SONG_TOPLIST_URL, false).arg(toplist);
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryKWToplistThread::downLoadFinished()
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

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["status"].toInt() == 200 && value.contains("data"))
            {
                value = value["data"].toMap();
                MusicPlaylistItem info;
                info.m_name = value["name"].toString();
                info.m_coverUrl = value["pic"].toString();
                info.m_playCount = "-";
                info.m_description = value["info"].toString();
                info.m_updateTime = value["pub"].toString();
                emit createToplistInfoItem(info);
                ////////////////////////////////////////////////////////////
                QVariantList datas = value["musicList"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = value["artist"].toString();
                    musicInfo.m_songName = value["songName"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt()*1000);

                    musicInfo.m_songId = value["id"].toString();
                    musicInfo.m_artistId = value["artistId"].toString();
                    musicInfo.m_albumId = value["albumid"].toString();
                    musicInfo.m_albumName = value["albumName"].toString();

                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicSongPic(&musicInfo, musicInfo.m_songId);
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(musicInfo.m_songId);
                    ///music normal songs urls
                    readFromMusicSongAttribute(&musicInfo, value["formats"].toList(), m_searchQuality, m_queryAllRecords);
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    ////////////////////////////////////////////////////////////
                    for(int i=0; i<musicInfo.m_songAttrs.count(); ++i)
                    {
                        MusicObject::MusicSongAttribute *attr = &musicInfo.m_songAttrs[i];
                        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        attr->m_size = MusicUtils::Number::size2Label(getUrlFileSize(attr->m_url));
                        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
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
