#include "musicbdquerylearnrequest.h"
#include "musicbdqueryinterface.h"
#include "musicurlutils.h"

#include "qalgorithm/aeswrapper.h"

MusicBDQueryLearnRequest::MusicBDQueryLearnRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_queryServer = QUERY_BD_INTERFACE;
}

void MusicBDQueryLearnRequest::startToSearch(QueryType type, const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    Q_UNUSED(type);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(BD_LEARN_URL, false).arg(value).arg(1).arg(30));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicBDQueryLearnRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicAbstractQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["error_code"].toInt() == 22000 && value.contains("result"))
            {
                value = value["result"].toMap();
                const QVariantList &datas = value["items"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = MusicUtils::String::charactersReplaced(value["artist_name"].toString());
                    musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["song_title"].toString());
                    musicInfo.m_songId = value["song_id"].toString();
                    musicInfo.m_albumId = value["album_id"].toString();

                    musicInfo.m_year = value["publishtime"].toString();
                    musicInfo.m_discNumber = "1";
                    musicInfo.m_trackNumber = value["album_no"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicLrcProperty(&musicInfo);
                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongProperty(&musicInfo);
                    TTK_NETWORK_QUERY_CHECK();

                    if(musicInfo.m_songProps.isEmpty())
                    {
                        continue;
                    }
                    //
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_type = mapQueryServerString();
                    Q_EMIT createSearchedItem(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicBDQueryLearnRequest::readFromMusicSongProperty(MusicObject::MusicSongInformation *info) const
{
    const QString &key = MusicUtils::Algorithm::mdII(BD_LEARN_DATA_URL, false).arg(info->m_songId).arg(MusicTime::timestamp());
    QString eKey = QString(QAlgorithm::Aes().encryptCBC(key.toUtf8(), "4CC20A0C44FEB6FD", "2012061402992850"));
    MusicUtils::Url::urlEncode(eKey);

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(BD_LEARN_INFO_URL, false).arg(key, eKey));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

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
        if(value["error_code"].toInt() == 22000 && value.contains("result"))
        {
            value = value["result"].toMap();
            MusicObject::MusicSongProperty prop;
            prop.m_bitrate = MB_128;
            prop.m_url = value["merge_link"].toString();
            prop.m_format = MP3_FILE_PREFIX;
            //
            if(!findUrlFileSize(&prop)) return;
            //
            info->m_songProps.append(prop);
        }
    }
}

void MusicBDQueryLearnRequest::readFromMusicLrcProperty(MusicObject::MusicSongInformation *info) const
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(BD_SONG_PATH_URL, false).arg(info->m_songId));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

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
        if(value["errorCode"].toInt() == 22000 && value.contains("data"))
        {
            value = value["data"].toMap();
            const QVariantList &datas = value["songList"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                info->m_lrcUrl = value["lrcLink"].toString();
            }
        }
    }
}
