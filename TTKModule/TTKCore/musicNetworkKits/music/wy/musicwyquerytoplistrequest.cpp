#include "musicwyquerytoplistrequest.h"

MusicWYQueryToplistRequest::MusicWYQueryToplistRequest(QObject *parent)
    : MusicQueryToplistRequest(parent)
{
    m_pageSize = TTK_HIGH_LEVEL; // TOPLIST_PAGE_SIZE
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryToplistRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = m_pageSize; // 0
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_TOPLIST_URL, false),
                      TTK::Algorithm::mdII(WY_TOPLIST_DATA_URL, false).arg(m_queryValue).arg(m_pageSize * offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryToplistRequest::startToSearch(const QString &value)
{
    MusicQueryToplistRequest::startToSearch(value.isEmpty() ? "19723756" : value);
}

void MusicWYQueryToplistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downLoadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQueryToplistRequest::startToQueryResult(info, bitrate);
}

void MusicWYQueryToplistRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("playlist"))
            {
                value = value["playlist"].toMap();
//                m_totalSize = value["trackCount"].toInt();

                queryToplistInfo(value);

                const QVariantList &datas = value["tracks"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songId = value["id"].toString();
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());

                    const QVariantList &artistsArray = value["ar"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        if(info.m_artistId.isEmpty())
                        {
                            info.m_artistId = artistObject["id"].toString();
                        }

                        info.m_artistName = ReqWYInterface::makeSongArtist(info.m_artistName, artistObject["name"].toString());
                    }

                    const QVariantMap &albumObject = value["al"].toMap();
                    info.m_albumId = albumObject["id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(albumObject["name"].toString());

                    info.m_coverUrl = albumObject["picUrl"].toString();
                    info.m_lrcUrl = TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["dt"].toInt());
                    info.m_year.clear();
                    info.m_trackNumber = value["no"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    ReqWYInterface::parseFromSongProperty(&info, value);
                    TTK_NETWORK_QUERY_CHECK();

                    Q_EMIT createResultItem({info, serverToString()});
                    m_items << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicWYQueryToplistRequest::queryToplistInfo(const QVariantMap &input)
{
    MusicResultDataItem item;
    item.m_name = input["name"].toString();
    item.m_coverUrl = input["coverImgUrl"].toString();
    item.m_count = input["playCount"].toString();
    item.m_description = input["description"].toString();
    item.m_time = TTKDateTime::format(input["updateTime"].toULongLong(), TTK_DATE_FORMAT);
    Q_EMIT createToplistItem(item);
}
