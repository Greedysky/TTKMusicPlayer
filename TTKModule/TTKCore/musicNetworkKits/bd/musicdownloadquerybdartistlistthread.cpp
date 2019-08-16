#include "musicdownloadquerybdartistlistthread.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryBDArtistListThread::MusicDownLoadQueryBDArtistListThread(QObject *parent)
    : MusicDownLoadQueryArtistListThread(parent)
{
    m_pageSize = 100;
    m_queryServer = QUERY_BD_INTERFACE;
}

void MusicDownLoadQueryBDArtistListThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToPage %2").arg(getClassName()).arg(offset));
    deleteAll();

    QString catId = "area=1&sex=1", initial = "%E7%83%AD%E9%97%A8";
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
            initial = QString(MStatic_cast(char, mIdx + 65));
        }
        else if(mIdx >= 26)
        {
            initial = "%E5%85%B6%E4%BB%96";
        }
    }
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_AR_LIST_URL, false).arg(catId).arg(m_pageSize).arg(offset*m_pageSize).arg(initial);
    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryBDArtistListThread::startToSearch(const QString &artistlist)
{
    m_searchText = artistlist;
    m_searchText.replace("n", "&");
    startToPage(0);
}

void MusicDownLoadQueryBDArtistListThread::downLoadFinished()
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
            if(value.contains("artist"))
            {
                m_pageTotal = value["nums"].toLongLong();
                const QVariantList &datas = value["artist"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(m_interrupt) return;

                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();

                    MusicResultsItem info;
                    info.m_id = value["ting_uid"].toString();
                    info.m_name = value["name"].toString();
                    emit createArtistListItem(info);
                }
            }
        }
    }

//    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
