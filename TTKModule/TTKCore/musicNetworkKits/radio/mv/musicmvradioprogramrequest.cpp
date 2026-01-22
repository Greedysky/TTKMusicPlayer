#include "musicmvradioprogramrequest.h"
#include "musickgqueryinterface.h"

namespace ReqKGInterface
{
    /*!
     * Read mv tags(size and bitrate and url) from query results.
     */
    static void parseFromMovieProperty(TTK::MusicSongInformation *info);
    /*!
     * Read mv tags(size and bitrate and url) from query results.
     */
    static void parseFromMovieProperty(TTK::MusicSongInformation *info, const QVariantMap &key);

}

void ReqKGInterface::parseFromMovieProperty(TTK::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    const QByteArray &key = TTK::Algorithm::md5(QString("%1kugoumvcloud").arg(info->m_songId).toUtf8());

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_MOVIE_INFO_URL, false).arg(key.constData(), info->m_songId));
    ReqKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value.contains("mvdata"))
        {
            value = value["mvdata"].toMap();

            QVariantMap mv = value["sd"].toMap();
            if(!mv.isEmpty())
            {
                parseFromMovieProperty(info, mv);
            }

            mv = value["hd"].toMap();
            if(!mv.isEmpty())
            {
                parseFromMovieProperty(info, mv);
            }

            mv = value["sq"].toMap();
            if(!mv.isEmpty())
            {
                parseFromMovieProperty(info, mv);
            }

            mv = value["rq"].toMap();
            if(!mv.isEmpty())
            {
                parseFromMovieProperty(info, mv);
            }
        }
    }
}

void ReqKGInterface::parseFromMovieProperty(TTK::MusicSongInformation *info, const QVariantMap &key)
{
    TTK::MusicSongProperty prop;
    prop.m_url = key["downurl"].toString();
    prop.m_size = TTK::Number::sizeByteToLabel(key["filesize"].toInt());
    prop.m_format = TTK::String::splitToken(prop.m_url);

    const int bitrate = key["bitrate"].toInt() / 1000;
    if(bitrate <= 1000)
    {
        prop.m_bitrate = TTK_BN_250;
    }
    else if(bitrate > 1000 && bitrate <= 2000)
    {
        prop.m_bitrate = TTK_BN_500;
    }
    else if(bitrate > 2000 && bitrate <= 3000)
    {
        prop.m_bitrate = TTK_BN_750;
    }
    else if(bitrate > 3000)
    {
        prop.m_bitrate = TTK_BN_1000;
    }

    info->m_songProps.append(prop);
}


MusicMVRadioProgramRequest::MusicMVRadioProgramRequest(QObject *parent)
    : MusicAbstractMVRadioRequest(parent)
{

}

void MusicMVRadioProgramRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractMVRadioRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("data"))
            {
                bool contains = false;
                for(const QVariant &var : value["data"].toList())
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    QVariantMap value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem item;
                    item.m_nickName = value["className"].toString();

                    for(const QVariant &var : value["fm_list"].toList())
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        value = var.toMap();
                        TTK_NETWORK_QUERY_CHECK();

                        if(!contains && value["fmId"].toString() == m_queryValue)
                        {
                            contains = true;
                            item.m_name = value["fmName"].toString();
                            item.m_id = value["fmId"].toString();
                            item.m_coverUrl = value["imgUrlMv"].toString();
                            Q_EMIT createMVRadioItem(item);

                            for(const QVariant &var : value["mvs"].toList())
                            {
                                if(var.isNull())
                                {
                                    continue;
                                }

                                value = var.toMap();
                                TTK_NETWORK_QUERY_CHECK();

                                TTK::MusicSongInformation info;
                                info.m_songId = value["mvhash"].toString();
                                info.m_songName = TTK::String::charactersReplace(value["name"].toString());

                                info.m_artistName = info.m_songName;
                                if(info.m_artistName.contains(TTK_DEFAULT_STR))
                                {
                                    const QStringList &ds = info.m_artistName.split(TTK_DEFAULT_STR);
                                    if(ds.count() >= 2)
                                    {
                                        info.m_artistName = ds.first().trimmed();
                                        info.m_songName = ds.last().trimmed();
                                    }
                                }

                                info.m_duration = TTKTime::formatDuration(value["time"].toInt());

                                TTK_NETWORK_QUERY_CHECK();
                                ReqKGInterface::parseFromMovieProperty(&info);
                                TTK_NETWORK_QUERY_CHECK();

                                if(info.m_songProps.isEmpty())
                                {
                                    continue;
                                }

                                Q_EMIT createResultItem({info});
                                m_items << info;
                            }
                        }
                    }
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
