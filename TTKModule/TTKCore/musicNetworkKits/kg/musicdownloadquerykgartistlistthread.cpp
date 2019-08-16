#include "musicdownloadquerykgartistlistthread.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKGArtistListThread::MusicDownLoadQueryKGArtistListThread(QObject *parent)
    : MusicDownLoadQueryArtistListThread(parent)
{
    m_pageSize = DEFAULT_LEVEL_HIGHER;
    m_pageTotal = DEFAULT_LEVEL_HIGHER;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicDownLoadQueryKGArtistListThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToPage %2").arg(getClassName()).arg(offset));
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
            m_rawData["initial"] = QString(MStatic_cast(char, mIdx + 65));
        }
        else if(mIdx >= 26)
        {
            m_rawData["initial"] = "%E5%85%B6%E4%BB%96";
        }
    }
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KG_AR_LIST_URL, false).arg(catId);
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryKGArtistListThread::startToSearch(const QString &artistlist)
{
    m_searchText = artistlist;
    m_searchText.replace("n", "&");
    startToPage(0);
}

void MusicDownLoadQueryKGArtistListThread::downLoadFinished()
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
            if(value.contains("data") && value["errcode"].toInt() == 0)
            {
                value = value["data"].toMap();
                const QVariantList &datas = value["info"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(m_interrupt) return;

                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();

                    if(m_rawData["initial"].toString() == QUrl(value["title"].toString()).toEncoded())
                    {
                        foreach(const QVariant &sg, value["singer"].toList())
                        {
                            if(m_interrupt) return;

                            if(sg.isNull())
                            {
                                continue;
                            }

                            value = sg.toMap();

                            MusicResultsItem info;
                            info.m_id = QString::number(value["singerid"].toLongLong());
                            info.m_name = value["singername"].toString();
                            emit createArtistListItem(info);
                        }
                        break;
                    }
                }
            }
        }
    }

//    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
