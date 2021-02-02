#include "musicmgqueryrequest.h"

MusicMGQueryRequest::MusicMGQueryRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_pageSize = 40;
    m_queryServer = QUERY_MG_INTERFACE;
}

void MusicMGQueryRequest::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    m_currentType = type;
    m_searchText = text.trimmed();

    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();

    startToPage(0);
}

void MusicMGQueryRequest::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToPage %2").arg(getClassName()).arg(offset));

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(MG_SONG_SEARCH_URL, false).arg(m_searchText).arg(offset).arg(m_pageSize));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL, ALG_UA_KEY, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(MG_REFERER_URL, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicMGQueryRequest::startToSingleSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSingleSearch %2").arg(getClassName()).arg(text));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(MG_SONG_DETAIL_URL, false).arg(text));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL, ALG_UA_KEY, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(MG_REFERER_URL, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(singleDownLoadFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicMGQueryRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

    setNetworkAbort(false);

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("musics"))
            {
                m_totalSize = value["pgt"].toInt() * m_pageSize;
                const QVariantList &datas = value["musics"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(value["singerName"].toString());
                    musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["songName"].toString());
                    musicInfo.m_timeLength = STRING_NULL;

                    musicInfo.m_songId = value["id"].toString();
                    musicInfo.m_artistId = value["singerId"].toString();
                    musicInfo.m_albumId = value["albumId"].toString();
                    musicInfo.m_smallPicUrl = value["cover"].toString();

                    musicInfo.m_year = QString();
                    musicInfo.m_discNumber = "1";
                    musicInfo.m_trackNumber = "0";

                    if(!m_querySimplify)
                    {
                        musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(MG_SONG_LRC_URL, false).arg(value["copyrightId"].toString());
                        musicInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["albumName"].toString());

                        TTK_NETWORK_QUERY_CHECK();
                        readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);
                        TTK_NETWORK_QUERY_CHECK();

                        if(musicInfo.m_songAttrs.isEmpty())
                        {
                            continue;
                        }
                        //
                        if(!findUrlFileSize(&musicInfo.m_songAttrs)) return;
                        //
                        setTimeLength(&musicInfo);
                        //
                        MusicSearchedItem item;
                        item.m_songName = musicInfo.m_songName;
                        item.m_singerName = musicInfo.m_singerName;
                        item.m_albumName = musicInfo.m_albumName;
                        item.m_time = musicInfo.m_timeLength;
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

void MusicMGQueryRequest::singleDownLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 singleDownLoadFinished").arg(getClassName()));

    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
    setNetworkAbort(false);

    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                TTK_NETWORK_QUERY_CHECK();

                MusicObject::MusicSongInformation musicInfo;
                const QVariantList &singerName = value["singerName"].toList();
                musicInfo.m_singerName = singerName.isEmpty() ? STRING_NULL : MusicUtils::String::illegalCharactersReplaced(singerName.first().toString());
                musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["songName"].toString());
                musicInfo.m_timeLength = STRING_NULL;

                musicInfo.m_songId = value["songId"].toString();
                musicInfo.m_smallPicUrl = value["picS"].toString();

                musicInfo.m_year = QString();
                musicInfo.m_discNumber = "1";
                musicInfo.m_trackNumber = "0";

                musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(MG_SONG_LRC_URL, false).arg(value["copyrightId"].toString());
                musicInfo.m_albumName = musicInfo.m_songName;

                TTK_NETWORK_QUERY_CHECK();
                readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);
                TTK_NETWORK_QUERY_CHECK();

                if(!musicInfo.m_songAttrs.isEmpty())
                {
                    if(!findUrlFileSize(&musicInfo.m_songAttrs)) return;
                    //
                    setTimeLength(&musicInfo);
                    //
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName = musicInfo.m_albumName;
                    item.m_time = musicInfo.m_timeLength;
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
