#include "musickgqueryrequest.h"

MusicKGQueryRequest::MusicKGQueryRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(QString("%1 startToPage %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_SEARCH_URL, false).arg(m_queryValue).arg(offset + 1).arg(m_pageSize));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicKGQueryRequest::startToSearch(QueryType type, const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className(), value));

    m_queryType = type;
    m_queryValue = value.trimmed();
    MusicAbstractQueryRequest::downLoadFinished();

    startToPage(0);
}

void MusicKGQueryRequest::startToSingleSearch(const QString &id)
{
    TTK_INFO_STREAM(QString("%1 startToSingleSearch %2").arg(className(), id));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_INFO_URL, false).arg(id));
    MusicKGInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downLoadSingleFinished()));
    QtNetworkErrorConnect(reply, this, replyError);
}

void MusicKGQueryRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
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

                    TTK::MusicSongInformation info;
                    info.m_singerName = TTK::String::charactersReplace(value["singername"].toString());
                    info.m_songName = TTK::String::charactersReplace(value["songname"].toString());
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);

                    info.m_songId = value["hash"].toString();
                    info.m_albumId = value["album_id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(value["album_name"].toString());

                    info.m_year.clear();
                    info.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongLrcAndPicture(&info);
                    TTK_NETWORK_QUERY_CHECK();

                    if(m_queryMode != QueryMode::None)
                    {
                        if(m_queryMode != QueryMode::List)
                        {
                            TTK_NETWORK_QUERY_CHECK();
                            MusicKGInterface::parseFromSongProperty(&info, value, true);
                            TTK_NETWORK_QUERY_CHECK();

                            if(info.m_songProps.isEmpty())
                            {
                                continue;
                            }
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

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicKGQueryRequest::downLoadSingleFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadSingleFinished").arg(className()));

    MusicAbstractQueryRequest::downLoadFinished();
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["errcode"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();
                TTK::MusicSongInformation info;
                info.m_songId = value["hash"].toString();
                info.m_singerName = TTK::String::charactersReplace(value["singername"].toString());
                info.m_songName = TTK::String::charactersReplace(value["songname"].toString());
                info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);
                info.m_artistId = value["singerid"].toString();
                info.m_coverUrl = value["imgurl"].toString().replace("{size}", "480");
                info.m_lrcUrl = TTK::Algorithm::mdII(KG_SONG_LRC_URL, false).arg(info.m_songName, info.m_songId).arg(value["duration"].toInt() * TTK_DN_S2MS);

                const QVariantList &albumArray = value["album"].toList();
                for(const QVariant &var : qAsConst(albumArray))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    const QVariantMap &albumObject = var.toMap();
                    info.m_albumId = albumObject["album_audio_id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(albumObject["album_name"].toString());
                }

                info.m_year.clear();
                info.m_trackNumber = "0";

                TTK_NETWORK_QUERY_CHECK();
                MusicKGInterface::parseFromSongProperty(&info, value["extra"].toMap(), true);
                TTK_NETWORK_QUERY_CHECK();

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

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
