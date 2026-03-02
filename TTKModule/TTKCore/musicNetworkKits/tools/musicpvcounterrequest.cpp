#include "musicpvcounterrequest.h"

#include "qsync/qsyncutils.h"

static constexpr const char *QUERY_COUNTER_URL = "resource/counter";

MusicPVCounterRequest::MusicPVCounterRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicPVCounterRequest::startToRequest()
{
    QByteArray bytes;
    QFile file(APPDATA_DIR_FULL + QUERY_COUNTER_URL);
    if(file.open(QIODevice::ReadOnly))
    {
        TTK_INFO_STREAM("PV counter using local resource config");
        bytes = file.readAll();
        file.close();
    }
    else
    {
        TTK_INFO_STREAM("PV counter using network resource config");
        QNetworkRequest request;
        request.setUrl(QSyncUtils::makeDataBucketUrl() + QUERY_COUNTER_URL);
        bytes = TTK::syncNetworkQueryForGet(&request);
    }

    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Counter PV data error");
        Q_EMIT downloadDataChanged(TTK_DEFAULT_STR);
        return;
    }

    QNetworkRequest request;
    request.setUrl(QString::fromUtf8(bytes));
    TTK::setSslConfiguration(&request);
    TTK::setContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicPVCounterRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    QString pv = TTK_DEFAULT_STR;
    MusicAbstractNetwork::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();

                if(value.contains("page_pv"))
                {
                    pv = value["page_pv"].toString();
                }
                else if(value.contains("site_pv"))
                {
                    pv = value["site_pv"].toString();
                }
            }
        }
    }

    Q_EMIT downloadDataChanged(pv);
    deleteAll();
}
