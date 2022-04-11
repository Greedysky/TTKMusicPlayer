#include "musicwyartistsimilarrequest.h"

MusicWYArtistSimilarRequest::MusicWYArtistSimilarRequest(QObject *parent)
    : MusicSimilarRequest(parent)
{

}

void MusicWYArtistSimilarRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_ARTIST_SIMILAR_URL, false),
                      MusicUtils::Algorithm::mdII(WY_ARTIST_SIMILAR_DATA_URL, false).arg(value));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicWYArtistSimilarRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicSimilarRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("artists"))
            {
                const QVariantList &datas = value["artists"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem info;
                    info.m_id = QString::number(value["id"].toULongLong());
                    info.m_coverUrl = value["picUrl"].toString();
                    info.m_name = value["name"].toString();
                    info.m_updateTime.clear();
                    Q_EMIT createSimilarItem(info);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
