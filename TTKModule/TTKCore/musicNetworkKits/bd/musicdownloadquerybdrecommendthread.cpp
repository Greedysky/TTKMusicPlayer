#include "musicdownloadquerybdrecommendthread.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryBDRecommendThread::MusicDownLoadQueryBDRecommendThread(QObject *parent)
    : MusicDownLoadQueryRecommendThread(parent)
{
    m_queryServer = QUERY_BD_INTERFACE;
}

void MusicDownLoadQueryBDRecommendThread::startToSearch(const QString &id)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(id));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_RCM_URL, false).arg(50);
    m_interrupt = true;
    m_searchText = id;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryBDRecommendThread::downLoadFinished()
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
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["error_code"].toInt() == 22000 && value.contains("content"))
            {
                QVariantList datas = value["content"].toList();
                if(!datas.isEmpty())
                {
                    value = datas.first().toMap();
                    datas = value["song_list"].toList();
                    foreach(const QVariant &var, datas)
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        value = var.toMap();
                        MusicObject::MusicSongInformation musicInfo;
                        musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(value["author"].toString());
                        musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["title"].toString());
                        musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["file_duration"].toInt()*1000);

                        musicInfo.m_songId = value["song_id"].toString();
                        musicInfo.m_albumId = value["album_id"].toString();
                        musicInfo.m_artistId = value["ting_uid"].toString();
                        musicInfo.m_lrcUrl = value["lrclink"].toString();
                        musicInfo.m_smallPicUrl = value["pic_small"].toString().replace("_90", "_500");
                        musicInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["album_title"].toString());

                        musicInfo.m_year = value["publishtime"].toString();
                        musicInfo.m_discNumber = "1";
                        musicInfo.m_trackNumber = value["album_no"].toString();

                        if(m_interrupt || m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                        readFromMusicSongAttribute(&musicInfo, value["all_rate"].toString(), m_searchQuality, m_queryAllRecords);
                        if(m_interrupt || m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;

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
                        emit createSearchedItem(item);
                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
