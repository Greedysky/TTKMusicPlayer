#include "musicqqqueryartistlistrequest.h"

MusicQQQueryArtistListRequest::MusicQQQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = 100;
    m_queryServer = QUERY_QQ_INTERFACE;
}

void MusicQQQueryArtistListRequest::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToPage %2").arg(getClassName()).arg(offset));
    deleteAll();

    QString catId = "cn_man_", initial = "all";
    const QStringList &dds = m_searchText.split(TTK_STR_SPLITER);
    if(dds.count() == 2)
    {
        catId = dds[0];
        if(catId.isEmpty())
        {
            catId = "cn_man_";
        }

        const int mIdx = dds[1].toInt();
        if(mIdx > -1 && mIdx < 26)
        {
            initial = QString(TTKStatic_cast(char, mIdx + 65));
        }
        else if(mIdx >= 26)
        {
            initial = "9";
        }
    }
    catId += initial;

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(QQ_AR_LIST_URL, false).arg(catId).arg(m_pageSize).arg(offset + 1);
    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicQQQueryArtistListRequest::startToSearch(const QString &artistlist)
{
    m_searchText = artistlist;
    startToPage(0);
}

void MusicQQQueryArtistListRequest::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    Q_EMIT clearAllItems();
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
            if(value.contains("data") && value["code"].toInt() == 0)
            {
                value = value["data"].toMap();
                m_pageTotal = value["total"].toLongLong();
                const QVariantList &datas = value["list"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(m_interrupt) return;

                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();

                    MusicResultsItem info;
                    info.m_id = value["Fsinger_mid"].toString();
                    info.m_name = value["Fsinger_name"].toString();
                    Q_EMIT createArtistListItem(info);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
