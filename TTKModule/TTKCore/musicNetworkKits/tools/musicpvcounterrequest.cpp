#include "musicpvcounterrequest.h"

#include "qsync/qsyncutils.h"

static constexpr const char *QUERY_COUNTERS_URL = "counters";

MusicPVCounterRequest::MusicPVCounterRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicPVCounterRequest::startToRequest()
{
    QByteArray bytes;
    QFile file(RESOURCE_DIR_FULL + QUERY_COUNTERS_URL);
    if(file.open(QIODevice::ReadOnly))
    {
        TTK_INFO_STREAM("PV counters using local resource config");
        bytes = file.readAll();
        file.close();
    }
    else
    {
        TTK_INFO_STREAM("PV counters using network resource config");
        QNetworkRequest request;
        request.setUrl(QSyncUtils::makeDataBucketUrl() + RESOURCE_DIR + QUERY_COUNTERS_URL);
        TTK::setUserAgentHeader(&request);
        bytes = TTK::syncNetworkQueryForGet(&request);
    }

    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Counters PV data error");
        Q_EMIT downloadDataChanged(TTK_DEFAULT_STR);
        return;
    }

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        const QVariantList &servers = json.toVariant().toList();
        for(const QVariant &var : qAsConst(servers))
        {
            if(var.isNull())
            {
                continue;
            }

            const QVariantMap &server = var.toMap();
            const bool option = server["option"].toBool();

            if(!option)
            {
                continue;
            }

            QNetworkRequest request;
            request.setUrl(TTK::Algorithm::mdII(server["base"].toString(), false));
            TTK::setUserAgentHeader(&request);
            TTK::setSslConfiguration(&request);
            TTK::setContentTypeHeader(&request);

            if(server.contains("body"))
            {
                m_reply = m_manager.post(request, TTK::Algorithm::mdII(server["body"].toString(), false).toUtf8());
            }
            else
            {
                m_reply = m_manager.get(request);
            }

            connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
            QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
        }
    }
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
