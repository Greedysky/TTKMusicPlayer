#include "musicdjradioprogramrequest.h"

#include <QDateTime>

MusicDJRadioProgramRequest::MusicDJRadioProgramRequest(QObject *parent)
    : MusicAbstractDJRadioRequest(parent)
{

}

void MusicDJRadioProgramRequest::startToDownload(MusicObject::Program type)
{
    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = makeTokenQueryUrl(&request, type == MusicObject::Recommed ?
                      MusicUtils::Algorithm::mdII(DJ_RECOMMEND_URL, false):
                      MusicUtils::Algorithm::mdII(DJ_HOT_URL, false),
                      MusicUtils::Algorithm::mdII(DJ_HOT_DATA_URL, false));

    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicDJRadioProgramRequest::downLoadFinished()
{
    m_items.clear();
    MusicAbstractDJRadioRequest::downLoadFinished();

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(m_reply->readAll(), &ok);
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

                    MusicResultsItem item;
                    item.m_name = value["name"].toString();
                    item.m_id = value["id"].toString();
                    item.m_coverUrl = value["picUrl"].toString();
                    item.m_tags = value["category"].toString();
                    item.m_nickName = value["categoryId"].toString();
                    item.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["createTime"].toULongLong()).toString(MUSIC_YEAR_FORMAT);
                    item.m_playCount = value["subCount"].toString();
                    Q_EMIT createProgramItem(item);
                    m_items << item;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
