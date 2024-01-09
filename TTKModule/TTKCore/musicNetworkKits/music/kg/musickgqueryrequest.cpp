#include "musickgqueryrequest.h"

MusicKGQueryRequest::MusicKGQueryRequest(QObject *parent)
    : MusicQueryRequest(parent)
{
    m_pageSize = SONG_PAGE_SIZE;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_SEARCH_URL, false).arg(m_queryValue).arg(offset + 1).arg(m_pageSize));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryRequest::startToSearchByID(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearchByID" << value);

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_INFO_URL, false).arg(value));
    MusicKGInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downLoadSingleFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << "startToQueryResult" << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downLoadFinished();
    TTK_NETWORK_QUERY_CHECK();
    MusicKGInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();
    MusicQueryRequest::startToQueryResult(info, bitrate);
}

void MusicKGQueryRequest::downLoadFinished()
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
                    MusicKGInterface::parseFromSongAlbumLrc(&info);
                    TTK_NETWORK_QUERY_CHECK();

                    if(m_queryMode != QueryMode::Meta)
                    {
                        TTK_NETWORK_QUERY_CHECK();
                        MusicKGInterface::parseFromSongProperty(&info, value);
                        TTK_NETWORK_QUERY_CHECK();

                        Q_EMIT createResultItem({info, serverToString()});
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
    TTK_INFO_STREAM(className() << "downLoadSingleFinished");

    MusicQueryRequest::downLoadFinished();
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
                info.m_singerName = TTK::String::charactersReplace(value["singername"].toString());
                info.m_songName = TTK::String::charactersReplace(value["songname"].toString());
                info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);

                info.m_songId = value["hash"].toString();
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
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongAlbumInfo(&info, albumObject["album_audio_id"].toString());
                    TTK_NETWORK_QUERY_CHECK();
                }

                info.m_year.clear();
                info.m_trackNumber = "0";

                TTK_NETWORK_QUERY_CHECK();
                MusicKGInterface::parseFromSongProperty(&info, value["extra"].toMap());
                TTK_NETWORK_QUERY_CHECK();

                Q_EMIT createResultItem({info, serverToString()});
                m_songInfos << info;
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
