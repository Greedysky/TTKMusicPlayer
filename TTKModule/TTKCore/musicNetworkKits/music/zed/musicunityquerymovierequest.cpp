#include "musicunityquerymovierequest.h"
#include "musicblqueryinterface.h"

/*! @brief The namespace of the yyt request interface.
 * @author Greedysky <greedysky@163.com>
 */
namespace ReqYYTInterface
{
    static constexpr const char *MODULE = "YYT";
    static constexpr const char *MOVIE_URL = "aHFyKzJ2V2gvdWVrdzEyWlBmUFdHbys5RGM3STRrY25uQ2xCRDdXeldPV0pXMWtNVmN1di9CRndFeDNqbExEdjA5dC9qcWEzMWFxc3NnRWkrcFcrZlBReC90bz0=";
    static constexpr const char *MOVIE_DATA_URL = "emxYODBYb2dkQUVzcFcxVFZJS29kbzIwMzhybDVJRk1aNDFab1l3ellaM2pEdEFTVHFvN3RRV2ZNOXM4QkFkVXcrNWNzamFzc2ducEs2NXVyU1RmaVM2VlJtSmlSVUt5MUJmSE9WS2lxc3A5bVh0TXNBRzBEeWd2R29yNVZkUzJLbVBVbGdQSFZOemdQUGkxWFN5TllweHdDNnA3ZStUclVIUE1zYmpwNkVaSC96RHY5eXdTVWdoTVhsK2tidGhmOGcvcFpLejA5NlVLaUlxZko5Ni9ucEJRSUJyZVJGUW5YQlpLQzBGdm44SklDVDh5T0xmTWZoRWpoU3pWTFRtby9mYWQ0QlpJNjk4Q3dzczA=";
}

/*! @brief The namespace of the bl request interface.
 * @author Greedysky <greedysky@163.com>
 */
namespace ReqBLInterface
{
    static constexpr const char *MODULE = "BL";
}



MusicUnityQueryMovieRequest::MusicUnityQueryMovieRequest(QObject *parent)
    : MusicQueryMovieRequest(parent),
      m_isUnity(false),
      m_pluginSelector(0),
      m_value("0")
{
    m_pageSize = TTK_PAGE_SIZE_30;
}

void MusicUnityQueryMovieRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    switch(m_pluginSelector)
    {
        case 0:
        {
            QNetworkRequest request;
            request.setUrl(TTK::Algorithm::mdII(ReqYYTInterface::MOVIE_URL, false));
            TTK::setSslConfiguration(&request);
            TTK::setContentTypeHeader(&request, "application/json");

            TTK_INFO_STREAM("parse song property in yyt module");

            m_queryServer = ReqYYTInterface::MODULE;
            m_reply = m_manager.post(request, TTK::Algorithm::mdII(ReqYYTInterface::MOVIE_DATA_URL, false).arg(m_queryValue, m_value).arg(m_pageSize).toUtf8());
            break;
        }
        case 1:
        {
            QNetworkRequest request;
            request.setUrl(TTK::Algorithm::mdII(BL_MOVIE_URL, false).arg(m_queryValue).arg(offset));
            ReqBLInterface::makeRequestRawHeader(&request);

            TTK_INFO_STREAM("parse song property in bl module");

            m_queryServer = ReqBLInterface::MODULE;
            m_reply = m_manager.get(request);
            break;
        }
        default: return;
    }

    connect(m_reply, SIGNAL(finished()), SLOT(downloadUnityFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicUnityQueryMovieRequest::downloadUnityFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicPageQueryRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            const int lastCount = m_items.count();

            switch(m_pluginSelector)
            {
                case 0:
                {
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
                                const QString &artistName = TTK::String::charactersReplace(artistObject["name"].toString());
                                info.m_artistName = info.m_artistName.isEmpty() ? artistName : (info.m_artistName + ";" + artistName);
                            }

                            value = value["fullClip"].toMap();
                            info.m_coverUrl = value["cover"].toString();
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

                            if(info.m_songProps.isEmpty() || !fetchUrlPathSize(&info.m_songProps, info.m_duration))
                            {
                                continue;
                            }

                            m_value = info.m_songId;
                            Q_EMIT createResultItem({info, serverToString()});
                            m_items << info;
                        }
                    }

                    findAllPlugins(lastCount);
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

                            info.m_coverUrl = value["pic"].toString();
                            if(info.m_coverUrl.startsWith("//"))
                            {
                                info.m_coverUrl = TTK_STR_CAT(HTTP_PROTOCOL_PREFIX, ":") + info.m_coverUrl;
                            }
                            else if(!TTK::String::isNetworkUrl(info.m_coverUrl))
                            {
                                info.m_coverUrl = HTTP_PROTOCOL + info.m_coverUrl;
                            }

                            QString cid;
                            ReqBLInterface::parseFromMovieInfo(&info, cid);

                            if(!cid.isEmpty())
                            {
                                ReqBLInterface::parseFromMovieProperty(&info, cid);
                            }

                            if(info.m_songProps.isEmpty())
                            {
                                continue;
                            }

                            Q_EMIT createResultItem({info, serverToString()});
                            m_items << info;
                        }
                    }
                    break;
                }
                default: break;
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}

void MusicUnityQueryMovieRequest::findAllPlugins(int count)
{
    if(count + m_pageSize > m_items.count())
    {
        setToUnity();
        ++m_pluginSelector;
    }
}
