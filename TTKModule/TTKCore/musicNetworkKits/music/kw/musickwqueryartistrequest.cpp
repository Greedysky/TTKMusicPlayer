#include "musickwqueryartistrequest.h"

MusicKWQueryArtistRequest::MusicKWQueryArtistRequest(QObject *parent)
    : MusicQueryArtistRequest(parent)
{
    m_pageSize = ARTIST_PAGE_SIZE;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryArtistRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_ARTIST_URL, false).arg(m_queryValue).arg(offset).arg(m_pageSize));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryArtistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << "startToQueryResult" << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downLoadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqKWInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    findUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQueryArtistRequest::startToQueryResult(info, bitrate);
}

void MusicKWQueryArtistRequest::downLoadFinished()
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

                    TTK_NETWORK_QUERY_CHECK();
                    ReqKWInterface::parseFromSongProperty(&info, value["FORMATS"].toString());
                    TTK_NETWORK_QUERY_CHECK();

                    if(!m_artistFound)
                    {
                        m_artistFound = true;
                        MusicResultDataItem result;
                        TTK_NETWORK_QUERY_CHECK();
                        queryArtistIntro(&result);
                        TTK_NETWORK_QUERY_CHECK();

                        result.m_id = info.m_artistId;
                        result.m_name = info.m_artistName;
                        Q_EMIT createArtistItem(result);
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

void MusicKWQueryArtistRequest::queryArtistIntro(MusicResultDataItem *item) const
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_ARTIST_INFO_URL, false).arg(m_queryValue));
    ReqKWInterface::makeRequestRawHeader(&request);

    QByteArray bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes.replace("'", "\""), &ok);
    if(ok)
    {
        const QVariantMap &value = data.toMap();
        item->m_updateTime = value["birthday"].toString();
        item->m_nickName = value["aartist"].toString();
        item->m_coverUrl = value["hts_pic"].toString();
        item->m_description = TTK::String::convertHtmlToPlain(value["info"].toString());

        if(item->m_nickName.isEmpty())
        {
            item->m_nickName = TTK_DEFAULT_STR;
        }
    }
}
