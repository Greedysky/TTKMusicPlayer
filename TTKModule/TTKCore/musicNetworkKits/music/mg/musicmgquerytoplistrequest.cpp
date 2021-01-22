#include "musicmgquerytoplistrequest.h"
#include "musiccategoryconfigmanager.h"

MusicMGQueryToplistRequest::MusicMGQueryToplistRequest(QObject *parent)
    : MusicQueryToplistRequest(parent)
{
    m_queryServer = QUERY_MG_INTERFACE;
}

void MusicMGQueryToplistRequest::startToSearch(QueryType type, const QString &toplist)
{
    if(type == MusicQuery)
    {
        startToSearch(toplist);
    }
    else
    {
        startToSearch(toplist.isEmpty() ? "jianjiao_newsong" : toplist);
    }
}

void MusicMGQueryToplistRequest::startToSearch(const QString &toplist)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(toplist));

    deleteAll();
    m_rawData["songID"] = toplist;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(MG_TOPLIST_URL, false).arg(toplist));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL, ALG_UA_KEY, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(MG_REFERER_URL, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicMGQueryToplistRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
    setNetworkAbort(false);

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        const QString html(m_reply->readAll());
        QRegExp regx("<script>(.*)var(.*)listData(.*)=(.*)<\\/script>");
        regx.setMinimal(true);

        if(html.indexOf(regx) == -1)
        {
            Q_EMIT downLoadDataChanged(QString());
            deleteAll();
            return;
        }

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(regx.cap(4).toUtf8(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("songs"))
            {
                MusicResultsCategorys categorys;
                MusicCategoryConfigManager manager;
                manager.readConfig(MusicCategoryConfigManager::ToplistResource);
                manager.readCategoryData(categorys, m_queryServer);

                if(categorys.isEmpty())
                {
                    Q_EMIT downLoadDataChanged(QString());
                    deleteAll();
                    return;
                }

                MusicResultsItem info;
                info.m_name = value["name"].toString();
                info.m_coverUrl = STRING_NULL;
                info.m_playCount = STRING_NULL;
                info.m_description = STRING_NULL;
                info.m_updateTime = value["updateTime"].toString();

                for(const MusicResultsCategory &category : qAsConst(categorys))
                {
                    for(const MusicResultsCategoryItem &item : qAsConst(category.m_items))
                    {
                        if(item.m_key == m_rawData["songID"])
                        {
                            info.m_coverUrl = item.m_value;
                            break;
                        }
                    }
                }
                Q_EMIT createToplistInfoItem(info);
                //
                value = value["songs"].toMap();
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
                    musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["name"].toString());
                    musicInfo.m_timeLength = MusicTime::fromString(value["duration"].toString(), MUSIC_STIME_FORMAT).msecTime2LabelJustified();
                    musicInfo.m_songId = value["id"].toString();
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(MG_SONG_LRC_URL, false).arg(value["copyrightId"].toString());

                    const QVariantMap &albumObject = value["al"].toMap();
                    musicInfo.m_smallPicUrl = albumObject["mediumPic"].toString();
                    if(!musicInfo.m_smallPicUrl.contains(TTK_HTTPM))
                    {
                        musicInfo.m_smallPicUrl.insert(0, TTK_HTTPM);
                    }
                    musicInfo.m_albumId = QString::number(albumObject["albumId"].toInt());
                    musicInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(albumObject["albumName"].toString());

                    const QVariantList &artistsArray = value["singers"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistMap = artistValue.toMap();
                        musicInfo.m_artistId = artistMap["id"].toString();
                        musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(artistMap["name"].toString());
                    }

                    musicInfo.m_year = QString();
                    musicInfo.m_discNumber = "1";
                    musicInfo.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongAttributeNew(&musicInfo, value, m_searchQuality, m_queryAllRecords);
                    TTK_NETWORK_QUERY_CHECK();

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    //
                    if(!findUrlFileSize(&musicInfo.m_songAttrs)) return;
                    //
                    setTimeLength(&musicInfo);
                    //
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName = musicInfo.m_albumName;
                    item.m_time = musicInfo.m_timeLength;
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
