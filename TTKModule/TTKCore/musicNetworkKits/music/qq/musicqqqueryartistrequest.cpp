#include "musicqqqueryartistrequest.h"

#define REFER_URL   "a2tQNGw2RlJyeC9sZjc5RXpsM2hZRXZUeWd0UVJycE1oTzI5MkRQbVRQemN3WUhtOHJiV3FzcS95ZVU9"

MusicQQArtistInfoConfigManager::MusicQQArtistInfoConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicQQArtistInfoConfigManager::readBuffer(MusicResultsItem *item)
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

void MusicQQQueryArtistRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();
    m_queryValue = value;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_ARTIST_URL, false).arg(value).arg(0).arg(50));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
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
                    info.m_lrcUrl = MusicUtils::Algorithm::mdII(QQ_SONG_LRC_URL, false).arg(info.m_songId);
                    info.m_coverUrl = MusicUtils::Algorithm::mdII(QQ_SONG_PIC_URL, false).arg(info.m_albumId);

                    info.m_albumName = MusicUtils::String::charactersReplaced(value["albumname"].toString());

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

                    if(!artistFound)
                    {
                        artistFound = true;
                        MusicResultsItem result;
                        TTK_NETWORK_QUERY_CHECK();
                        downLoadIntro(&result);
                        TTK_NETWORK_QUERY_CHECK();
                        result.m_id = info.m_artistId;
                        result.m_name = info.m_singerName;
                        result.m_coverUrl = info.m_coverUrl;
                        Q_EMIT createArtistInfoItem(result);
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

void MusicQQQueryArtistRequest::downLoadIntro(MusicResultsItem *item) const
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_ARTIST_INFO_URL, false).arg(m_queryValue));
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    MusicQQInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    MusicQQArtistInfoConfigManager xml;
    xml.fromByteArray(bytes);
    xml.readBuffer(item);
}
