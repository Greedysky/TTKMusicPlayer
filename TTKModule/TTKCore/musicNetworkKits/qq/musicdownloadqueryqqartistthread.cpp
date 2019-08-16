#include "musicdownloadqueryqqartistthread.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

#define REFER_URL   "a2tQNGw2RlJyeC9sZjc5RXpsM2hZRXZUeWd0UVJycE1oTzI5MkRQbVRQemN3WUhtOHJiV3FzcS95ZVU9"

MusicQQArtistInfoConfigManager::MusicQQArtistInfoConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicQQArtistInfoConfigManager::readArtistInfoData(MusicResultsItem *item)
{
    const QDomNodeList &resultlist = m_document->elementsByTagName("info");
    for(int i=0; i<resultlist.count(); ++i)
    {
        const QDomNodeList &infolist = resultlist.at(i).childNodes();
        for(int j=0; j<infolist.count(); ++j)
        {
            const QDomNode &node = infolist.at(j);
            if(node.nodeName() == "desc")
            {
                item->m_description = node.toElement().text();
            }
            else if(node.nodeName() == "basic")
            {
                const QDomNodeList &basiclist = node.childNodes();
                for(int k=0; k<basiclist.count(); ++k)
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
                for(int k=0; k<otherlist.count(); ++k)
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



MusicDownLoadQueryQQArtistThread::MusicDownLoadQueryQQArtistThread(QObject *parent)
    : MusicDownLoadQueryArtistThread(parent)
{
    m_queryServer = QUERY_QQ_INTERFACE;
}

void MusicDownLoadQueryQQArtistThread::startToSearch(const QString &artist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(artist));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(QQ_ARTIST_URL, false).arg(artist).arg(0).arg(50);
    m_searchText = artist;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryQQArtistThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                bool artistFlag = false;
                //
                value = value["data"].toMap();
                const QVariantList &datas = value["list"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    value = value["musicData"].toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    foreach(const QVariant &var, value["singer"].toList())
                    {
                        if(var.isNull())
                        {
                            continue;
                        }
                        const QVariantMap &name = var.toMap();
                        musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(name["name"].toString());
                        musicInfo.m_artistId = name["mid"].toString();
                    }
                    musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["songname"].toString());
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["interval"].toInt()*1000);

                    m_rawData["songID"] = value["songid"].toString();
                    musicInfo.m_songId = value["songmid"].toString();
                    musicInfo.m_albumId = value["albummid"].toString();
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(QQ_SONG_LRC_URL, false).arg(musicInfo.m_songId);
                    musicInfo.m_smallPicUrl = MusicUtils::Algorithm::mdII(QQ_SONG_PIC_URL, false)
                                              .arg(musicInfo.m_albumId.right(2).left(1))
                                              .arg(musicInfo.m_albumId.right(1)).arg(musicInfo.m_albumId);
                    musicInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["albumname"].toString());

                    musicInfo.m_year = QString();
                    musicInfo.m_discNumber = value["cdIdx"].toString();
                    musicInfo.m_trackNumber = value["belongCD"].toString();

                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                    readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);
                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    //
                    if(!artistFlag)
                    {
                        artistFlag = true;
                        MusicResultsItem info;
                        if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                        getDownLoadIntro(&info);
                        if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                        info.m_id = musicInfo.m_artistId;
                        info.m_name = musicInfo.m_singerName;
                        info.m_coverUrl = musicInfo.m_smallPicUrl;
                        emit createArtistInfoItem(info);
                    }
                    //
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName = musicInfo.m_albumName;
                    item.m_time = musicInfo.m_timeLength;
                    item.m_type = mapQueryServerString();
                    emit createSearchedItem(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryQQArtistThread::getDownLoadIntro(MusicResultsItem *item)
{
    if(!m_manager)
    {
        return;
    }

    QNetworkRequest request;
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(QQ_ARTIST_INFO_URL, false).arg(m_searchText);

    request.setUrl(musicUrl);
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    MusicQQArtistInfoConfigManager xml;
    xml.fromByteArray(reply->readAll());
    xml.readArtistInfoData(item);

}
