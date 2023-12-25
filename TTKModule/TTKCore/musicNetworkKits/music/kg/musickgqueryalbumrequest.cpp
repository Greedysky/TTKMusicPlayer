#include "musickgqueryalbumrequest.h"

MusicKGQueryAlbumRequest::MusicKGQueryAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryAlbumRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();
    m_queryValue = value;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ALBUM_URL, false).arg(value));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryAlbumRequest::startToSingleSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSingleSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ARTIST_ALBUM_URL, false).arg(value));
    MusicKGInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downLoadSingleFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryAlbumRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    MusicPageQueryRequest::downLoadFinished();
    TTK_INFO_STREAM(QString("%1 startToQueryResult %2 %3kbps").arg(className(), info->m_songId).arg(bitrate));

    TTK_NETWORK_QUERY_CHECK();
    MusicKGInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    MusicQueryAlbumRequest::startToQueryResult(info, bitrate);
}

void MusicKGQueryAlbumRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

    MusicQueryAlbumRequest::downLoadFinished();
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
                bool albumFound = false;
                MusicResultDataItem result;
                value = value["data"].toMap();

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
                    info.m_songName = TTK::String::charactersReplace(value["filename"].toString());
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);

                    if(info.m_songName.contains(TTK_DEFAULT_STR))
                    {
                        const QStringList &ll = info.m_songName.split(TTK_DEFAULT_STR);
                        info.m_singerName = TTK::String::charactersReplace(ll.front().trimmed());
                        info.m_songName = TTK::String::charactersReplace(ll.back().trimmed());
                    }

                    info.m_songId = value["hash"].toString();
                    info.m_albumId = value["album_id"].toString();

                    info.m_year.clear();
                    info.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongAlbumInfo(&result, m_queryValue);
                    info.m_albumName = TTK::String::charactersReplace(result.m_nickName);
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongLrcAndPicture(&info);
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongProperty(&info, value);
                    TTK_NETWORK_QUERY_CHECK();

                    if(!albumFound)
                    {
                        albumFound = true;
                        result.m_id = info.m_albumId;
                        result.m_name = info.m_singerName;
                        result.m_coverUrl = info.m_coverUrl;
                        Q_EMIT createAlbumItem(result);
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

void MusicKGQueryAlbumRequest::downLoadSingleFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadSingleFinished").arg(className()));

    MusicPageQueryRequest::downLoadFinished();
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

                const QVariantList &datas = value["info"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem result;
                    result.m_id = value["albumid"].toString();
                    result.m_coverUrl = value["imgurl"].toString().replace("{size}", "400");
                    result.m_name = value["albumname"].toString();
                    result.m_updateTime = TTK::String::split(value["publishtime"].toString().replace(TTK_DEFAULT_STR, TTK_DOT), " ").front();
                    Q_EMIT createAlbumItem(result);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
