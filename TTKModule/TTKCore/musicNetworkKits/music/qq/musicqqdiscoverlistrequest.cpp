#include "musicqqdiscoverlistrequest.h"
#include "musicqqqueryinterface.h"

MusicQQDiscoverListRequest::MusicQQDiscoverListRequest(QObject *parent)
    : MusicDiscoverListRequest(parent)
{

}

void MusicQQDiscoverListRequest::startToSearch()
{
    TTK_LOGGER_INFO(QString("%1 startToSearch").arg(className()));

    deleteAll();
    m_toplistInfo.clear();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_TOPLIST_URL, false).arg(4));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicQQDiscoverListRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicDiscoverListRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0 && value.contains("songlist"))
            {
                const QVariantList &datas = value["songlist"].toList();
                int where = datas.count();
                where = (where > 0) ? MusicTime::random(where) : 0;

                int counter = 0;
                for(const QVariant &var : qAsConst(datas))
                {
                    if((where != counter++) || var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    value = value["data"].toMap();
                    const QVariantList &artistsArray = value["singer"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        m_toplistInfo = artistObject["name"].toString();
                    }

                    m_toplistInfo += " - " + value["songname"].toString();
                    break;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(m_toplistInfo);
    deleteAll();
}
