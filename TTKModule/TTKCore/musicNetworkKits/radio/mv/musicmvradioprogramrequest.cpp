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
        bool ok;
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

                MusicResultsItem item;
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

                        Q_EMIT createCategoryItem(item);

                        for(const QVariant &var : value["mvs"].toList())
                        {
                            if(var.isNull())
                            {
                                continue;
                            }

                            value = var.toMap();
                            TTK_NETWORK_QUERY_CHECK();

                            MusicObject::MusicSongInformation musicInfo;
                            musicInfo.m_singerName = MusicUtils::String::charactersReplaced(value["name"].toString());
                            musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["name"].toString());
                            if(musicInfo.m_singerName.contains(" - "))
                            {
                                const QStringList &ds = musicInfo.m_singerName.split(" - ");
                                if(ds.count() >= 2)
                                {
                                    musicInfo.m_singerName = ds.front();
                                    musicInfo.m_songName = ds.back();
                                }
                            }
                            musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["time"].toInt());

                            musicInfo.m_songId = value["mvhash"].toString();
                            TTK_NETWORK_QUERY_CHECK();
                            readFromMusicMVProperty(&musicInfo);
                            TTK_NETWORK_QUERY_CHECK();

                            if(musicInfo.m_songProps.isEmpty())
                            {
                                continue;
                            }
                            //
                            MusicSearchedItem item;
                            item.m_songName = musicInfo.m_songName;
                            item.m_singerName = musicInfo.m_singerName;
                            item.m_duration = musicInfo.m_duration;
                            item.m_albumName.clear();
                            item.m_type.clear();
                            Q_EMIT createSearchedItem(item);
                            m_musicSongInfos << musicInfo;
                        }
                    }
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicMVRadioProgramRequest::readFromMusicMVProperty(MusicObject::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    const QByteArray &encodedData = MusicUtils::Algorithm::md5(QString("%1kugoumvcloud").arg(info->m_songId).toUtf8());

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_MOVIE_INFO_URL, false).arg(encodedData.constData(), info->m_songId));
    MusicKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok;
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
                readFromMusicMVProperty(info, mv);
            }
            mv = value["hd"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVProperty(info, mv);
            }
            mv = value["sq"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVProperty(info, mv);
            }
            mv = value["rq"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVProperty(info, mv);
            }
        }
    }
}

void MusicMVRadioProgramRequest::readFromMusicMVProperty(MusicObject::MusicSongInformation *info, const QVariantMap &key)
{
    MusicObject::MusicSongProperty prop;
    prop.m_url = key["downurl"].toString();
    prop.m_size = MusicUtils::Number::sizeByte2Label(key["filesize"].toInt());
    prop.m_format = MusicUtils::String::stringSplitToken(prop.m_url);

    const int bitrate = key["bitrate"].toInt() / 1000;
    if(bitrate <= 375)
        prop.m_bitrate = MB_250;
    else if(bitrate > 375 && bitrate <= 625)
        prop.m_bitrate = MB_500;
    else if(bitrate > 625 && bitrate <= 875)
        prop.m_bitrate = MB_750;
    else if(bitrate > 875)
        prop.m_bitrate = MB_1000;

    info->m_songProps.append(prop);
}
