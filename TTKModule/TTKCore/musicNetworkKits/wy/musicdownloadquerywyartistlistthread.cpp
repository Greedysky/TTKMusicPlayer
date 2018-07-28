#include "musicdownloadquerywyartistlistthread.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryWYArtistListThread::MusicDownLoadQueryWYArtistListThread(QObject *parent)
    : MusicDownLoadQueryArtistListThread(parent)
{
    m_pageSize = DEFAULT_LEVEL_HIGHER;
    m_pageTotal = DEFAULT_LEVEL_HIGHER;
    m_queryServer = "WangYi";
}

void MusicDownLoadQueryWYArtistListThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToPage %2").arg(getClassName()).arg(offset));
    QString catId = "1001", initial = "-1";
    QStringList dds = m_searchText.split(TTK_STR_SPLITER);
    if(dds.count() == 2)
    {
        catId = dds[0];
        if(catId.isEmpty())
        {
            catId = "1001";
        }

        int mIdx = dds[1].toInt();
        if(mIdx > -1 && mIdx < 26)
        {
            mIdx *= 10;
        }
        else if(mIdx >= 26)
        {
            mIdx = 0;
        }

        initial = QString::number(mIdx);
    }

    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    QByteArray parameter = makeTokenQueryUrl(&request,
               MusicUtils::Algorithm::mdII(WY_AR_LIST_N_URL, false),
               MusicUtils::Algorithm::mdII(WY_AR_LIST_DATA_N_URL, false).arg(catId).arg(initial).arg(10));
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    setSslConfiguration(&request);

    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryWYArtistListThread::startToSearch(const QString &artistlist)
{
    m_searchText = artistlist;
    startToPage(0);
}

void MusicDownLoadQueryWYArtistListThread::downLoadFinished()
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
        QByteArray bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("artists"))
            {
                QVariantList datas = value["artists"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    value = var.toMap();
                    MusicResultsItem info;
                    info.m_id = value["id"].toString();
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
