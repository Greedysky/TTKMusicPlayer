#include "musicqqquerytoplistrequest.h"

MusicQQQueryToplistRequest::MusicQQQueryToplistRequest(QObject *parent)
    : MusicQueryToplistRequest(parent)
{
    m_queryServer = QUERY_QQ_INTERFACE;
}

void MusicQQQueryToplistRequest::startToSearch(QueryType type, const QString &value)
{
    if(type == QueryType::Music)
    {
        startToSearch(value);
    }
    else
    {
        startToSearch(value.isEmpty() ? "4" : value);
    }
}

void MusicQQQueryToplistRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch").arg(className()));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_TOPLIST_URL, false).arg(value));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicQQQueryToplistRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryToplistRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0 && value.contains("songlist"))
            {
                const QVariantMap &topInfo = value["topinfo"].toMap();
                MusicResultsItem result;
                result.m_name = topInfo["ListName"].toString();
                result.m_coverUrl = topInfo["pic"].toString();
                result.m_playCount = QString::number(topInfo["listennum"].toULongLong());
                result.m_description = topInfo["info"].toString();
                result.m_updateTime = value["date"].toString();
                Q_EMIT createToplistInfoItem(result);

                const QVariantList &datas = value["songlist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    value = value["data"].toMap();
                    MusicObject::MusicSongInformation info;
                    for(const QVariant &var : value["singer"].toList())
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &name = var.toMap();
                        info.m_singerName = MusicUtils::String::charactersReplaced(name["name"].toString());
                        info.m_artistId = name["mid"].toString();
                        break; //just find first singer
                    }
                    info.m_songName = MusicUtils::String::charactersReplaced(value["songname"].toString());
                    info.m_duration = MusicTime::msecTime2LabelJustified(value["interval"].toInt() * 1000);

                    m_rawData["sid"] = value["songid"].toString();
                    info.m_songId = value["songmid"].toString();
                    info.m_albumId = value["albummid"].toString();
                    info.m_albumName = MusicUtils::String::charactersReplaced(value["albumname"].toString());

                    info.m_lrcUrl = MusicUtils::Algorithm::mdII(QQ_SONG_LRC_URL, false).arg(info.m_songId);
                    info.m_coverUrl = MusicUtils::Algorithm::mdII(QQ_SONG_PIC_URL, false).arg(info.m_albumId);

                    info.m_year = QString();
                    info.m_discNumber = value["cdIdx"].toString();
                    info.m_trackNumber = value["belongCD"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongProperty(&info, value, m_queryQuality, m_queryAllRecords);
                    TTK_NETWORK_QUERY_CHECK();

                    if(info.m_songProps.isEmpty())
                    {
                        continue;
                    }

                    MusicSearchedItem item;
                    item.m_songName = info.m_songName;
                    item.m_singerName = info.m_singerName;
                    item.m_albumName = info.m_albumName;
                    item.m_duration = info.m_duration;
                    item.m_type = mapQueryServerString();
                    Q_EMIT createSearchedItem(item);
                    m_songInfos << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
