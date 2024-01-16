#include "musickwqueryalbumrequest.h"

MusicKWQueryAlbumRequest::MusicKWQueryAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_pageSize = SONG_PAGE_SIZE;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryAlbumRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_ALBUM_URL, false).arg(m_queryValue));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryAlbumRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << "startToQueryResult" << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downLoadFinished();
    TTK_NETWORK_QUERY_CHECK();
    MusicKWInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    findUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQueryAlbumRequest::startToQueryResult(info, bitrate);
}

void MusicKWQueryAlbumRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll().replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("musiclist"))
            {
                MusicResultDataItem result;
                result.m_id = value["albumid"].toString();
                result.m_coverUrl = MusicKWInterface::makeCoverPixmapUrl(value["pic"].toString(), {});
                const QString &albumName = value["name"].toString();
                result.m_count = albumName;
                result.m_description = value["company"].toString();
                result.m_updateTime = value["pub"].toString();
                result.m_category = value["lang"].toString();

                m_totalSize = value["songnum"].toInt();

                const QVariantList &datas = value["musiclist"].toList();
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

                    info.m_artistId = value["artistid"].toString();
                    info.m_artistName = TTK::String::charactersReplace(value["artist"].toString());

                    info.m_albumId = result.m_id;
                    info.m_albumName = TTK::String::charactersReplace(albumName);

                    info.m_coverUrl = MusicKWInterface::makeCoverPixmapUrl(value["web_albumpic_short"].toString(), info.m_songId);
                    info.m_lrcUrl = TTK::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info.m_songId);
                    info.m_duration = TTK_DEFAULT_STR;
                    info.m_year.clear();
                    info.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    MusicKWInterface::parseFromSongProperty(&info, value["formats"].toString());
                    TTK_NETWORK_QUERY_CHECK();

                    if(!m_albumFound)
                    {
                        m_albumFound = true;
                        result.m_id = info.m_albumId;
                        result.m_name = info.m_artistName;
                        Q_EMIT createAlbumItem(result);
                    }

                    Q_EMIT createResultItem({info, serverToString()});
                    m_songInfos << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}



MusicKWQueryArtistAlbumRequest::MusicKWQueryArtistAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_pageSize = ARTIST_ATTR_PAGE_SIZE;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryArtistAlbumRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_ARTIST_ALBUM_URL, false).arg(m_queryValue).arg(offset).arg(m_pageSize));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryArtistAlbumRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll().replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("albumlist"))
            {
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["albumlist"].toList();
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
                    result.m_coverUrl = MusicKWInterface::makeCoverPixmapUrl(value["pic"].toString(), {});
                    result.m_name = value["name"].toString();
                    result.m_updateTime = value["pub"].toString().replace(TTK_DEFAULT_STR, TTK_DOT);
                    Q_EMIT createAlbumItem(result);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
