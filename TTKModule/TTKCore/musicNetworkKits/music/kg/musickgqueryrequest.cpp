#include "musickgqueryrequest.h"

MusicKGQueryRequest::MusicKGQueryRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryRequest::startToSearch(QueryType type, const QString &text)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), text));

    m_currentType = type;
    m_queryText = text.trimmed();
    MusicAbstractQueryRequest::downLoadFinished();

    startToPage(0);
}

void MusicKGQueryRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToPage %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_SONG_SEARCH_URL, false).arg(m_queryText).arg(offset + 1).arg(m_pageSize));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKGQueryRequest::startToSingleSearch(const QString &text)
{
    TTK_LOGGER_INFO(QString("%1 startToSingleSearch %2").arg(className(), text));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_SONG_INFO_URL, false).arg(text));
    MusicKGInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downLoadSingleFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKGQueryRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                m_totalSize = value["total"].toInt();
                const QVariantList &datas = value["info"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = MusicUtils::String::charactersReplaced(value["singername"].toString());
                    musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["songname"].toString());
                    musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt() * 1000);

                    musicInfo.m_songId = value["hash"].toString();
                    musicInfo.m_albumId = value["album_id"].toString();
                    musicInfo.m_albumName = MusicUtils::String::charactersReplaced(value["album_name"].toString());

                    musicInfo.m_year = QString();
                    musicInfo.m_discNumber = "1";
                    musicInfo.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongLrcAndPicture(&musicInfo);
                    TTK_NETWORK_QUERY_CHECK();

                    if(!m_querySimplify)
                    {
                        TTK_NETWORK_QUERY_CHECK();
                        readFromMusicSongProperty(&musicInfo, value, m_queryQuality, m_queryAllRecords);
                        TTK_NETWORK_QUERY_CHECK();

                        if(musicInfo.m_songProps.isEmpty())
                        {
                            continue;
                        }
                        //
                        MusicSearchedItem item;
                        item.m_songName = musicInfo.m_songName;
                        item.m_singerName = musicInfo.m_singerName;
                        item.m_albumName = musicInfo.m_albumName;
                        item.m_duration = musicInfo.m_duration;
                        item.m_type = mapQueryServerString();
                        Q_EMIT createSearchedItem(item);
                    }
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicKGQueryRequest::downLoadSingleFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadSingleFinished").arg(className()));

    MusicAbstractQueryRequest::downLoadFinished();
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["errcode"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();
                MusicObject::MusicSongInformation musicInfo;
                musicInfo.m_songId = value["hash"].toString();
                musicInfo.m_singerName = MusicUtils::String::charactersReplaced(value["singername"].toString());
                musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["songname"].toString());
                musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt() * 1000);
                musicInfo.m_artistId = QString::number(value["singerid"].toULongLong());
                musicInfo.m_coverUrl = value["imgurl"].toString().replace("{size}", "480");
                musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(KG_SONG_LRC_URL, false)
                                                        .arg(musicInfo.m_songName, musicInfo.m_songId)
                                                        .arg(value["duration"].toInt() * 1000);
                const QVariantList &albumArray = value["album"].toList();
                for(const QVariant &var : qAsConst(albumArray))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    const QVariantMap &albumMap = var.toMap();
                    musicInfo.m_albumId = albumMap["album_audio_id"].toString();
                    musicInfo.m_albumName = MusicUtils::String::charactersReplaced(albumMap["album_name"].toString());
                }

                musicInfo.m_year = QString();
                musicInfo.m_discNumber = "1";
                musicInfo.m_trackNumber = "0";

                TTK_NETWORK_QUERY_CHECK();
                readFromMusicSongProperty(&musicInfo, value["extra"].toMap(), m_queryQuality, true);
                TTK_NETWORK_QUERY_CHECK();

                if(!musicInfo.m_songProps.isEmpty())
                {
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName = musicInfo.m_albumName;
                    item.m_duration = musicInfo.m_duration;
                    item.m_type = mapQueryServerString();
                    Q_EMIT createSearchedItem(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
