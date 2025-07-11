#include "musicdjradiocategoryrequest.h"

MusicDJRadioCategoryRequest::MusicDJRadioCategoryRequest(QObject *parent)
    : MusicAbstractDJRadioRequest(parent)
{

}

void MusicDJRadioCategoryRequest::startToRequest()
{
    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(DJ_CATEGORY_URL, false),
                      QString("{}"));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicDJRadioCategoryRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    m_items.clear();
    MusicAbstractDJRadioRequest::downLoadFinished();

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["code"].toInt() == 200 && value.contains("categories"))
            {
                const QVariantList &datas = value["categories"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem item;
                    item.m_name = value["name"].toString();
                    item.m_id = value["id"].toString();
                    item.m_coverUrl = value["picMacUrl"].toString();
                    m_items << item;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
