#include "musicqqartistsimilarrequest.h"
#include "musicqqqueryinterface.h"

MusicQQArtistSimilarRequest::MusicQQArtistSimilarRequest(QObject *parent)
    : MusicSimilarRequest(parent)
{

}

void MusicQQArtistSimilarRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_ARTIST_SIMILAR_URL, false).arg(value));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicQQArtistSimilarRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
        bytes.replace("SingerSimCallback(", "");
        bytes.chop(1);

        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("singers"))
            {
                value = value["singers"].toMap();
                const QVariantList &datas = value["items"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem info;
                    info.m_id = value["mid"].toString();
                    info.m_coverUrl = value["pic"].toString();
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
