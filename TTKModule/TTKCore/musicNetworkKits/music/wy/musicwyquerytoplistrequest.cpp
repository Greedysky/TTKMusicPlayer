#include "musicwyquerytoplistrequest.h"

MusicWYQueryToplistRequest::MusicWYQueryToplistRequest(QObject *parent)
    : MusicQueryToplistRequest(parent)
{
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryToplistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();
    m_queryValue = value.isEmpty() ? "19723756" : value;

    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_TOPLIST_URL, false),
                      TTK::Algorithm::mdII(WY_TOPLIST_DATA_URL, false).arg(m_queryValue));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryToplistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    MusicPageQueryRequest::downLoadFinished();
    TTK_INFO_STREAM(QString("%1 startToQueryResult %2 %3kbps").arg(className(), info->m_songId).arg(bitrate));

    TTK_NETWORK_QUERY_CHECK();
    MusicWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    MusicQueryToplistRequest::startToQueryResult(info, bitrate);
}

void MusicWYQueryToplistRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

    MusicQueryToplistRequest::downLoadFinished();
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
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());
                    info.m_duration = TTKTime::formatDuration(value["dt"].toInt());
                    info.m_songId = value["id"].toString();
                    info.m_lrcUrl = TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(info.m_songId);

                    const QVariantMap &albumObject = value["al"].toMap();
                    info.m_coverUrl = albumObject["picUrl"].toString();
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
                        info.m_singerName = TTK::String::charactersReplace(artistObject["name"].toString());
                        break; //just find first singer
                    }

                    info.m_year.clear();
                    info.m_trackNumber = value["no"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    MusicWYInterface::parseFromSongProperty(&info, value);
                    TTK_NETWORK_QUERY_CHECK();

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

void MusicWYQueryToplistRequest::queryToplistInfo(const QVariantMap &input)
{
    MusicResultDataItem result;
    result.m_name = input["name"].toString();
    result.m_coverUrl = input["coverImgUrl"].toString();
    result.m_playCount = input["playCount"].toString();
    result.m_description = input["description"].toString();
    result.m_updateTime = TTKDateTime::format(input["updateTime"].toULongLong(), TTK_YEAR_FORMAT);
    Q_EMIT createToplistItem(result);
}
