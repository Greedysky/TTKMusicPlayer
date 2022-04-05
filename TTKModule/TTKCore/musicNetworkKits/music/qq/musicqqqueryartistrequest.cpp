#include "musicqqqueryartistrequest.h"

#define REFER_URL   "a2tQNGw2RlJyeC9sZjc5RXpsM2hZRXZUeWd0UVJycE1oTzI5MkRQbVRQemN3WUhtOHJiV3FzcS95ZVU9"

MusicQQArtistInfoConfigManager::MusicQQArtistInfoConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicQQArtistInfoConfigManager::readArtistInfoData(MusicResultsItem *item)
{
    const QDomNodeList &resultlist = m_document->elementsByTagName("info");
    for(int i = 0; i < resultlist.count(); ++i)
    {
        const QDomNodeList &infolist = resultlist.at(i).childNodes();
        for(int j = 0; j < infolist.count(); ++j)
        {
            const QDomNode &node = infolist.at(j);
            if(node.nodeName() == "desc")
            {
                item->m_description = node.toElement().text();
            }
            else if(node.nodeName() == "basic")
            {
                const QDomNodeList &basiclist = node.childNodes();
                for(int k = 0; k < basiclist.count(); ++k)
                {
                    const QDomNodeList &itemlist = basiclist.at(k).childNodes();
                    if(itemlist.count() != 2)
                    {
                        continue;
                    }

                    const QString &key = itemlist.at(0).toElement().text();
                    const QString &value = itemlist.at(1).toElement().text();
                    if(key.contains("\u4e2d\u6587\u540d"))
                        item->m_nickName = value;
                    else if(key.contains("\u51fa\u751f\u65e5\u671f"))
                        item->m_updateTime = value;
                    else if(key.contains("\u56fd\u7c4d"))
                        item->m_tags = value;
                }
            }
            else if(node.nodeName() == "other")
            {
                const QDomNodeList &otherlist = node.childNodes();
                for(int k = 0; k < otherlist.count(); ++k)
                {
                    const QDomNodeList &itemlist = otherlist.at(k).childNodes();
                    if(itemlist.count() != 2)
                    {
                        continue;
                    }

                    item->m_description += ("\r\n" + itemlist.at(1).toElement().text());
                }
            }
        }
    }

}



MusicQQQueryArtistRequest::MusicQQQueryArtistRequest(QObject *parent)
    : MusicQueryArtistRequest(parent)
{
    m_queryServer = QUERY_QQ_INTERFACE;
}

void MusicQQQueryArtistRequest::startToSearch(const QString &artist)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), artist));

    deleteAll();
    m_queryText = artist;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_ARTIST_URL, false).arg(artist).arg(0).arg(50));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicQQQueryArtistRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryArtistRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                bool artistFound = false;
                //
                value = value["data"].toMap();
                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    value = value["musicData"].toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    for(const QVariant &var : value["singer"].toList())
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &name = var.toMap();
                        musicInfo.m_singerName = MusicUtils::String::charactersReplaced(name["name"].toString());
                        musicInfo.m_artistId = name["mid"].toString();
                        break; //just find first singer
                    }
                    musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["songname"].toString());
                    musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["interval"].toInt() * 1000);

                    m_rawData["sid"] = value["songid"].toString();
                    musicInfo.m_songId = value["songmid"].toString();
                    musicInfo.m_albumId = value["albummid"].toString();
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(QQ_SONG_LRC_URL, false).arg(musicInfo.m_songId);
                    musicInfo.m_coverUrl = MusicUtils::Algorithm::mdII(QQ_SONG_PIC_URL, false).arg(musicInfo.m_albumId);

                    musicInfo.m_albumName = MusicUtils::String::charactersReplaced(value["albumname"].toString());

                    musicInfo.m_year = QString();
                    musicInfo.m_discNumber = value["cdIdx"].toString();
                    musicInfo.m_trackNumber = value["belongCD"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongProperty(&musicInfo, value, m_queryQuality, m_queryAllRecords);
                    TTK_NETWORK_QUERY_CHECK();

                    if(musicInfo.m_songProps.isEmpty())
                    {
                        continue;
                    }
                    //
                    if(!artistFound)
                    {
                        artistFound = true;
                        MusicResultsItem info;
                        TTK_NETWORK_QUERY_CHECK();
                        downLoadIntro(&info);
                        TTK_NETWORK_QUERY_CHECK();
                        info.m_id = musicInfo.m_artistId;
                        info.m_name = musicInfo.m_singerName;
                        info.m_coverUrl = musicInfo.m_coverUrl;
                        Q_EMIT createArtistInfoItem(info);
                    }
                    //
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName = musicInfo.m_albumName;
                    item.m_duration = musicInfo.m_duration;
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

void MusicQQQueryArtistRequest::downLoadIntro(MusicResultsItem *item) const
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_ARTIST_INFO_URL, false).arg(m_queryText));
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    MusicQQInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    MusicQQArtistInfoConfigManager xml;
    xml.fromByteArray(bytes);
    xml.readArtistInfoData(item);
}
