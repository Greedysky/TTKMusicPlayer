#include "musicdjradioprogramrequest.h"

MusicDJRadioProgramRequest::MusicDJRadioProgramRequest(QObject *parent)
    : MusicAbstractDJRadioRequest(parent)
{

}

void MusicDJRadioProgramRequest::startToRequest(TTK::Program type)
{
    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request, type == TTK::Program::Recommed ?
                      TTK::Algorithm::mdII(DJ_RECOMMEND_URL, false):
                      TTK::Algorithm::mdII(DJ_HOT_URL, false),
                      TTK::Algorithm::mdII(DJ_HOT_DATA_URL, false));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicDJRadioProgramRequest::downLoadFinished()
{
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

                    MusicResultDataItem result;
                    result.m_name = value["name"].toString();
                    result.m_id = value["id"].toString();
                    result.m_coverUrl = value["picUrl"].toString();
                    result.m_category = value["category"].toString();
                    result.m_nickName = value["categoryId"].toString();
                    result.m_updateTime = TTKDateTime::format(value["createTime"].toULongLong(), TTK_YEAR_FORMAT);
                    result.m_count = value["subCount"].toString();
                    Q_EMIT createProgramItem(result);
                    m_items << result;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
