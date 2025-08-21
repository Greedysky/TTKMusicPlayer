#include "musicwydiscoverlistrequest.h"
#include "musicwyqueryinterface.h"

MusicWYDiscoverListRequest::MusicWYDiscoverListRequest(QObject *parent)
    : MusicDiscoverListRequest(parent)
{

}

void MusicWYDiscoverListRequest::startToSearch()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    deleteAll();
    m_item.clear();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_TOPLIST_URL, false),
                      TTK::Algorithm::mdII(WY_TOPLIST_DATA_URL, false).arg(19723756).arg(0).arg(TOPLIST_PAGE_SIZE));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYDiscoverListRequest::downLoadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicDiscoverListRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["code"].toInt() == 200 && value.contains("playlist"))
            {
                value = value["playlist"].toMap();

                const QVariantList &datas = value["tracks"].toList();
                int where = datas.count();
                where = (where > 0) ? TTK::random(where) : 0;

                int counter = 0;
                for(const QVariant &var : qAsConst(datas))
                {
                    if((where != counter++) || var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    const QVariantList &artistsArray = value["ar"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        m_item = artistObject["name"].toString();
                    }

                    m_item += " - " + value["name"].toString();
                    break;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(m_item);
    deleteAll();
}
