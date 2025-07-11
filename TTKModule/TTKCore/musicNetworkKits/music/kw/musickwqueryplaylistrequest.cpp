#include "musickwqueryplaylistrequest.h"

MusicKWQueryPlaylistRequest::MusicKWQueryPlaylistRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = SONG_PAGE_SIZE;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryPlaylistRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_PLAYLIST_URL, false).arg(m_queryValue).arg(offset + 1).arg(m_pageSize));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryPlaylistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_PLAYLIST_INFO_URL, false).arg(value));
    ReqKWInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downloadDetailsFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryPlaylistRequest::startToSearchByID(const QString &value)
{
    MusicQueryPlaylistRequest::startToSearchByID(value.isEmpty() ? "393" : value);
}

void MusicKWQueryPlaylistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downLoadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqKWInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQueryPlaylistRequest::startToQueryResult(info, bitrate);
}

void MusicKWQueryPlaylistRequest::startToQueryInfo(MusicResultDataItem &item)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << item.m_id);

    MusicPageQueryRequest::downLoadFinished();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_PLAYLIST_INFO_URL, false).arg(item.m_id));
    ReqKWInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        const QVariantMap &value = json.toVariant().toMap();
        if(!value.isEmpty())
        {
            item.m_coverUrl = value["pic"].toString();
            item.m_name = value["title"].toString();
            item.m_count = value["playnum"].toString();
            item.m_description = value["info"].toString();
            item.m_time = TTKDateTime::format(value["ctime"].toULongLong() * TTK_DN_S2MS, TTK_DATE_FORMAT);
            item.m_nickName = value["uname"].toString();
        }
    }
}

void MusicKWQueryPlaylistRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicQueryPlaylistRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["code"].toInt() == 200 && value.contains("data"))
            {
                value = value["data"].toMap();
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["data"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    morePlaylistDetails(value["id"].toString());
                    TTK_NETWORK_QUERY_CHECK();
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicKWQueryPlaylistRequest::downloadDetailsFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicQueryPlaylistRequest::downLoadFinished();
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("musiclist"))
            {
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
                    info.m_artistName = ReqKWInterface::makeSongArtist(value["artist"].toString());

                    info.m_albumId = value["albumid"].toString();
                    info.m_albumName = TTK::String::charactersReplace(value["album"].toString());

                    info.m_coverUrl = ReqKWInterface::makeCoverPixmapUrl(value["web_albumpic_short"].toString(), info.m_songId);
                    info.m_lrcUrl = TTK::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);
                    info.m_year.clear();
                    info.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    ReqKWInterface::parseFromSongProperty(&info, value["formats"].toString());
                    TTK_NETWORK_QUERY_CHECK();

                    Q_EMIT createResultItem({info, serverToString()});
                    m_items << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
}

void MusicKWQueryPlaylistRequest::downloadMoreDetailsFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            const QVariantMap &value = json.toVariant().toMap();
            if(value["result"].toString() == "ok")
            {
                MusicResultDataItem item;
                item.m_coverUrl = value["pic"].toString();
                item.m_id = value["id"].toString();
                item.m_name = value["title"].toString();
                item.m_count = value["playnum"].toString();
                item.m_description = value["info"].toString();
                item.m_time = TTKDateTime::format(value["ctime"].toULongLong() * TTK_DN_S2MS, TTK_DATE_FORMAT);
                item.m_nickName = value["uname"].toString();
                item.m_category = value["tag"].toString();

                if(!item.m_category.isEmpty())
                {
                    item.m_category.replace(",", "|");
                    item.m_category.insert(0, "|");
                    item.m_category.append("|");
                }

                Q_EMIT createPlaylistItem(item);
            }
        }
    }
}

void MusicKWQueryPlaylistRequest::morePlaylistDetails(const QString &pid)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << pid);

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_PLAYLIST_INFO_URL, false).arg(pid));
    ReqKWInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downloadMoreDetailsFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}
