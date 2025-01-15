#include "musicmvradiocategoryrequest.h"

MusicMVRadioCategoryRequest::MusicMVRadioCategoryRequest(QObject *parent)
    : MusicAbstractMVRadioRequest(parent)
{

}

void MusicMVRadioCategoryRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicAbstractMVRadioRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                const QVariantList &datas = value["data"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    QVariantMap value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    if(value["classId"].toString() == m_queryValue)
                    {
                        const QVariantList &fms = value["fm_list"].toList();
                        for(const QVariant &var : qAsConst(fms))
                        {
                            if(var.isNull())
                            {
                                continue;
                            }

                            value = var.toMap();
                            TTK_NETWORK_QUERY_CHECK();

                            MusicResultDataItem item;
                            item.m_name = value["fmName"].toString();
                            item.m_id = value["fmId"].toString();
                            item.m_coverUrl = value["imgUrlMv"].toString();
                            Q_EMIT createMVRadioItem(item);
                        }
                    }
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
