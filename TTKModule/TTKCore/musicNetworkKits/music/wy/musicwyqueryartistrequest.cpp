#include "musicwyqueryartistrequest.h"

MusicWYQueryArtistRequest::MusicWYQueryArtistRequest(QObject *parent)
    : MusicQueryArtistRequest(parent)
{
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryArtistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();
    m_queryValue = value;

    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_ARTIST_URL, false).arg(value),
                      QString("{}"));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryArtistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(QString("%1 startToQueryResult %2 %3kbps").arg(className(), info->m_songId).arg(bitrate));
    MusicPageQueryRequest::downLoadFinished();

    TTK_NETWORK_QUERY_CHECK();
    MusicWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    MusicQueryArtistRequest::startToQueryResult(info, bitrate);
}

void MusicWYQueryArtistRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

    MusicQueryArtistRequest::downLoadFinished();
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
                bool artistFound = false;
                const QVariantMap &artistObject = value["artist"].toMap();
                const QString &coverUrl = artistObject["picUrl"].toString();
                const QString &singerName = TTK::String::charactersReplace(artistObject["name"].toString());

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
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());
                    info.m_singerName = singerName;
                    info.m_coverUrl = coverUrl;
                    info.m_duration = TTKTime::formatDuration(value["dt"].toInt());
                    info.m_songId = value["id"].toString();
                    info.m_lrcUrl = TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(info.m_songId);

                    const QVariantMap &albumObject = value["al"].toMap();
                    info.m_albumId = albumObject["id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(albumObject["name"].toString());

                    const QVariantList &artistsArray = value["ar"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        info.m_artistId = artistObject["id"].toString();
                        break; //just find first singer
                    }

                    info.m_year.clear();
                    info.m_trackNumber = value["no"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    MusicWYInterface::parseFromSongProperty(&info, value);
                    TTK_NETWORK_QUERY_CHECK();

                    if(!artistFound)
                    {
                        artistFound = true;
                        MusicResultDataItem result;
                        TTK_NETWORK_QUERY_CHECK();
                        queryArtistIntro(&result);
                        TTK_NETWORK_QUERY_CHECK();

                        result.m_nickName.clear();
                        const QVariantList &aliasArray = artistObject["alias"].toList();
                        for(const QVariant &aliasValue : qAsConst(aliasArray))
                        {
                            if(aliasValue.isNull())
                            {
                                continue;
                            }

                            result.m_nickName += aliasValue.toString() + ",";
                        }

                        if(!result.m_nickName.isEmpty())
                        {
                            result.m_nickName.chop(1);
                        }

                        result.m_id = m_queryValue;
                        result.m_name = info.m_singerName;
                        result.m_coverUrl = info.m_coverUrl;
                        Q_EMIT createArtistItem(result);
                    }

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

void MusicWYQueryArtistRequest::queryArtistIntro(MusicResultDataItem *item) const
{
    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request,
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
