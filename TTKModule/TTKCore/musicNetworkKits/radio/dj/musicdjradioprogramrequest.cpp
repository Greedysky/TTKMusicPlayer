#include "musicdjradioprogramrequest.h"

MusicDJRadioProgramRequest::MusicDJRadioProgramRequest(QObject *parent)
    : MusicAbstractDJRadioRequest(parent)
{

}

void MusicDJRadioProgramRequest::startToRequest(TTK::Program type)
{
    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request, type == TTK::Program::Recommed ?
                      TTK::Algorithm::mdII(DJ_RECOMMEND_URL, false):
                      TTK::Algorithm::mdII(DJ_HOT_URL, false),
                      TTK::Algorithm::mdII(DJ_HOT_DATA_URL, false));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicDJRadioProgramRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    m_items.clear();
    MusicAbstractDJRadioRequest::downLoadFinished();

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("djRadios"))
            {
                const QVariantList &datas = value["djRadios"].toList();
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
                    item.m_coverUrl = value["picUrl"].toString();
                    item.m_category = value["category"].toString();
                    item.m_nickName = value["categoryId"].toString();
                    item.m_time = TTKDateTime::format(value["createTime"].toULongLong(), TTK_DATE_FORMAT);
                    item.m_count = value["subCount"].toString();
                    Q_EMIT createProgramItem(item);
                    m_items << item;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
