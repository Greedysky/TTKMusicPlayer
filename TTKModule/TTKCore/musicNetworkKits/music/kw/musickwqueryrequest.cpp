#include "musickwqueryrequest.h"

MusicKWQueryRequest::MusicKWQueryRequest(QObject *parent)
    : MusicQueryRequest(parent)
{
    m_pageSize = SONG_PAGE_SIZE;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_SEARCH_URL, false).arg(m_queryValue).arg(offset).arg(m_pageSize));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryRequest::startToSearchByID(const QString &value)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_INFO_URL, false).arg(value));
    ReqKWInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downLoadSingleFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downLoadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqKWInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQueryRequest::startToQueryResult(info, bitrate);
}

void MusicKWQueryRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
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

                    TTK::MusicSongInformation info;
                    info.m_songId = value["MUSICRID"].toString().remove("MUSIC_");
                    info.m_songName = TTK::String::charactersReplace(value["SONGNAME"].toString());

                    info.m_artistId = value["ARTISTID"].toString();
                    info.m_artistName = ReqKWInterface::makeSongArtist(value["ARTIST"].toString());

                    info.m_albumId = value["ALBUMID"].toString();
                    info.m_albumName = TTK::String::charactersReplace(value["ALBUM"].toString());

                    info.m_coverUrl = ReqKWInterface::makeCoverPixmapUrl(value["web_albumpic_short"].toString(), info.m_songId);
                    info.m_lrcUrl = TTK::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["DURATION"].toInt() * TTK_DN_S2MS);
                    info.m_year = value["RELEASEDATE"].toString();
                    info.m_trackNumber = "0";

                    if(m_queryMode != QueryMode::Meta)
                    {
                        TTK_NETWORK_QUERY_CHECK();
                        ReqKWInterface::parseFromSongProperty(&info, value["FORMATS"].toString());
                        TTK_NETWORK_QUERY_CHECK();

                        Q_EMIT createResultItem({info, serverToString()});
                    }

                    m_items << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicKWQueryRequest::downLoadSingleFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

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
            if(value.contains("data"))
            {
                value = value["data"].toMap();

                TTK::MusicSongInformation info;
                info.m_songId = value["rid"].toString();
                info.m_songName = TTK::String::charactersReplace(value["name"].toString());

                info.m_artistId = value["artistid"].toString();
                info.m_artistName = ReqKWInterface::makeSongArtist(value["artist"].toString());

                info.m_albumId = value["albumid"].toString();
                info.m_albumName = TTK::String::charactersReplace(value["album"].toString());

                info.m_coverUrl = value["pic"].toString();
                info.m_lrcUrl = TTK::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info.m_songId);
                info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);
                info.m_year = value["releaseDate"].toString();
                info.m_trackNumber = value["track"].toString();

                TTK_NETWORK_QUERY_CHECK();
                ReqKWInterface::parseFromSongProperty(&info, "MP3128|MP3192|MP3H");
                TTK_NETWORK_QUERY_CHECK();

                Q_EMIT createResultItem({info, serverToString()});
                m_items << info;
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
