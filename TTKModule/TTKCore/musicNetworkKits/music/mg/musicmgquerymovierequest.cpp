#include "musicmgquerymovierequest.h"
#include "musicmgqueryinterface.h"
#include "musicsemaphoreloop.h"

MusicMGQueryMovieRequest::MusicMGQueryMovieRequest(QObject *parent)
    : MusicQueryMovieRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_MG_INTERFACE;
}

void MusicMGQueryMovieRequest::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));

    deleteAll();
    m_searchText = text.trimmed();
    m_currentType = type;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(MG_SONG_SEARCH_URL, false).arg(m_searchText).arg(0).arg(m_pageSize));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL, ALG_UA_KEY, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(MG_REFERER_URL, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicMGQueryMovieRequest::startToPage(int offset)
{
    Q_UNUSED(offset);

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicMGQueryMovieRequest::startToSingleSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSingleSearch %2").arg(getClassName()).arg(text));

    deleteAll();
    m_searchText = text.trimmed();

    QTimer::singleShot(MT_MS, this, SLOT(singleDownLoadFinished()));
}

void MusicMGQueryMovieRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
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
                const QVariantList &datas = value["musics"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    if(value["hasMv"].toString().isEmpty())
                    {
                        continue;
                    }

                    const QString &mvid = value["mvCopyrightId"].toString();
                    TTK_NETWORK_QUERY_CHECK();
                    startMVListQuery(mvid);
                    TTK_NETWORK_QUERY_CHECK();
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicMGQueryMovieRequest::pageDownLoadFinished()
{

}

void MusicMGQueryMovieRequest::singleDownLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 singleDownLoadFinished").arg(getClassName()));

    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
    setNetworkAbort(false);

    if(!m_searchText.isEmpty())
    {
        TTK_NETWORK_QUERY_CHECK();
        startMVListQuery(m_searchText);
        TTK_NETWORK_QUERY_CHECK();
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicMGQueryMovieRequest::startMVListQuery(const QString &id)
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(MG_MOVIE_URL, false).arg(id));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL, ALG_UA_KEY, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(MG_REFERER_URL, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

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
    const QVariant &data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("data"))
        {
            value = value["data"].toMap();
            MusicObject::MusicSongInformation musicInfo;
            musicInfo.m_songId = id;
            musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["contentName"].toString());
            musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(value["actorName"].toString());
            musicInfo.m_timeLength = STRING_NULL;

            value = value["videoUrlMap"].toMap();
            const QVariantList &datas = value["entry"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                MusicObject::MusicSongAttribute attr;
                const QString &key = value["key"].toString();
                if(key == "050014")
                    attr.m_bitrate = MB_250;
                else if(key == "050013")
                    attr.m_bitrate = MB_500;
                else if(key == "050019")
                    attr.m_bitrate = MB_750;
                else if(key == "050018")
                    attr.m_bitrate = MB_1000;

                attr.m_url = value["value"].toString();
                attr.m_format = MusicUtils::String::stringSplitToken(attr.m_url);
                //
                if(!findUrlFileSize(&attr)) return;
                //
                musicInfo.m_songAttrs.append(attr);
            }

            if(musicInfo.m_songAttrs.isEmpty())
            {
                return;
            }
            //
            MusicSearchedItem item;
            item.m_songName = musicInfo.m_songName;
            item.m_singerName = musicInfo.m_singerName;
            item.m_time = musicInfo.m_timeLength;
            item.m_type = mapQueryServerString();
            Q_EMIT createSearchedItem(item);
            m_musicSongInfos << musicInfo;
        }
    }
}
