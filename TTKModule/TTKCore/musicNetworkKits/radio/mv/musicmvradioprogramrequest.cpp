#include "musicmvradioprogramrequest.h"
#include "musickgqueryinterface.h"

MusicMVRadioProgramRequest::MusicMVRadioProgramRequest(QObject *parent)
    : MusicAbstractMVRadioRequest(parent)
{

}

void MusicMVRadioProgramRequest::downLoadFinished()
{
    MusicAbstractMVRadioRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
        bytes = QString(bytes).split("var mvfmdata = ").back().split("$img = ").front().toUtf8();
        bytes.chop(3);

        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(bytes, &ok);
        if(ok)
        {
            bool contains = false;
            for(const QVariant &var : data.toList())
            {
                if(var.isNull())
                {
                    continue;
                }

                QVariantMap value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                MusicResultDataItem result;
                result.m_nickName = value["className"].toString();

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
                        result.m_name = value["fmName"].toString();
                        result.m_id = value["fmId"].toString();
                        result.m_coverUrl = value["imgUrlMv"].toString();
                        Q_EMIT createMVRadioItem(result);

                        for(const QVariant &var : value["mvs"].toList())
                        {
                            if(var.isNull())
                            {
                                continue;
                            }

                            value = var.toMap();
                            TTK_NETWORK_QUERY_CHECK();

                            TTK::MusicSongInformation info;
                            info.m_singerName = TTK::String::charactersReplace(value["name"].toString());
                            info.m_songName = TTK::String::charactersReplace(value["name"].toString());
                            if(info.m_singerName.contains(" - "))
                            {
                                const QStringList &ds = info.m_singerName.split(" - ");
                                if(ds.count() >= 2)
                                {
                                    info.m_singerName = ds.front();
                                    info.m_songName = ds.back();
                                }
                            }
                            info.m_duration = TTKTime::formatDuration(value["time"].toInt());

                            info.m_songId = value["mvhash"].toString();
                            TTK_NETWORK_QUERY_CHECK();
                            parseFromMovieProperty(&info);
                            TTK_NETWORK_QUERY_CHECK();

                            if(info.m_songProps.isEmpty())
                            {
                                continue;
                            }

                            MusicResultInfoItem item;
                            item.m_songName = info.m_songName;
                            item.m_singerName = info.m_singerName;
                            item.m_duration = info.m_duration;
                            item.m_albumName.clear();
                            item.m_type.clear();
                            Q_EMIT createSearchedItem(item);
                            m_songInfos << info;
                        }
                    }
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicMVRadioProgramRequest::parseFromMovieProperty(TTK::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    const QByteArray &encodedData = TTK::Algorithm::md5(QString("%1kugoumvcloud").arg(info->m_songId).toUtf8());

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_MOVIE_INFO_URL, false).arg(encodedData.constData(), info->m_songId));
    MusicKGInterface::makeRequestRawHeader(&request);

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
        if(!value.isEmpty() && value.contains("mvdata"))
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

void MusicMVRadioProgramRequest::parseFromMovieProperty(TTK::MusicSongInformation *info, const QVariantMap &key)
{
    TTK::MusicSongProperty prop;
    prop.m_url = key["downurl"].toString();
    prop.m_size = TTK::Number::sizeByteToLabel(key["filesize"].toInt());
    prop.m_format = TTK::String::slitToken(prop.m_url);

    const int bitrate = key["bitrate"].toInt() / 1000;
    if(bitrate <= 375)
    {
        prop.m_bitrate = MB_250;
    }
    else if(bitrate > 375 && bitrate <= 625)
    {
        prop.m_bitrate = MB_500;
    }
    else if(bitrate > 625 && bitrate <= 875)
    {
        prop.m_bitrate = MB_750;
    }
    else if(bitrate > 875)
    {
        prop.m_bitrate = MB_1000;
    }

    info->m_songProps.append(prop);
}
