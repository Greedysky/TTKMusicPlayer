#include "musicxmdiscoverlistthread.h"
#include "musicdownloadxminterface.h"
#///QJson import
#include "qjson/parser.h"

MusicXMDiscoverListThread::MusicXMDiscoverListThread(QObject *parent)
    : MusicDownLoadDiscoverListThread(parent)
{

}

void MusicXMDiscoverListThread::startToSearch()
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch").arg(getClassName()));
    m_toplistInfo.clear();
    deleteAll();

    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(XM_SONG_TOPLIST_DATA_URL, false).arg(103),
                      MusicUtils::Algorithm::mdII(XM_SONG_TOPLIST_URL, false));
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicXMDiscoverListThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["data"].toMap();
                value = value["billboard"].toMap();
                const QVariantList &datas = value["items"].toList();
                int where = datas.count();
                where = (where > 0) ? qrand()%where : 0;

                int counter = 0;
                foreach(const QVariant &var, datas)
                {
                    if(m_interrupt) return;

                    if((where != counter++) || var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    m_toplistInfo = QString("%1 - %2").arg(value["artistName"].toString()).arg(value["songName"].toString());
                    break;
                }
            }
        }
    }

    emit downLoadDataChanged(m_toplistInfo);
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
