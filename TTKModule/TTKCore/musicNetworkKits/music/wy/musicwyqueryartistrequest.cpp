#include "musicwyqueryartistrequest.h"

MusicWYQueryArtistRequest::MusicWYQueryArtistRequest(QObject *parent)
    : MusicQueryArtistRequest(parent)
{
    m_pageSize = TTK_HIGH_LEVEL; // ARTIST_PAGE_SIZE;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryArtistRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = m_pageSize; // 0
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_ARTIST_URL, false).arg(m_queryValue),
                      QString("{}"));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryArtistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << "startToQueryResult" << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downLoadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    findUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQueryArtistRequest::startToQueryResult(info, bitrate);
}

void MusicWYQueryArtistRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("hotSongs"))
            {
                const QVariantMap &artistObject = value["artist"].toMap();
                const QString &coverUrl = artistObject["picUrl"].toString();
                const QString &artistName = TTK::String::charactersReplace(artistObject["name"].toString());

                const QVariantList &datas = value["hotSongs"].toList();
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

                    info.m_artistName = artistName;
                    const QVariantList &artistsArray = value["ar"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        info.m_artistId = artistObject["id"].toString();
                        break;
                    }

                    const QVariantMap &albumObject = value["al"].toMap();
                    info.m_albumId = albumObject["id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(albumObject["name"].toString());

                    info.m_coverUrl = coverUrl;
                    info.m_lrcUrl = TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["dt"].toInt());
                    info.m_year.clear();
                    info.m_trackNumber = value["no"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    ReqWYInterface::parseFromSongProperty(&info, value);
                    TTK_NETWORK_QUERY_CHECK();

                    if(!m_artistFound)
                    {
                        m_artistFound = true;
                        MusicResultDataItem item;
                        TTK_NETWORK_QUERY_CHECK();
                        queryArtistIntro(&item);
                        TTK_NETWORK_QUERY_CHECK();

                        item.m_nickName.clear();
                        const QVariantList &aliasArray = artistObject["alias"].toList();
                        for(const QVariant &aliasValue : qAsConst(aliasArray))
                        {
                            if(aliasValue.isNull())
                            {
                                continue;
                            }

                            item.m_nickName += aliasValue.toString() + ",";
                        }

                        if(!item.m_nickName.isEmpty())
                        {
                            item.m_nickName.chop(1);
                        }
                        else
                        {
                            item.m_nickName = TTK_DEFAULT_STR;
                        }

                        item.m_id = info.m_artistId;
                        item.m_name = info.m_artistName;
                        item.m_coverUrl = info.m_coverUrl;
                        item.m_updateTime = TTKDateTime::format(artistObject["publishTime"].toLongLong(), TTK_DATE_FORMAT);
                        Q_EMIT createArtistItem(item);
                    }

                    Q_EMIT createResultItem({info, serverToString()});
                    m_items << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicWYQueryArtistRequest::queryArtistIntro(MusicResultDataItem *item) const
{
    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_ARTIST_INFO_URL, false),
                      TTK::Algorithm::mdII(WY_ARTIST_INFO_DATA_URL, false).arg(m_queryValue));

    const QByteArray &bytes = TTK::syncNetworkQueryForPost(&request, parameter);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 200)
        {
            item->m_description = value["briefDesc"].toString();
            if(!item->m_description.isEmpty())
            {
                item->m_description = QString("%1\r\n\r\n").arg(item->m_description);
            }

            const QVariantList &datas = value["introduction"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                item->m_description += QString("**%1**\r\n").arg(value["ti"].toString());
                item->m_description += value["txt"].toString() + "\r\n\r\n";
            }
        }
    }
}
