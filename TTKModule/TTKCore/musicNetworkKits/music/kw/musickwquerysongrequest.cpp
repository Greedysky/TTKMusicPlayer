#include "musickwquerysongrequest.h"

static constexpr const char *KG_NEW_SONG_URL = "SU5LT3ZXSExhQ2FEWWpFRmJxa1k1cjB6WktRc2IrNENiclo3KzVqUlBaNVRORzVoNHhlU2VheVA4VFhlaGxWaVg3SFJLQ0FmNWQ0dFdIQVVUTUxIUmU4RkJyTDRxSC9DL3kvZ0crZmZYWTlOR21WRlU0WFZaa1BFN3NGcVlIcXlXamUyWmg0UjlnT2hLVVU2bzdTb0QxaGQ3QmhkNWJXMStTTlFJVmFQK1c5TDZnclU=";

MusicKWQuerySongRequest::MusicKWQuerySongRequest(QObject *parent)
    : MusicQuerySongRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQuerySongRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SEARCH_URL, false).arg(m_queryValue).arg(offset).arg(m_pageSize));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQuerySongRequest::startToSearchByID(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_INFO_URL, false).arg(value));
    ReqKWInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downloadSingleFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicKWQuerySongRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqKWInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQuerySongRequest::startToQueryResult(info, bitrate);
}

void MusicKWQuerySongRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicPageQueryRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll().replace("'", "\""), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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
                    info.m_year.clear();
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

    Q_EMIT downloadDataChanged({});
    deleteAll();
}

void MusicKWQuerySongRequest::downloadSingleFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicQuerySongRequest::downloadFinished();
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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
                info.m_year = value["releaseDate"].toString().section(TTK_DEFAULT_STR, 0, 0);
                info.m_trackNumber = value["track"].toString();

                TTK_NETWORK_QUERY_CHECK();
                ReqKWInterface::parseFromSongProperty(&info, "MP3128|MP3192|MP3H");
                TTK_NETWORK_QUERY_CHECK();

                Q_EMIT createResultItem({info, serverToString()});
                m_items << info;
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicKWQueryNewSongRequest::MusicKWQueryNewSongRequest(QObject *parent)
    : MusicQuerySongRequest(parent)
{
    m_queryServer = QUERY_KW_INTERFACE;
    m_pageSize = 1;
    m_totalSize = 13;
}

void MusicKWQueryNewSongRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_NEW_SONG_URL, false).arg(offset).arg(m_totalSize));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryNewSongRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqKWInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicAbstractQueryRequest::startToQueryResult(info, bitrate);
}

void MusicKWQueryNewSongRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractQueryRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
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

                    if(!pageValid())
                    {
                        break;
                    }

                    ++m_pageIndex;

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songId = value["id"].toString();
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());

                    info.m_artistId = value["artistid"].toString();
                    info.m_artistName = ReqKWInterface::makeSongArtist(value["artist"].toString());

                    info.m_albumId = value["albumid"].toString();
                    info.m_albumName = TTK::String::charactersReplace(value["album"].toString());

                    info.m_coverUrl = value["albumpic"].toString().replace("/120/", "/500/");
                    info.m_lrcUrl = TTK::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);
                    info.m_year = value["firstrecordtime"].toString().section(TTK_DEFAULT_STR, 0, 0);
                    info.m_trackNumber = "0";

                    if(m_queryMode != QueryMode::Meta)
                    {
                        TTK_NETWORK_QUERY_CHECK();
                        ReqKWInterface::parseFromSongProperty(&info, value["formats"].toString());
                        TTK_NETWORK_QUERY_CHECK();

                        Q_EMIT createResultItem({info, serverToString()});
                    }

                    m_items << info;
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
