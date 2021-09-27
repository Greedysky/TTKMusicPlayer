#include "musicbdquerylearnrequest.h"
#include "musicbdqueryinterface.h"
#include "musicurlutils.h"

#include "qalgorithm/qaeswrapper.h"

MusicBDQueryLearnRequest::MusicBDQueryLearnRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_queryServer = QUERY_BD_INTERFACE;
}

void MusicBDQueryLearnRequest::startToSearch(QueryType type, const QString &text)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));

    Q_UNUSED(type);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(BD_LEARN_URL, false).arg(text).arg(1).arg(30));
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
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

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
                    readFromMusicLrcAttribute(&musicInfo);
                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongAttribute(&musicInfo);
                    TTK_NETWORK_QUERY_CHECK();

                    if(musicInfo.m_songAttrs.isEmpty())
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

void MusicBDQueryLearnRequest::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info)
{
    const QString &key = MusicUtils::Algorithm::mdII(BD_LEARN_DATA_URL, false).arg(info->m_songId).arg(MusicTime::timestamp());
    QString eKey = QString(QAesWrapper().encryptCBC(key.toUtf8(), "4CC20A0C44FEB6FD", "2012061402992850"));
    MusicUtils::Url::urlEncode(eKey);

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(BD_LEARN_INFO_URL, false).arg(key).arg(eKey));
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
            MusicObject::MusicSongAttribute attr;
            attr.m_bitrate = MB_128;
            attr.m_url = value["merge_link"].toString();
            attr.m_format = MP3_FILE_PREFIX;
            //
            if(!findUrlFileSize(&attr)) return;
            //
            info->m_songAttrs.append(attr);
        }
    }
}

void MusicBDQueryLearnRequest::readFromMusicLrcAttribute(MusicObject::MusicSongInformation *info)
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
