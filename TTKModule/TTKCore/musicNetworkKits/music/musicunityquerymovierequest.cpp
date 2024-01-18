#include "musicunityquerymovierequest.h"

namespace YYTInterface
{
    static constexpr const char *MODULE = "YYT";
    static const QString MOVIE_URL      = "aHFyKzJ2V2gvdWVrdzEyWlBmUFdHbys5RGM3STRrY25uQ2xCRDdXeldPV0pXMWtNVmN1di9CRndFeDNqbExEdjA5dC9qcWEzMWFxc3NnRWkrcFcrZlBReC90bz0=";
    static const QString MOVIE_DATA_URL = "emxYODBYb2dkQUVzcFcxVFZJS29kbzIwMzhybDVJRk1aNDFab1l3ellaM2pEdEFTVHFvN3RRV2ZNOXM4QkFkVXcrNWNzamFzc2ducEs2NXVyU1RmaVM2VlJtSmlSVUt5MUJmSE9WS2lxc3A5bVh0TXNBRzBEeWd2R29yNVZkUzJLbVBVbGdQSFZOemdQUGkxWFN5TllweHdDNnA3ZStUclVIUE1zYmpwNkVaSC96RHY5eXdTVWdoTVhsK2tidGhmOGcvcFpLejA5NlVLaUlxZko5Ni9ucEJRSUJyZVJGUW5YQlpLQzBGdm44SklDVDh5T0xmTWZoRWpoU3pWTFRtby9mYWQ0QlpJNjk4Q3dzczA=";
}

namespace BLInterface
{
    static constexpr const char *MODULE = "BL";
    static const QString BUVID_URL      = "NmozU2YzaW5JbFE0QnRHVHJheWFaWk5JbjE1QWdWdFpTWFU4aFoyZEt1RU5FdVZ0T1JsV3pscUlkbFdMYVVjT1ArYStiOG9yR1RZMFZ0UVY=";
    static const QString MOVIE_URL      = "ZTRrNzB2YnIzbTVvRzZUdDZVeGllN09MM3J6TjhaVGplNUZUOHBFWnBtZ0FFWEh2MUN2STBWb2ZJUzlXRE5HcHo4RlhMQWZjRitha04rczNIcnlOVXpxcWVRQlp6YzBsNE9pVExuNWRDNS9zSnNKSjlyN0pOQUs2S3JOQjVkWmo=";
    static const QString MOVIE_DATA_URL = "ak5VTmcwemNhNXNpU1NCcXFTb28rWmo5TEdmMmtHNkh1ZUdHWWEveHFCWG80MnRyVkpHWHVNQ2VIN0dhUDJvOGRGVmdDei9paTFNPQ==";
    static const QString MOVIE_PLAY_URL = "a0Y2MzNmVEFOd2JHN0xuaDBjVmJEYndRNkRmb2w1YUs3K0ZoNExpYXVYR3BjaEtSemgwdXVmem5GSDhpS29Cc0RDMStIR1RuWmF5RENKYWVxcmhCRXI0dE5JdGgwVTh5YW9YWVMvK1B3bWdNZFBMag==";

    /*!
     * Make request query data.
     */
    static void makeRequestRawHeader(QNetworkRequest *request);

    /*!
     * Read mv cid from query results.
     */
    static void parseFromMovieInfo(TTK::MusicSongInformation *info, QString &cid);
    /*!
     * Read mv tags(size\bitrate\url) from query results.
     */
    static void parseFromMovieProperty(TTK::MusicSongInformation *info, const QString &cid);

}

void BLInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    TTK::setSslConfiguration(request);
    TTK::makeContentTypeHeader(request);
    request->setRawHeader("Cookie", TTK::Algorithm::mdII(BLInterface::BUVID_URL, false).toUtf8());
}

void BLInterface::parseFromMovieInfo(TTK::MusicSongInformation *info, QString &cid)
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(BLInterface::MOVIE_DATA_URL, false).arg(info->m_songId));
    BLInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
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
        if(value["code"].toInt() == 0 && value.contains("data"))
        {
            const QVariantList &datas = value["data"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();

                cid = value["cid"].toString();
                break;
            }
        }
    }
}

void BLInterface::parseFromMovieProperty(TTK::MusicSongInformation *info, const QString &cid)
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(BLInterface::MOVIE_PLAY_URL, false).arg(info->m_songId, cid).arg(16));
    BLInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
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
        if(value["code"].toInt() == 0 && value.contains("data"))
        {
            value = value["data"].toMap();

            const QVariantList &datas = value["durl"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();

                TTK::MusicSongProperty prop;
                prop.m_url = value["url"].toString();
                prop.m_bitrate = TTK_BN_250;
                prop.m_format = MP4_FILE_SUFFIX;
                prop.m_size = TTK::Number::sizeByteToLabel(value["size"].toInt());
                info->m_songProps.append(prop);

                info->m_duration = TTKTime::formatDuration(value["length"].toInt());
                break;
            }
        }
    }
}



MusicUnityQueryMovieRequest::MusicUnityQueryMovieRequest(QObject *parent)
    : MusicQueryMovieRequest(parent),
      m_isUnity(false),
      m_pluginIndex(0),
      m_value("0")
{
    m_pageSize = SONG_PAGE_SIZE;
}

void MusicUnityQueryMovieRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset - 1;

    switch(m_pluginIndex)
    {
        case 0:
        {
            TTK_INFO_STREAM(className() << "YYTInterface");
            QNetworkRequest request;
            request.setUrl(TTK::Algorithm::mdII(YYTInterface::MOVIE_URL, false));
            TTK::setSslConfiguration(&request);
            TTK::makeContentTypeHeader(&request, "application/json");

            m_queryServer = YYTInterface::MODULE;
            m_reply = m_manager.post(request, TTK::Algorithm::mdII(YYTInterface::MOVIE_DATA_URL, false).arg(m_queryValue, m_value).arg(m_pageSize).toUtf8());
            break;
        }
        case 1:
        {
            TTK_INFO_STREAM(className() << "BLInterface");
            QNetworkRequest request;
            request.setUrl(TTK::Algorithm::mdII(BLInterface::MOVIE_URL, false).arg(m_pageSize).arg(m_pageIndex + 1));
            BLInterface::makeRequestRawHeader(&request);

            m_queryServer = BLInterface::MODULE;
            m_reply = m_manager.get(request);
            break;
        }
        default: return;
    }

    connect(m_reply, SIGNAL(finished()), SLOT(downLoadUnityFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicUnityQueryMovieRequest::downLoadUnityFinished()
{
    TTK_INFO_STREAM(className() << "downLoadUnityFinished");

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            switch(m_pluginIndex)
            {
                case 0:
                {
                    const int lastCount = m_songInfos.count();
                    if(value["code"].toInt() == 0 && value.contains("data"))
                    {
                        const QVariantList &datas = value["data"].toList();
                        for(const QVariant &var : qAsConst(datas))
                        {
                            if(var.isNull())
                            {
                                continue;
                            }

                            value = var.toMap();
                            TTK_NETWORK_QUERY_CHECK();

                            m_totalSize = TTK_HIGH_LEVEL * TTK_HIGH_LEVEL;

                            TTK::MusicSongInformation info;
                            info.m_songId = value["id"].toString();
                            info.m_songName = TTK::String::charactersReplace(value["title"].toString());

                            const QVariantList &artistsArray = value["artists"].toList();
                            for(const QVariant &artistValue : qAsConst(artistsArray))
                            {
                                if(artistValue.isNull())
                                {
                                    continue;
                                }

                                const QVariantMap &artistObject = artistValue.toMap();
                                info.m_artistName = TTK::String::charactersReplace(artistObject["name"].toString());
                                break; //just find first artist
                            }

                            value = value["fullClip"].toMap();
                            info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);

                            const QVariantList &urlsArray = value["urls"].toList();
                            for(const QVariant &urlValue : qAsConst(urlsArray))
                            {
                                if(urlValue.isNull())
                                {
                                    continue;
                                }

                                value = urlValue.toMap();

                                TTK::MusicSongProperty prop;
                                prop.m_url = value["url"].toString();
                                prop.m_bitrate = value["streamType"].toInt() == 1 ? TTK_BN_250 : TTK_BN_750;
                                prop.m_format = TTK::String::splitToken(prop.m_url);
                                info.m_songProps.append(prop);
                            }

                            if(info.m_songProps.isEmpty() || !findUrlPathSize(&info.m_songProps, info.m_duration))
                            {
                                continue;
                            }

                            m_value = info.m_songId;
                            Q_EMIT createResultItem({info, serverToString()});
                            m_songInfos << info;
                        }
                    }

                    if(lastCount == m_songInfos.count())
                    {
                        m_totalSize = 0;
                    }
//                    if(lastCount + m_pageSize > m_songInfos.count())
//                    {
//                        setToUnity();
//                        ++m_pluginIndex;
//                    }
                    break;
                }
                case 1:
                {
                    if(value["code"].toInt() == 0 && value.contains("data"))
                    {
                        value = value["data"].toMap();
                        m_totalSize = value["numResults"].toInt();

                        const QVariantList &datas = value["result"].toList();
                        for(const QVariant &var : qAsConst(datas))
                        {
                            if(var.isNull())
                            {
                                continue;
                            }

                            value = var.toMap();
                            TTK_NETWORK_QUERY_CHECK();

                            TTK::MusicSongInformation info;
                            info.m_songId = value["bvid"].toString();
                            info.m_songName = TTK::String::charactersReplace(value["title"].toString());

                            info.m_artistName = TTK::String::charactersReplace(value["author"].toString());

                            QString cid;
                            BLInterface::parseFromMovieInfo(&info, cid);

                            if(!cid.isEmpty())
                            {
                                BLInterface::parseFromMovieProperty(&info, cid);
                            }

                            if(info.m_songProps.isEmpty())
                            {
                                continue;
                            }

                            Q_EMIT createResultItem({info, serverToString()});
                            m_songInfos << info;
                        }
                    }
                    break;
                }
                default: break;
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
