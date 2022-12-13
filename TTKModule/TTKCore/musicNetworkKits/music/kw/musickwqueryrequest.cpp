#include "musickwqueryrequest.h"

MusicKWQueryRequest::MusicKWQueryRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_pageSize = 40;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(QString("%1 startToPage %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_SONG_SEARCH_URL, false).arg(m_queryValue).arg(offset).arg(m_pageSize));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicKWQueryRequest::startToSearch(QueryType type, const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className(), value));

    m_queryType = type;
    m_queryValue = value.trimmed();
    MusicAbstractQueryRequest::downLoadFinished();

    startToPage(0);
}

void MusicKWQueryRequest::startToSingleSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSingleSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_SONG_INFO_URL, false).arg(value));
    MusicKWInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downLoadSingleFinished()));
    QtNetworkErrorConnect(reply, this, replyError);
}

void MusicKWQueryRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll().replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("abslist"))
            {
                m_totalSize = value["TOTAL"].toInt();
                const QVariantList &datas = value["abslist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicObject::MusicSongInformation info;
                    info.m_singerName = MusicUtils::String::charactersReplaced(value["ARTIST"].toString());
                    info.m_songName = MusicUtils::String::charactersReplaced(value["SONGNAME"].toString());
                    info.m_duration = TTKTime::msecTime2LabelJustified(value["DURATION"].toInt() * 1000);

                    info.m_songId = value["MUSICRID"].toString().replace("MUSIC_", "");
                    info.m_artistId = value["ARTISTID"].toString();
                    info.m_albumId = value["ALBUMID"].toString();

                    info.m_year = value["RELEASEDATE"].toString();
                    info.m_trackNumber = "0";

                    info.m_coverUrl = MusicUtils::Algorithm::mdII(KW_ALBUM_COVER_URL, false).arg(info.m_songId);
                    info.m_lrcUrl = MusicUtils::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info.m_songId);
                    info.m_albumName = MusicUtils::String::charactersReplaced(value["ALBUM"].toString());

                    if(!m_queryLite)
                    {
                        TTK_NETWORK_QUERY_CHECK();
                        parseFromSongProperty(&info, value["FORMATS"].toString(), m_queryQuality, m_queryAllRecords);
                        TTK_NETWORK_QUERY_CHECK();

                        if(info.m_songProps.isEmpty())
                        {
                            continue;
                        }

                        if(!findUrlFileSize(&info.m_songProps))
                        {
                            return;
                        }

                        MusicResultInfoItem item;
                        item.m_songName = info.m_songName;
                        item.m_singerName = info.m_singerName;
                        item.m_albumName = info.m_albumName;
                        item.m_duration = info.m_duration;
                        item.m_type = mapQueryServerString();
                        Q_EMIT createSearchedItem(item);
                    }
                    m_songInfos << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicKWQueryRequest::downLoadSingleFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadSingleFinished").arg(className()));

    MusicAbstractQueryRequest::downLoadFinished();
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();

                MusicObject::MusicSongInformation info;
                info.m_singerName = MusicUtils::String::charactersReplaced(value["artist"].toString());
                info.m_songName = MusicUtils::String::charactersReplaced(value["name"].toString());
                info.m_duration = TTKTime::msecTime2LabelJustified(value["duration"].toInt() * 1000);

                info.m_songId = value["rid"].toString();
                info.m_artistId = value["artistid"].toString();
                info.m_albumId = value["albumid"].toString();

                info.m_year = value["releaseDate"].toString();
                info.m_trackNumber = value["track"].toString();

                info.m_coverUrl = MusicUtils::Algorithm::mdII(KW_ALBUM_COVER_URL, false).arg(info.m_songId);
                info.m_lrcUrl = MusicUtils::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info.m_songId);
                info.m_albumName = MusicUtils::String::charactersReplaced(value["album"].toString());

                TTK_NETWORK_QUERY_CHECK();
                parseFromSongProperty(&info, "MP3128|MP3192|MP3H|ALFLAC|AL", m_queryQuality, m_queryAllRecords);
                TTK_NETWORK_QUERY_CHECK();

                if(!findUrlFileSize(&info.m_songProps))
                {
                    return;
                }

                if(!info.m_songProps.isEmpty())
                {
                    MusicResultInfoItem item;
                    item.m_songName = info.m_songName;
                    item.m_singerName = info.m_singerName;
                    item.m_albumName = info.m_albumName;
                    item.m_duration = info.m_duration;
                    item.m_type = mapQueryServerString();
                    Q_EMIT createSearchedItem(item);
                    m_songInfos << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
