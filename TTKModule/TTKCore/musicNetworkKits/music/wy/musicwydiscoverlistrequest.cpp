#include "musicwydiscoverlistrequest.h"
#include "musicwyqueryinterface.h"

MusicWYDiscoverListRequest::MusicWYDiscoverListRequest(QObject *parent)
    : MusicDiscoverListRequest(parent)
{

}

void MusicWYDiscoverListRequest::startToSearch()
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch").arg(getClassName()));
    deleteAll();

    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_TOPLIST_URL, false),
                      MusicUtils::Algorithm::mdII(WY_TOPLIST_DATA_URL, false).arg(19723756));
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWYDiscoverListRequest::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
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
            if(value.contains("playlist") && value["code"].toInt() == 200)
            {
                value = value["playlist"].toMap();
                const QVariantList &datas = value["tracks"].toList();
                int where = datas.count();
                where = (where > 0) ? MusicTime::random(where) : 0;

                int counter = 0;
                for(const QVariant &var : qAsConst(datas))
                {
                    if(m_interrupt) return;

                    if((where != counter++) || var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    const QVariantList &artistsArray = value["ar"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }
                        const QVariantMap &artistMap = artistValue.toMap();
                        m_toplistInfo = artistMap["name"].toString();
                    }

                    m_toplistInfo += " - " + value["name"].toString();
                    break;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(m_toplistInfo);
    deleteAll();
}
