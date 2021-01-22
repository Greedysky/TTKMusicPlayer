#include "musickgqueryartistlistrequest.h"

MusicKGQueryArtistListRequest::MusicKGQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = DEFAULT_LEVEL_HIGHER;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryArtistListRequest::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToPage %2").arg(getClassName()).arg(offset));

    deleteAll();
    QString catId = "type=1&sextype=1";
    m_rawData["initial"] = "%E7%83%AD%E9%97%A8";
    const QStringList &dds = m_searchText.split(TTK_STR_SPLITER);
    if(dds.count() == 2)
    {
        catId = dds[0];
        if(catId.isEmpty())
        {
            catId = "area=1&sex=1";
        }

        int mIdx = dds[1].toInt();
        if(mIdx > -1 && mIdx < 26)
        {
            m_rawData["initial"] = QString(TTKStatic_cast(char, mIdx + 65));
        }
        else if(mIdx >= 26)
        {
            m_rawData["initial"] = "%E5%85%B6%E4%BB%96";
        }
    }
    m_totalSize = DEFAULT_LEVEL_HIGHER;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_ARTIST_LIST_URL, false).arg(catId));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicKGQueryArtistListRequest::startToSearch(const QString &artistlist)
{
    m_searchText = artistlist;
    m_searchText.replace("n", "&");
    startToPage(0);
}

void MusicKGQueryArtistListRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
    setNetworkAbort(false);

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data") && value["errcode"].toInt() == 0)
            {
                value = value["data"].toMap();
                const QVariantList &datas = value["info"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    if(m_rawData["initial"].toString() == QUrl(value["title"].toString()).toEncoded())
                    {
                        for(const QVariant &sg : value["singer"].toList())
                        {
                            if(sg.isNull())
                            {
                                continue;
                            }

                            value = sg.toMap();
                            TTK_NETWORK_QUERY_CHECK();

                            MusicResultsItem info;
                            info.m_id = QString::number(value["singerid"].toLongLong());
                            info.m_name = value["singername"].toString();
                            Q_EMIT createArtistListItem(info);
                        }
                        break;
                    }
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
